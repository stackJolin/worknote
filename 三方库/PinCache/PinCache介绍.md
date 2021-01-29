## PinCache介绍

------

`fork`自`TMCache`，并且进行了重构，解决了频繁使用`TMCache`时，导致的死锁问题。

基本上分为三个部分：

- `PinMemoryCahce` - 内存缓存
- `PinDiskCache` - 磁盘缓存
- `PinOperation` - 多线程并发编程



其中`PinMemoryCache`是完全基于`字典`实现，所以复杂度在O(1)，每次进入后台或者缓存都会自动清理。没有`LRU`或者`FRU`的机制

`PinDiskCache`则是完全基于写本地文件

