## dispatch_barrier

--------

主要有两个API：

- dispatch_barrier_sync
- dispatch_barrier_async

`dispatch_barrier`简单使用介绍：

```markdown
dispatch_queue_t queue = dispatch_queue_create("queue", DISPATCH_QUEUE_CONCURRENT);
dispatch_async(queue, ^{
    // 任务1
    ...
});
dispatch_async(queue, ^{
    // 任务2
    ...
});
dispatch_async(queue, ^{
    // 任务3
    ...
});
dispatch_barrier_async(queue, ^{
    // 任务4
    ...
});
dispatch_async(queue, ^{
    // 任务5
    ...
});
dispatch_async(queue, ^{
    // 任务6
    ...
});
```

//dispatch_barrier_async的作用可以用一个词概括－－承上启下，它保证此前的任务都先于自己执行，此后的任务也迟于自己执行。本例中，任务4会在任务1、2、3都执行完之后执行，而任务5、6会等待任务4执行完后执行。

<font color=red>和dispatch_group类似，dispatch_barrier也是异步任务间的一种同步方式，可以在比如文件的读写操作时使用，保证读操作的准确性。
另外，有一点需要注意，dispatch_barrier_sync和dispatch_barrier_async只在自己创建的并发队列上有效，在全局(Global)并发队列、串行队列上，效果跟dispatch_(a)sync效果一样。</font>



#### dispatch_barrier_sync和dispatch_barrier_async的区别

--------

1、dispatch_barrier_sync将自己的任务插入到队列的时候，需要等待自己的任务结束之后才会继续插入被写在它后面的任务，然后执行它们

2、dispatch_barrier_async将自己的任务插入到队列之后，不会等待自己的任务结束，它会继续把后面的任务插入到队列，然后等待自己的任务结束后才执行后面任务。



#### `dispatch_barrier`应用场景一：实现多读单写

-------

主要需要涉及到三件事：

- 读者与读者并发
- 读者与写者互斥
- 写者与写者互斥

```objective-c
- (id)dataForKey:(NSString *)key {
    __block id data;
    //同步读取指定数据
    dispatch_sync(self.concurrentQueue, ^{
        data = [self.dict objectForKey:key];
    });
    return data;
}

- (void)setData:(id)data forKey:(NSString *)key {
    // 异步栅栏调用设置数据
    dispatch_barrier_async(self.concurrentQueue, ^{
        [self.dict setObject:data forKey:key];
    });
}
```





#### dispatch_barrier 需要注意的问题

-------









