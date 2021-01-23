## pthread - condition variable

-------

该文档主要介绍`pthread`中的另一个功能：条件锁。条件锁适用于什么场景呢？

如果一个线程需要等待某一条件才能继续执行，而这个条件是由别的线程产生的，这时候只用锁就有点捉襟见肘了。要么不停的轮询，消耗资源，要么每隔一段时间查询一次，丧失了及时性。 条件变量就是为了满足这种场景而生的。它可以让一个线程等待某一条件，当条件满足时，会收到通知。 在获取条件变量并等待条件发生的过程中，也会产生多线程的竞争，所以条件变量通常会和互斥锁一起工作。



#### 基本使用

-----

```objective-c
pthread_cond_init(&_cond, NULL);

- (void)testCondition {
		dispatch_queue_t queue = dispatch_get_global_queue(0, 0);
		dispatch_async(queue, ^{
    		for (NSInteger i = 0; i < 10; i ++) {
        		[self add];
    		}
		});
  	
		dispatch_async(queue, ^{
    		for (NSInteger i = 0; i < 10; i ++) {
        		[self remove];
    		}
		});
}

- (void)add {
		pthread_mutex_lock(&_mutex);
		[self.mutableArr addObject:@"123"];
		pthread_cond_signal(&_cond);
		pthread_mutex_unlock(&_mutex);
}

- (void)remove {
		pthread_mutex_lock(&_mutex);
		if (self.mutableArr.count == 0) {
    		pthread_cond_wait(&_cond, &_mutex);
		}
		[self.mutableArr removeLastObject];
		pthread_mutex_unlock(&_mutex);
}
```

可以看到，上面是`pthread_cond`和`pthread_mutex`结合使用，之所以加互斥锁，是为了保证`mutableArr`的线程安全性。



#### pthread_cond_broadcast与pthread_cond_signal的区别

--------

###### pthread_cond_broadcast:

- 一个生产者，多个消费者，生产者能一次性生产多个产品的情况
- 多个生产者和多个消费者的情况
- 读写锁的实现



###### pthread_cond_signal：

- 单一生产者，生产者一次生产一个产品，最好是一个消费者

