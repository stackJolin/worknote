## pthread中的锁



#### iOS中常用的pthread_mutex

------

互斥锁。他的实现机制和信号量的实现机制相似，不像自旋锁一样`忙等`，而是阻塞线程并睡眠，需要进行上下文切换



###### 一般使用方法

````objective-c
/// 定义锁的属性
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
/// 创建锁
pthread_mutex_t mutex; // 初始化信号量
pthread_mutex_init(&mutex, &attr) // 创建锁
/// 申请锁
pthread_mutex_lock(&mutex); // 申请锁 或者使用 `pthread_mutex_trylock` 加锁成功返回0，否则返回错误码
///-----------------临界区

///-----------------end
/// 释放锁
pthread_mutex_unlock(&mutex); // 释放锁
/// 销毁锁
pthread_mutex_destroy(&mutex);
````

<font color=orange>一般情况下，一个线程只能申请一次锁，也只能在获得锁的情况下才能释放锁。多次申请锁或者释放未获得的锁，都会导致崩溃。</font>

<font color=red>假设在已经获得锁的情况下，再次申请锁，线程会因为等待锁而进入睡眠状态，因此就不可能再释放锁，从而导致死锁</font>

然而这种情况经常会发生，比如某个函数申请了锁，在临界区内又递归调用了自己。辛运的是 `pthread_mutex` 支持递归锁，也就是允许一个线程递归的申请锁，只要把 attr 的类型改成 `PTHREAD_MUTEX_RECURSIVE` 即可。

`pthread_mutexattr_settype`可设置的属性有以下四种：

- `PTHREAD_MUTEX_NORMAL`

  ```markdown
  默认值普通锁，当一个线程加锁以后，其他线程进入按照优先顺序进入等待队列，并且解锁的时候按照先入先出的方式获得锁。
  ```

- `PTHREAD_MUTEX_ERRORCHECK`

  ```markdown
  检错锁，当同一个线程获得同一个锁的时候，则返回EDEADLK，否则与普通锁处理一样。
  ```

- `PTHREAD_MUTEX_RECURSIVE`

  ```markdown
  递归锁。这里有别于上面的检错锁，同一个线程可以递归获得锁，但是加锁和解锁必须要一一对应。
  ```

- `PTHREAD_MUTEX_DEFAULT`

  ```
  适应锁，等待解锁之后重新竞争，没有等待队列。
  ```




#### 相关文章

----

- https://juejin.im/post/5a3887dbf265da432e5c1f2f