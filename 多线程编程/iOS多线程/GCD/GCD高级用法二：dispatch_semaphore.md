## GCD高级用法二：dispatch_semaphore



#### 相关文章

----

- https://www.jianshu.com/p/ed61bd59b4af
- https://www.jianshu.com/p/4112a2be793f



#### dispatch_semaphore用法一：实现线程同步

----



#### dispatch_semaphore用法二：控制并发数量

-----



#### 源码解析

-----

semaphore.h

```objective-c
/*!
 * @function dispatch_semaphore_create
 *
 * @abstract
 * 用初始值创建新的计数信号量。
 *
 * @discussion
 * 当两个线程需要协调特定事件的完成时，将值传递给零值非常有用。
 * 传递大于零的值对于管理池大小等于该值的有限资源池非常有用
 *
 * @param value
 * 信号量的初始值.传递一个小于零的值将导致返回NULL
 *
 * @result
 * 创建成功返回创建的信号量，创建失败返回NULL.
 */
  dispatch_semaphore_t 
  dispatch_semaphore_create(long value);

/*!
 * @function dispatch_semaphore_wait
 *
 * @abstract
 * 等待（递减）信号量。
 *
 * @discussion
 * 信号量减一，如果结果值小于零，则此函数在返回之前以FIFO顺序等待。
 *
 * @param dsema
 * 信号量
 *
 * @param timeout
 * 何时超时（请参阅dispatch_time）。 为了方便起见，有DISPATCH_TIME_NOW = 0和DISPATCH_TIME_FOREVER = ~0ull两个常量。
 *
 * @result
 * 成功时返回零，如果发生超时则返回非零值
 */
  long 
  dispatch_semaphore_wait(dispatch_semaphore_t dsema, dispatch_time_t timeout);

/*!
 * @function dispatch_semaphore_signal
 *
 * @abstract
 * 信号（增加）一个信号量
 *
 * @discussion
 * 信号量加1。如果前一个值小于零，则此函数在返回之前唤醒等待的线程
 *
 * @param dsema
 * 在这个参数中传递NULL的结果是未定义的.
 *
 * @result
 * 如果线程被唤醒，该函数返回非零值。否则，返回零.
 */
  long
  dispatch_semaphore_signal(dispatch_semaphore_t dsema);
```

semaphore.c关于信号部分

