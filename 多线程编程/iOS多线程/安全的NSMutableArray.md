## 线程安全的NSMutableArray

----

我们知道，NSMutableArray并不是一个线程安全的数组，那么怎么保证NSMutableArray的线程安全呢？关键点在于锁该如何选择

两种方案：

- dispatch_barrier
- dispatch_semphore



#### dispatch_barrier

------





#### dispatch_semphore

-----



