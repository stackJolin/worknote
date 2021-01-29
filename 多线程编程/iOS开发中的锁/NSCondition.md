## NSCondition

-----

NSCondition是对`pthread_cond`的封装，属于`条件锁`。主要用于解决`生产-消费`问题

```objective-c
@interface NSCondition : NSObject <NSLocking> {
@private
    void *_priv;
}

- (void)wait;
- (BOOL)waitUntilDate:(NSDate *)limit;
- (void)signal; // 通知一个正在wait的线程，这个根据线程的优先级和等待的时间长短判断
- (void)broadcast; // 通知所有正在wait的线程

@property (nullable, copy) NSString *name API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@end
```





#### 基本使用

-------

```objective-c
- (void)add {
    [condition lock];
    [products addObject:[[NSObject alloc] init]];
    [condition signal];
    [condition unlock];
}

- (void)remove {
    [condition lock];
    while ([products count] == 0) {
        [condition wait];
    }
    [products removeObjectAtIndex:0];
    [condition unlock];
}
```