```objective-c
#pragma mark -
#pragma mark dispatch_semaphore_t

struct dispatch_semaphore_vtable_s {
  DISPATCH_VTABLE_HEADER(dispatch_semaphore_s);
};

const struct dispatch_semaphore_vtable_s _dispatch_semaphore_vtable = {
  .do_type = DISPATCH_SEMAPHORE_TYPE,
  .do_kind = "semaphore",
  .do_dispose = _dispatch_semaphore_dispose,
  .do_debug = _dispatch_semaphore_debug,
};

dispatch_semaphore_t dispatch_semaphore_create(long value)
{
  dispatch_semaphore_t dsema;

  // 如果内部值为负数，则该值的绝对值等于等待线程的数量。因此，用负值初始化信号量是虚假的
  if (value < 0) {
      return NULL;
  }

  dsema = calloc(1, sizeof(struct dispatch_semaphore_s));

  /*
   const struct dispatch_semaphore_vtable_s _dispatch_semaphore_vtable = {
       .do_type = DISPATCH_SEMAPHORE_TYPE,
       .do_kind = "semaphore",
       .do_dispose = _dispatch_semaphore_dispose,
       .do_debug = _dispatch_semaphore_debug,
   };

   DISPATCH_QUEUE_PRIORITY_DEFAULT优先级的全局队列：
   {
       .do_vtable = &_dispatch_queue_root_vtable,
       .do_ref_cnt = DISPATCH_OBJECT_GLOBAL_REFCNT,
       .do_xref_cnt = DISPATCH_OBJECT_GLOBAL_REFCNT,
       .do_suspend_cnt = DISPATCH_OBJECT_SUSPEND_LOCK,
       .do_vtable = &_dispatch_semaphore_vtable,
       .do_ref_cnt = DISPATCH_OBJECT_GLOBAL_REFCNT,
       .do_xref_cnt = DISPATCH_OBJECT_GLOBAL_REFCNT,
       .dgq_thread_pool_size = MAX_THREAD_COUNT,
       .dq_label = "com.apple.root.default-overcommit-priority",
       .dq_running = 2,
       .dq_width = UINT32_MAX,
       .dq_serialnum = 7,
   }
   */
  if (fastpath(dsema)) {
      dsema->do_vtable = &_dispatch_semaphore_vtable;
      dsema->do_next = DISPATCH_OBJECT_LISTLESS;
      dsema->do_ref_cnt = 1;
      dsema->do_xref_cnt = 1;
      dsema->do_targetq = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
      dsema->dsema_value = value;
      dsema->dsema_orig = value;
#if USE_POSIX_SEM
      int ret = sem_init(&dsema->dsema_sem, 0, 0);
      DISPATCH_SEMAPHORE_VERIFY_RET(ret);
#endif
  }

  return dsema;
}

#if USE_MACH_SEM
static void _dispatch_semaphore_create_port(semaphore_t *s4) {
  kern_return_t kr;
  semaphore_t tmp;

  if (*s4) {
      return;
  }

  // lazily allocate the semaphore port

  // Someday:
  // 1) Switch to a doubly-linked FIFO in user-space.
  // 2) User-space timers for the timeout.
  // 3) Use the per-thread semaphore port.

  while ((kr = semaphore_create(mach_task_self(), &tmp, SYNC_POLICY_FIFO, 0))) {
      DISPATCH_VERIFY_MIG(kr);
      sleep(1);
  }

  if (!dispatch_atomic_cmpxchg(s4, 0, tmp)) {
      kr = semaphore_destroy(mach_task_self(), tmp);
      DISPATCH_SEMAPHORE_VERIFY_KR(kr);
  }

  _dispatch_safe_fork = false;
}
#endif

static void _dispatch_semaphore_dispose(dispatch_semaphore_t dsema)
{
  // 当前可用资源数目小于原始值，表示有线程正在执行任务，不可被dispose
  if (dsema->dsema_value < dsema->dsema_orig) {
      printf("BUG IN CLIENT OF LIBDISPATCH: Semaphore/group object deallocated while in use");
  }

#if USE_MACH_SEM
  kern_return_t kr;
  // 销毁dsema_port
  if (dsema->dsema_port) {
      kr = semaphore_destroy(mach_task_self(), dsema->dsema_port);
      DISPATCH_SEMAPHORE_VERIFY_KR(kr);
  }
  // 有线程正在等待，销毁dsema_waiter_port
  if (dsema->dsema_waiter_port) {
      kr = semaphore_destroy(mach_task_self(), dsema->dsema_waiter_port);
      DISPATCH_SEMAPHORE_VERIFY_KR(kr);
  }
#elif USE_POSIX_SEM
  int ret = sem_destroy(&dsema->dsema_sem);
  DISPATCH_SEMAPHORE_VERIFY_RET(ret);
#endif

  _dispatch_dispose(dsema);
}

static size_t _dispatch_semaphore_debug(dispatch_semaphore_t dsema, char *buf, size_t bufsiz)
{
  size_t offset = 0;
  offset += snprintf(&buf[offset], bufsiz - offset, "%s[%p] = { ",
          dx_kind(dsema), dsema);
  offset += _dispatch_object_debug_attr(dsema, &buf[offset], bufsiz - offset);
#if USE_MACH_SEM
  offset += snprintf(&buf[offset], bufsiz - offset, "port = 0x%u, ",
          dsema->dsema_port);
#endif
  offset += snprintf(&buf[offset], bufsiz - offset,
          "value = %ld, orig = %ld }", dsema->dsema_value, dsema->dsema_orig);
  return offset;
}

DISPATCH_NOINLINE
long
_dispatch_semaphore_signal_slow(dispatch_semaphore_t dsema)
{
  _dispatch_retain(dsema);
  // 仅仅是将dsema->dsema_sent_ksignals值加1
  (void)dispatch_atomic_inc2o(dsema, dsema_sent_ksignals);
  // 创建semaphore_t
  _dispatch_semaphore_create_port(&dsema->dsema_port);
  // 核心:利用系统的信号量库实现发送信号量的功能，表示现在可用的资源数目+1，这里是可创建的用于并行线程数目+1
  kern_return_t kr = semaphore_signal(dsema->dsema_port);
  // 如果kr返回不为真，打印错误
  do {
      if (kr) {
          printf("BUG IN CLIENT OF LIBDISPATCH: flawed group/semaphore logic");
      }
  } while (0);
  _dispatch_release(dsema);
  return 1;
}

long dispatch_semaphore_signal(dispatch_semaphore_t dsema) {
  // dispatch_atomic_release_barrier();
  // __sync_add_and_fetch((p), (v))
  // dispatch_atomic_inc2o(dsema, dsema_value)
  long value = dsema->dsema_value + 1;
  if (value > 0) {
      return 0;
  }

  if (slowpath(value == LONG_MIN)) {// 输出错误日志
      printf("BUG IN CLIENT OF LIBDISPATCH: Unbalanced call to dispatch_semaphore_signal()");
  }

  return _dispatch_semaphore_signal_slow(dsema);
}

DISPATCH_NOINLINE
static long
_dispatch_semaphore_wait_slow(dispatch_semaphore_t dsema, dispatch_time_t timeout)
{
  long orig;
again:
  // Mach信号似乎有时会虚假地醒来,因此，我们保持一个Mach信号量被发信号的次数的平行计数6880961
  // 判断dsema->dsema_sent_ksignals与orig是否相等，如果相等就返回YES，并将orig - 1的值赋给dsema->dsema_sent_ksignals
  while ((orig = dsema->dsema_sent_ksignals)) {
      if ((long)(dsema->dsema_sent_ksignals) == orig) {
          dsema->dsema_sent_ksignals = orig - 1;
          return 0;
      }
  }
#if USE_MACH_SEM
  mach_timespec_t _timeout;
  kern_return_t kr;

  _dispatch_semaphore_create_port(&dsema->dsema_port);

  // From xnu/osfmk/kern/sync_sema.c:
  // wait_semaphore->count = -1; /* we don't keep an actual count */
  //
  // The code above does not match the documentation, and that fact is
  // not surprising. The documented semantics are clumsy to use in any
  // practical way. The above hack effectively tricks the rest of the
  // Mach semaphore logic to behave like the libdispatch algorithm.

  switch (timeout) {
      default:
          do {
              uint64_t nsec = _dispatch_timeout(timeout);
              _timeout.tv_sec = (typeof(_timeout.tv_sec))(nsec / NSEC_PER_SEC);
              _timeout.tv_nsec = (typeof(_timeout.tv_nsec))(nsec % NSEC_PER_SEC);
              kr = slowpath(semaphore_timedwait(dsema->dsema_port, _timeout));
          } while (kr == KERN_ABORTED);

          if (kr != KERN_OPERATION_TIMED_OUT) {
              DISPATCH_SEMAPHORE_VERIFY_KR(kr);
              break;
          }
          // Fall through and try to undo what the fast path did to
          // dsema->dsema_value
      case DISPATCH_TIME_NOW:
          while ((orig = dsema->dsema_value) < 0) {
              if (dispatch_atomic_cmpxchg2o(dsema, dsema_value, orig, orig + 1)) {
                  return KERN_OPERATION_TIMED_OUT;
              }
          }
          // Another thread called semaphore_signal().
          // Fall through and drain the wakeup.
      case DISPATCH_TIME_FOREVER:
          do {
              kr = semaphore_wait(dsema->dsema_port);
          } while (kr == KERN_ABORTED);
          DISPATCH_SEMAPHORE_VERIFY_KR(kr);
          break;
  }
#elif USE_POSIX_SEM
  struct timespec _timeout;
  int ret;

  switch (timeout) {
      default:
          do {
              uint64_t nsec = _dispatch_timeout(timeout);
              _timeout.tv_sec = (typeof(_timeout.tv_sec))(nsec / NSEC_PER_SEC);
              _timeout.tv_nsec = (typeof(_timeout.tv_nsec))(nsec % NSEC_PER_SEC);
              ret = slowpath(sem_timedwait(&dsema->dsema_sem, &_timeout));
          } while (ret == -1 && errno == EINTR);

          if (ret == -1 && errno != ETIMEDOUT) {
              DISPATCH_SEMAPHORE_VERIFY_RET(ret);
              break;
          }
          // Fall through and try to undo what the fast path did to
          // dsema->dsema_value
      case DISPATCH_TIME_NOW:
          while ((orig = dsema->dsema_value) < 0) {
              if (dispatch_atomic_cmpxchg2o(dsema, dsema_value, orig, orig + 1)) {
                  errno = ETIMEDOUT;
                  return -1;
              }
          }
          // Another thread called semaphore_signal().
          // Fall through and drain the wakeup.
      case DISPATCH_TIME_FOREVER:
          do {
              ret = sem_wait(&dsema->dsema_sem);
          } while (ret != 0);
          DISPATCH_SEMAPHORE_VERIFY_RET(ret);
          break;
  }
#endif
  goto again;
}

long dispatch_semaphore_wait(dispatch_semaphore_t dsema, dispatch_time_t timeout)
{
  // 调用GCC内置的函数__sync_sub_and_fetch，实现减法的原子性操作。
  // __sync_sub_and_fetch(p, v)
  // long value = dsema->dsema_value - 1;
  long value = dispatch_atomic_dec2o(dsema, dsema_value);

  // value大于等于0 就立刻返回
  if (fastpath(value >= 0)) {
      return 0;
  }
  // value < 0 进入等待状态
  return _dispatch_semaphore_wait_slow(dsema, timeout);
}
```



