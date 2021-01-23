## OSSpinLock

自旋锁的一种，由于在某些场景下不安全已经被弃用。需要导入头文件\#import <libkern/OSAtomic.h>



#### 使用

------

```
OSSpinLock lock = OS_SPINLOCK_INIT;// 默认值为0，在locked状态时大于0，unlock为0
OSSpinLockLock(&lock);  // 加锁
OSSpinLockUnlock(&lock); // 解锁
OSSpinLockTry(&lock); // 尝试加锁，可以加锁则立即加锁并返回 YES,反之返回 NO 
```

注：OSSpinLockTry用于解决优先级反转的问题



#### 自旋锁存在优先级反转问题

----

当一个低优先级线程获得锁的时候，如果此时一个高优先级的线程到来，那么会进入忙等状态，不会进入睡眠，此时会一直占用着系统CPU时间，导致低优先级的无法拿到CPU时间片，从而无法完成任务也无法释放锁。除非能保证访问锁的线程全部处于同一优先级，否则系统所有的自旋锁都会出现优先级反转的问题



#### OSSpinLock的自旋锁的替代方案os_unfair_lock

-----

见：os_unfair_lock