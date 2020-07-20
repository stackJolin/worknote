## OSSpinLock

iOS10之后出现的，替代OSSpinLock，用于解决OSSpinLock优先级反转的问题

os_unfair_lock是一个相对高效的互斥锁，并非一个自选锁，使用时，需要导入头文件\#import< os/lock.h >



#### 使用方法

-----

```
os_unfair_lock_t unfairLock;  
unfairLock = &(OS_UNFAIR_LOCK_INIT);  
os_unfair_lock_lock(unfairLock);  
os_unfair_lock_unlock(unfairLock);
os_unfair_lock_trylock(unfairLock);
```

