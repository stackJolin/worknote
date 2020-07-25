## NSNotification的同步/异步/线程安全

#### 相关方法和使用

-----

```swift
@interface NSNotificationCenter : NSObject {
    @package
    void *_impl;
    void *_callback;
    void *_pad[11];
}

@property (class, readonly, strong) NSNotificationCenter *defaultCenter;

- (void)addObserver:(id)observer selector:(SEL)aSelector name:(nullable NSNotificationName)aName object:(nullable id)anObject;

- (void)postNotification:(NSNotification *)notification;
- (void)postNotificationName:(NSNotificationName)aName object:(nullable id)anObject;
- (void)postNotificationName:(NSNotificationName)aName object:(nullable id)anObject userInfo:(nullable NSDictionary *)aUserInfo;

- (void)removeObserver:(id)observer;
- (void)removeObserver:(id)observer name:(nullable NSNotificationName)aName object:(nullable id)anObject;

- (id <NSObject>)addObserverForName:(nullable NSNotificationName)name object:(nullable id)obj queue:(nullable NSOperationQueue *)queue usingBlock:(void (^)(NSNotification *note))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
    // The return value is retained by the system, and should be held onto by the caller in
    // order to remove the observer with removeObserver: later, to stop observation.

@end
```



#### NSNotification需要注意的问题

-----

**问题1：在通知中心注册过的对象，必须在该对象释放前取消注册。否则，当相应的通知再次出现时，通知中心仍然会向该监听器发送消息。因为相应的监听器对象已经被释放了，所以可能会导致应用崩溃**

```c++
- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
```

**问题2：一定要先向通知中心注册通知监听器，也就是谁要监听谁发布的消息，然后再执行发布消息，不然会导致消息发出来了没人接收的情况**



**问题3：通知有可能在非主线程发送，所以接收到推送的是在非主线程，所以要注意多线程的问题**



**问题4：block接受通知的方式，当观察者被移除后，block中的代码依旧会打印**

```c++
id observer = [[NSNotificationCenter defaultCenter] addObserverForName:@"block" object:nil queue:nil usingBlock:^(NSNotification * _Nonnull note) {
    NSLog(@"block方式受到系统通知");
}];
首先点击发送通知，控制台会打印block方式受到系统通，然后点击注销通知，再点击发送通知，依然会打印；如果退出当前控制器，并且再进入一次控制器，点击发送通知，控制器会打印2次
首先，在使用block方式注册通知的时候，我们只是传了block，并没有传Observer监听者，NSNotificationCenter直接持有block
所以，需要在dealloc的时候将observer移除
- (void)dealloc {
    if (observer) {
        [[NSNotificationCenter defaultCenter] removeObserver:_observer];
        observer = nil;
    }
}

如果，block里面引用了self，那么需要加__weak
```

其实归根结底就两个问题：

- 线程问题：主要通知的发送和接受在同一个线程。这也是为什么NSNotification是线程安全的原因
- 释放问题；主要是移除监听者