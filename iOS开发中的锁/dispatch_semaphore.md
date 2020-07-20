## dispatch_semaphore实现锁

通过dispatch_semaphore控制并发, 实现锁机制。 dispatch_semaphore是用来控制并发数的，类似往篮子里放鸡蛋拿鸡蛋，停车场的车辆入库出库。 通过控制信号量值为0或者1，来实现锁。 dispatch_semaphore_wait会使信号量-1，dispatch_semaphore_signal会使信号量+1，信号量为0时线程等待，为1时方可继续执行。

```swift
dispatch_semaphore_t signal = dispatch_semaphore_create(1);
  
// Thread 1
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    NSLog(@"Thread 1 waiting");
    dispatch_semaphore_wait(signal, DISPATCH_TIME_FOREVER); // 信号量 -1
    NSLog(@"Thread 1 start doing something");
    [NSThread sleepForTimeInterval:4];
    NSLog(@"Thread 1 done");
    dispatch_semaphore_signal(signal); // 信号量 +1
    NSLog(@"Thread 1 signal");
});
    
// Thread 2
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    NSLog(@"Thread 2 waiting");
    dispatch_semaphore_wait(signal, DISPATCH_TIME_FOREVER);
    NSLog(@"Thread 2 start doing something");
    [NSThread sleepForTimeInterval:5];
    NSLog(@"Thread 2 done");
    dispatch_semaphore_signal(signal);
    NSLog(@"Thread 2 signal");
});
```



#### 相关文章

----

- https://xuyunan.github.io/posts/ios-lock-semaphore/

