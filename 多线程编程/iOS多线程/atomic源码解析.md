## atomic源码解析

--------

使用atomic修饰的属性的setter方法和getter方法的源码如下：

```c++
// settter
static inline void reallySetProperty(id self, SEL _cmd, id newValue, ptrdiff_t offset, bool atomic, bool copy, bool mutableCopy)
{
    if (offset == 0) {
        object_setClass(self, newValue);
        return;
    }

    id oldValue;
    id *slot = (id*) ((char*)self + offset);

    if (copy) {
        newValue = [newValue copyWithZone:nil];
    } else if (mutableCopy) {
        newValue = [newValue mutableCopyWithZone:nil];
    } else {
        if (*slot == newValue) return;
        newValue = objc_retain(newValue);
    }

    if (!atomic) {
        oldValue = *slot;
        *slot = newValue;
    } else {
        spinlock_t& slotlock = PropertyLocks[slot];
        slotlock.lock();
        oldValue = *slot;
        *slot = newValue;        
        slotlock.unlock();
    }

    objc_release(oldValue);
}

// getter
id objc_getProperty(id self, SEL _cmd, ptrdiff_t offset, BOOL atomic) {
    if (offset == 0) {
        return object_getClass(self);
    }

    // Retain release world
    id *slot = (id*) ((char*)self + offset);
    if (!atomic) return *slot;
        
    // Atomic retain release world
    spinlock_t& slotlock = PropertyLocks[slot];
    slotlock.lock();
    id value = objc_retain(*slot);
    slotlock.unlock();
    
    // for performance, we (safely) issue the autorelease OUTSIDE of the spinlock.
    return objc_autoreleaseReturnValue(value);
}
```

可以看到，atomic使用的是spinlock_t实现的，spinlock在iOS10之后的实现改为了`os_unfair_lock`，`os_unfair_lock`并不是一个自旋锁，而是一个互斥锁，但是性能很高



#### atomic为什么不是线程安全的

-----------------

举个例子：

```c++
- (void)competition {
		self.intSource = 0;

		dispatch_async(queue1, ^{
				for (int i = 0; i < 10000; i++) {
						self.intSource = self.intSource + 1;
				}
		});

		dispatch_async(queue2, ^{
				for (int i = 0; i < 10000; i++) {
						self.intSource = self.intSource + 1;
				}
		});
}
```

最终得到的结果肯定小于20000。当获取值的时候都是原子线程安全操作，比如两个线程依序获取了当前值 0，于是分别增量后变为了 1，所以两个队列依序写入值都是 1，所以不是线程安全的。

解决的办法应该是增加颗粒度，将读写两个操作合并为一个原子操作，从而解决写入过期数据的问题。

```objective-c
os_unfair_lock_t unfairLock;
- (void)competition {
		self.intSource = 0;
		unfairLock = &(OS_UNFAIR_LOCK_INIT);
		dispatch_async(queue1, ^{
				for (int i = 0; i < 10000; i++) {
						os_unfair_lock_lock(unfairLock);
						self.intSource = self.intSource + 1;
						os_unfair_lock_unlock(unfairLock);
				}
		});

		dispatch_async(queue2, ^{
				for (int i = 0; i < 10000; i++) {
						os_unfair_lock_lock(unfairLock);
						self.intSource = self.intSource + 1;
						os_unfair_lock_unlock(unfairLock);
				}
		});
}
```





#### OSSpinLock的优先级反转问题

-------









