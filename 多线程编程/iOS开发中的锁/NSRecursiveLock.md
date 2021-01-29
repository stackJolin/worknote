## NSRecursiveLock

--------

当我们在递归的场景下使用NSLock的时候：

```swift
NSLock *rLock = [NSLock new];
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    static void (^RecursiveBlock)(int);
    RecursiveBlock = ^(int value) {
        [rLock lock];
        if (value > 0) {
            NSLog(@"线程%d", value);
            RecursiveBlock(value - 1);
        }
        [rLock unlock];
    };
    RecursiveBlock(4);
});
```

NSLock作为一个互斥锁，只能被同一个线程请求一次，也就是lock后面必须是unlock，而不能是lock。那上面的问题怎么解决呢？使用NSRecursiveLock。NSRecursiveLock可以被同一个线程多次请求而不会引起死锁

NSReccursiveLock是对pthread_mutex递归锁的封装:

```swift
NSRecursiveLock *recursiveLock = [[NSRecursiveLock alloc] init];
dispatch_async(dispatch_get_global_queue(0, 0), ^{
    static void (^testMethod)(int);
    testMethod = ^(int value){
        [recursiveLock lock];
        if (value > 0) {
            NSLog(@"current value = %d",value);
            testMethod(value - 1);
        }
        [recursiveLock unlock];
    };
    testMethod(10);
});
```

NSRecursiveLock

```
@interface NSRecursiveLock : NSObject <NSLocking> {
@private
    void *_priv;
}

- (BOOL)tryLock;
- (BOOL)lockBeforeDate:(NSDate *)limit;

@property (nullable, copy) NSString *name API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@end
```



#### 源码分析

----

```swift
open class NSRecursiveLock: NSObject, NSLocking {
    internal var mutex = _RecursiveMutexPointer.allocate(capacity: 1)
#if os(macOS) || os(iOS) || os(Windows)
    private var timeoutCond = _ConditionVariablePointer.allocate(capacity: 1)
    private var timeoutMutex = _MutexPointer.allocate(capacity: 1)
#endif

    public override init() {
        super.init()
#if CYGWIN
        var attrib : pthread_mutexattr_t? = nil
#else
        var attrib = pthread_mutexattr_t()
#endif
        withUnsafeMutablePointer(to: &attrib) { attrs in
            pthread_mutexattr_init(attrs)
            // 设置 attrs 为 PTHREAD_MUTEX_RECURSIVE 类型
            pthread_mutexattr_settype(attrs, Int32(PTHREAD_MUTEX_RECURSIVE))
            // 如果是 NSLock，attrs 则为 nil
            pthread_mutex_init(mutex, attrs)
        }
#if os(macOS) || os(iOS)
        pthread_cond_init(timeoutCond, nil)
        pthread_mutex_init(timeoutMutex, nil)
#endif
}
```



#### 源码分析

---





#### 相关文章

----

- https://xuyunan.github.io/posts/ios-lock-semaphore/

