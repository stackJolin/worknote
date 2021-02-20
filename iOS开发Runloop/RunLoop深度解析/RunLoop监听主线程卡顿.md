## RunLoop监听主线程卡顿

---------

###### 卡顿产生的原因：

- 复杂的UI，图文混排内容过多

- 主线程IO

- 主线程文件编解码

- 死锁

- 运算量过大

  

###### RunLoop监听卡顿的原理：

`RunLoop`有六个状态:

````
typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) {
    kCFRunLoopEntry , // 进入 loop
    kCFRunLoopBeforeTimers , // 触发 Timer 回调
    kCFRunLoopBeforeSources , // 触发 Source0 回调
    kCFRunLoopBeforeWaiting , // 等待 mach_port 消息
    kCFRunLoopAfterWaiting ), // 接收 mach_port 消息
    kCFRunLoopExit , // 退出 loop
    kCFRunLoopAllActivities  // loop 所有状态改变
}
````

如果`RunLoop`所在的线程，进入睡眠前的方法执行的时间过于长，导致无法睡眠；或者睡眠唤醒后，接收消息时间过长而无法进入下一次`RunLoop`，即可以认为是线程受阻了。如果是主线程的话，表现出来的就是卡顿。

```objc
static void runLoopObserverCallBack(CFRunLoopObserverRef observer, CFRunLoopActivity activity, void *info)
{
    MJMonitorRunloop *instance = [MJMonitorRunloop sharedInstance];
    // 记录状态值
    instance->_activity = activity;
    // 发送信号
    dispatch_semaphore_t semaphore = instance->_semaphore;
    dispatch_semaphore_signal(semaphore);
}

// 注册一个Observer来监测Loop的状态,回调函数是runLoopObserverCallBack
- (void)registerObserver
{
    // 设置Runloop observer的运行环境
    CFRunLoopObserverContext context = {0, (__bridge void *)self, NULL, NULL};
    // 创建Runloop observer对象
    _observer = CFRunLoopObserverCreate(kCFAllocatorDefault,
                                        kCFRunLoopAllActivities,
                                        YES,
                                        0,
                                        &runLoopObserverCallBack,
                                        &context);
    // 将新建的observer加入到当前thread的runloop
    CFRunLoopAddObserver(CFRunLoopGetMain(), _observer, kCFRunLoopCommonModes);
    // 创建信号
    _semaphore = dispatch_semaphore_create(0);
    
    __weak __typeof(self) weakSelf = self;
    // 在子线程监控时长
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        __strong __typeof(weakSelf) strongSelf = weakSelf;
        if (!strongSelf) {
            return;
        }
        while (YES) {
            if (strongSelf.isCancel) {
                return;
            }
            // N次卡顿超过阈值T记录为一次卡顿
            long dsw = dispatch_semaphore_wait(self->_semaphore, dispatch_time(DISPATCH_TIME_NOW, strongSelf.limitMillisecond * NSEC_PER_MSEC));
            if (dsw != 0) {
                if (self->_activity == kCFRunLoopBeforeSources || self->_activity == kCFRunLoopAfterWaiting) {
                    if (++strongSelf.countTime < strongSelf.standstillCount){
                        NSLog(@"%ld",strongSelf.countTime);
                        continue;
                    }
                    [strongSelf logStack];
                    [strongSelf printLogTrace];
                    
                    NSString *backtrace = [MJCallStack mj_backtraceOfMainThread];
                    NSLog(@"++++%@",backtrace);
                    
                    if (strongSelf.callbackWhenStandStill) {
                        strongSelf.callbackWhenStandStill();
                    }
                }
            }
            strongSelf.countTime = 0;
        }
    });
}

```





