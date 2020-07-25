## GCD Timer详解

GCD timer在我们iOS开发中主要有两个使用场景：

- 定时器
- 倒计时

NSTimer和performSelector:after的方法，实现上面的功能的时候存在的问题，请查看NSTimer详解



```
/** 获取一个全局的线程来运行计时器*/
dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
/** 创建一个计时器*/
dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
/** 设置计时器, 这里是每10毫秒执行一次*/
dispatch_source_set_timer(timer, dispatch_walltime(nil, 0), 10*NSEC_PER_MSEC, 0);
/** 设置计时器的里操作事件*/
dispatch_source_set_event_handler(timer, ^{
	//do you want....
});
```



