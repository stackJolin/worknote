## dispatch_apply

-----

```c++
void dispatch_apply(size_t iterations, dispatch_queue_t queue, DISPATCH_NOESCAPE void (^block)(size_t));
```



#### dispatch_apply应用于并发队列

------

```objective-c
dispatch_queue_t queue = dispatch_queue_create("queue", DISPATCH_QUEUE_CONCURRENT);
dispatch_apply(5, queue, ^(size_t i) {
  	NSLog(@"%@我开始执行 %zu times",[NSThread currentThread],i+1);
});
NSLog(@"done");
```

输出：

```objective-c
[3545:1502823] <NSThread: 0x125e0c240>{number = 1, name = main}我开始执行 1 times
[3545:1502839] <NSThread: 0x125e06070>{number = 2, name = (null)}我开始执行 2 times
[3545:1502839] <NSThread: 0x125e06070>{number = 2, name = (null)}我开始执行 4 times
[3545:1502839] <NSThread: 0x125e06070>{number = 2, name = (null)}我开始执行 5 times
[3545:1502823] <NSThread: 0x125e0c240>{number = 1, name = main}我开始执行 3 times
[3553:1504850] done
```



#### dispatch_apply应用于串行队列

------

```objective-c
dispatch_queue_t queue = dispatch_queue_create("queue", DISPATCH_QUEUE_SERIAL);
dispatch_apply(5, queue, ^(size_t i) {
    NSLog(@"%@我开始执行 %zu times",[NSThread currentThread],i+1);
});
NSLog(@"done");
```

输出：

```objective-c
[3535:1501629] <NSThread: 0x13ce03880>{number = 1, name = main}我开始执行 1 times
[3535:1501629] <NSThread: 0x13ce03880>{number = 1, name = main}我开始执行 2 times
[3535:1501629] <NSThread: 0x13ce03880>{number = 1, name = main}我开始执行 3 times
[3535:1501629] <NSThread: 0x13ce03880>{number = 1, name = main}我开始执行 4 times
[3535:1501629] <NSThread: 0x13ce03880>{number = 1, name = main}我开始执行 5 times
[3553:1504850] done
```

**总结：**
 dispatch_apply在**串行队列**中按照顺序执行，完全没有意义。在**并发队列**中创建了N个任务，如果是在异步队列中则异步执行（由打印结果显示），但并非所有任务不开辟线程，也有在主线程中完成的。最后由输出的“done”字符串可以看出但done一定会输出在最后的位置,因为dispatch_apply函数会**等待所有的处理结束**



#### dispatch_apply使用场景

--------

在某些场景下使用dispatch_apply会对性能有很大的提升，可以避免线程爆炸

```objective-c
//危险，可能导致线程爆炸以及死锁
for (int i = 0; i < 999; i++) {
   dispatch_async(q, ^{...});
}
dispatch_barrier_sync(q, ^{});

// 较优选择， GCD 会管理并发
dispatch_apply(999, q, ^(size_t i){...});
```

替代for循环，注意dispatch_apply用于并发队列是没有顺序的。必须cell的大小需要提前计算，可以用dispatch_apply替代for循环计算大小，效率更高

