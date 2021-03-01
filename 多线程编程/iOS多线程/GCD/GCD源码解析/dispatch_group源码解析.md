## dispatch_group源码解析

-----------

涉及到的主要的几个API：

- `dispatch_group_create`
- `dispatch_group_enter`
- `dispatch_group_leave`
- `dispatch_group_wait`
- `dispatch_group_notify`
- `dispatch_group_async`

基本的数据结构：

```c++
struct dispatch_group_s {
	DISPATCH_OBJECT_HEADER(group);
	DISPATCH_UNION_LE(uint64_t volatile dg_state,
			uint32_t dg_bits,
			uint32_t dg_gen
	) DISPATCH_ATOMIC64_ALIGN;
	struct dispatch_continuation_s *volatile dg_notify_head;
	struct dispatch_continuation_s *volatile dg_notify_tail;
};
```



#### dispatch_group_create

-------------------

```c++
dispatch_group_t dispatch_group_create(void) {
	return _dispatch_group_create_with_count(0);
}

static inline dispatch_group_t _dispatch_group_create_with_count(uint32_t n) {
  // 分配内存空间
	dispatch_group_t dg = _dispatch_object_alloc(DISPATCH_VTABLE(group),
			sizeof(struct dispatch_group_s));
	dg->do_next = DISPATCH_OBJECT_LISTLESS;
  // 设置目标队列
	dg->do_targetq = _dispatch_get_default_queue(false);
  // 如果 n > 0，这个时候
	if (n) {
		os_atomic_store2o(dg, dg_bits,
				(uint32_t)-n * DISPATCH_GROUP_VALUE_INTERVAL, relaxed);
		os_atomic_store2o(dg, do_ref_cnt, 1, relaxed); // <rdar://22318411>
	}
	return dg;
}
```

 

#### dispatch_group_enter

-------------

```c++
void dispatch_group_enter(dispatch_group_t dg) {
	// The value is decremented on a 32bits wide atomic so that the carry
	// for the 0 -> -1 transition is not propagated to the upper 32bits.
	uint32_t old_bits = os_atomic_sub_orig2o(dg, dg_bits,
			DISPATCH_GROUP_VALUE_INTERVAL, acquire);
	uint32_t old_value = old_bits & DISPATCH_GROUP_VALUE_MASK;
	if (unlikely(old_value == 0)) {
		_dispatch_retain(dg); // <rdar://problem/22318411>
	}
  // 处理后的value值，等于group的MAX，那么就crash
	if (unlikely(old_value == DISPATCH_GROUP_VALUE_MAX)) {
		DISPATCH_CLIENT_CRASH(old_bits,
				"Too many nested calls to dispatch_group_enter()");
	}
}
```



#### dispatch_group_leave

----------

```c++
//手动标识组中的一个任务块已经完成
void dispatch_group_leave(dispatch_group_t dg)
{	
  //原子性加1
  //#define os_atomic_add_orig2o(p, f, v, m) \os_atomic_add_orig(&(p)->f, (v), m)
  //#define os_atomic_add_orig(p, v, m) \_os_atomic_c11_op_orig((p), (v), m, add, +)
  uint64_t new_state, old_state = os_atomic_add_orig2o(dg, dg_state,
  DISPATCH_GROUP_VALUE_INTERVAL, release);
  uint32_t old_value = (uint32_t)(old_state & DISPATCH_GROUP_VALUE_MASK);

  if (unlikely(old_value==DISPATCH_GROUP_VALUE_1)) {
    old_state += DISPATCH_GROUP_VALUE_INTERVAL;
    do {
      new_state = old_state;
      if ((old_state & DISPATCH_GROUP_VALUE_MASK)==0) {
        new_state &=~DISPATCH_GROUP_HAS_WAITERS;
        new_state &= ~DISPATCH_GROUP_HAS_NOTIFS;
      } else {
        new_state &= ~DISPATCH_GROUP_HAS_NOTIFS;
      }
      if (old_state==new_state) break;
    } while (unlikely(!os_atomic_cmpxchgv2o(dg, dg_state, old_state, new_state, &old_state, relaxed)));
    //do循环是对value的循环设置,处理完后,会去唤醒group_wake
    //leave时应该会调用notify:
    //#define dx_wakeup(x, y, z) dx_vtable(x)->dq_wakeup(x, y, z)
    //函数是保存的wake函数调用
    //此处唤醒组,不会释放group
    return _dispatch_group_wake(dg, old_state, true);
  }

  // leave的调用多于enter时的crash:
  if (unlikely(old_value==0)) {
    DISPATCH_CLIENT_CRASH((uintptr_t)old_value, "Unbalanced call to dispatch_group_leave()");
  }
}

```





#### 以上是最新版本的`dispatch_group`的实现，老的版本的源码实现如下：

-------------------------------

http://www.yuanchengwen.cn/2020/05/06/iOS%E5%A4%9A%E7%BA%BF%E7%A8%8B%EF%BC%9AGCD%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90-%E4%BA%8C-dispatch-group/

关键点是，通过`_dispatch_semaphore_init(LONG_MAX,dg)`函数，给当前队列设置了一个最大的semphore的值。并且，只有在这个值小于0的时候，