#### 先看一下几个基本的结构体

-----

<font color='red'>**dispatch_semaphore_t或dispatch_semaphore_s**</font>

```c++
struct dispatch_semaphore_s {
    DISPATCH_STRUCT_HEADER(dispatch_semaphore_s, dispatch_semaphore_vtable_s);
    long dsema_value;   //当前信号量
    long dsema_orig;    //初始化信号量
    size_t dsema_sent_ksignals;
#if USE_MACH_SEM && USE_POSIX_SEM
#error "Too many supported semaphore types"
#elif USE_MACH_SEM
    semaphore_t dsema_port;
    semaphore_t dsema_waiter_port;
#elif USE_POSIX_SEM
    sem_t dsema_sem;
#else
#error "No supported semaphore type"
#endif
    size_t dsema_group_waiters;
    struct dispatch_sema_notify_s *dsema_notify_head; //notify链表头部
    struct dispatch_sema_notify_s *dsema_notify_tail; //notify链表尾部
};

typedef mach_port_t     semaphore_t;

struct dispatch_sema_notify_s {
    struct dispatch_sema_notify_s *volatile dsn_next; //下一个信号节点
    dispatch_queue_t dsn_queue; //操作的队列
    void *dsn_ctxt;             //上下文
    void (*dsn_func)(void *);   //执行函数
};
```

<font color='red'>**dispatch_semaphore_vtable_s**</font>

```c++
struct dispatch_semaphore_vtable_s {
    DISPATCH_VTABLE_HEADER(dispatch_semaphore_s);
};

const struct dispatch_semaphore_vtable_s _dispatch_semaphore_vtable = {
    .do_type = DISPATCH_SEMAPHORE_TYPE,
    .do_kind = "semaphore",
    .do_dispose = _dispatch_semaphore_dispose,
    .do_debug = _dispatch_semaphore_debug,
};
```

