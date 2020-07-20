# NSConditionLock

NSLock是对pthread_mutex的封装

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



#### 源码分析

---





#### 相关文章

----

- https://xuyunan.github.io/posts/ios-lock-semaphore/

