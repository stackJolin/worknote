## dispatch_barrier

---------

```markdown
一个dispatch barrier 允许在一个并发队列中创建一个同步点。当在并发队列中遇到一个barrier, 他会延迟执行barrier的block,等待所有在barrier之前提交的blocks执行结束。 这时，barrier block自己开始执行。 之后， 队列继续正常的执行操作。这里指定的并发队列应该是自己通过dispatch_queue_create函数创建的。如果你传的是一个串行队列或者全局并发队列，这个函数等同于dispatch_async函数。
所有在barrier block之后提交的blocks会等到barrier block结束之后才执行。
```



两个方法：

- dispatch_barrier_async
- dispatch_barrier_sync



#### dispatch_barrier_async

-------

一般适用于，多读单写的场景。可以见`AFNetworking`中的`mutableHTTPRequestHeaders`

```objective-c
- (void)barrierTest {
    //同dispatch_queue_create函数生成的concurrent Dispatch Queue队列一起使用
    dispatch_queue_t queue = dispatch_queue_create("12312312", DISPATCH_QUEUE_CONCURRENT);
    
    dispatch_async(queue, ^{
        NSLog(@"----read table1-----%@", [NSThread currentThread]);
    });
    dispatch_async(queue, ^{
        NSLog(@"----read table2-----%@", [NSThread currentThread]);
    });
    
    dispatch_barrier_async(queue, ^{
        NSLog(@"----barrier-- do something use both table1 and table2 -----%@", [NSThread currentThread]);
    });
    
    dispatch_async(queue, ^{
        NSLog(@"----do something after data operated in barrier -----%@", [NSThread currentThread]);
    });
}
```



#### dispatch_barrier_sync

--------

相对于`dispatch_barrier_aysnc`，`dispatch_barrier_sync`会阻塞当前线程





#### 和dispatch_group的区别

-------

`dispatch_barrier`只作用于某一个队列中的多个任务设置一个隔离点。而`dispatch_group`除了拥有这个能力之外，还可以在不同的队列设置一个隔离点，因此更加强大