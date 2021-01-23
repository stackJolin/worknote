## NSLock

NSLock是对pthread_mutex的封装，继承自NSLocking协议

```object-c
@protocol NSLocking

- (void)lock;
- (void)unlock;

@end

@interface NSLock : NSObject <NSLocking> {
@private
    void *_priv;
}

- (BOOL)tryLock;
- (BOOL)lockBeforeDate:(NSDate *)limit;

@property (nullable, copy) NSString *name API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@end
```

可以看到，NSLock除了lock和unlock方法之外，还有tryLock和lockBefoeDate的方法：

- tryLock

  ```
  tryLock 并不会阻塞线程，[cjlock tryLock] 能加锁返回 YES，不能加锁返回 NO，然后都会执行后续代码。
  这里顺便提一下 trylock 和 lock 使用场景：当前线程锁失败，也可以继续其它任务，用 trylock 合适；当前线程只有锁成功后，才会做一些有意义的工作，那就 lock，没必要轮询 trylock
  ```

- lockBeforeDate

  ```swift
  方法会在所指定 Date 之前尝试加锁，会阻塞线程，如果在指定时间之前都不能加锁，则返回 NO，指定时间之前能加锁，则返回 YES。
  ```



#### 源码分析(Swift版)

---

```swift
open func lock() {
    pthread_mutex_lock(mutex)
}

open func unlock() 



    pthread_mutex_unlock(mutex)
#if os(macOS) || os(iOS)
    // Wakeup any threads waiting in lock(before:)
    pthread_mutex_lock(timeoutMutex)
    pthread_cond_broadcast(timeoutCond)
    pthread_mutex_unlock(timeoutMutex)
#endif
}

// 对应OC中的 -tryLock
open func `try`() -> Bool {
    return pthread_mutex_trylock(mutex) == 0
}

// 对应OC中的 -lockBeforeDate:
open func lock(before limit: Date) -> Bool {
    if pthread_mutex_trylock(mutex) == 0 {
        return true
    }

    return timedLock(mutex: mutex, endTime: limit, using: timeoutCond, with: timeoutMutex)
}

private func timedLock(mutex: _MutexPointer, endTime: Date,
                       using timeoutCond: _ConditionVariablePointer,
                       with timeoutMutex: _MutexPointer) -> Bool {
    var timeSpec = timeSpecFrom(date: endTime)
    while var ts = timeSpec {
        let lockval = pthread_mutex_lock(timeoutMutex)
        precondition(lockval == 0)
        let waitval = pthread_cond_timedwait(timeoutCond, timeoutMutex, &ts)
        precondition(waitval == 0 || waitval == ETIMEDOUT)
        let unlockval = pthread_mutex_unlock(timeoutMutex)
        precondition(unlockval == 0)

        if waitval == ETIMEDOUT {
            return false
        }
        let tryval = pthread_mutex_trylock(mutex)
        precondition(tryval == 0 || tryval == EBUSY)
        if tryval == 0 { // The lock was obtained.
            return true
        }
        // pthread_cond_timedwait didn't timeout so wait some more.
        timeSpec = timeSpecFrom(date: endTime)
    }
    return false
}
```



#### 注意的问题

----

不要在递归的场景下使用NSLock

```swift
NSLock * lock = [[NSLock alloc] init];
dispatch_async(dispatch_get_global_queue(0, 0), ^{
    static void(^testMethod)(int);
    testMethod = ^(int value){
        [lock lock];
        if (value >0) {
            NSLog(@"current value = %d",value);
            testMethod(value-1);
        }
        [lock unlock];
    };
    testMethod(10);
});
```



#### 相关文章

----

- https://xuyunan.github.io/posts/ios-lock-semaphore/

