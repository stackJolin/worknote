## GCD如何实现依赖

----------

关于GCD实现依赖，有下面几种方式：

- dispatch_group
- dispatch_semphore

举例介绍：

我们有三个任务：A、B、C，C依赖于B，B依赖于A，如何实现？



#### dispatch_group实现依赖

------

```objective-c


```



#### dispatch_semphore实现依赖

--------

```objective-c
// 创建两个信号量
dispatch_semaphore_t sem1 = dispatch_semaphore_create(0);
dispatch_semaphore_t sem2 = dispatch_semaphore_create(0);

// 添加任务C
dispatch_async(dispatch_get_global_queue(0, 0), ^{
		dispatch_semaphore_wait(sem1, DISPATCH_TIME_FOREVER);
    NSLog(@"C");
});
// 添加任务B
dispatch_async(dispatch_get_global_queue(0, 0), ^{
		dispatch_semaphore_wait(sem2, DISPATCH_TIME_FOREVER);
		NSLog(@"B");
  	dispatch_semaphore_signal(sem1);
});
// 添加任务A
dispatch_async(dispatch_get_global_queue(0, 0), ^{
		NSLog(@"任务1");
    dispatch_semaphore_signal(sem2);
});
```

