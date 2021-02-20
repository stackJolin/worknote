## SDMemoryCache的默认实现 - SDWebImage的亮点

-----------

```objective-c
@interface SDMemoryCache <KeyType, ObjectType> : NSCache <KeyType, ObjectType> <SDMemoryCache>

@property (nonatomic, strong, nonnull, readonly) SDImageCacheConfig *config;

@end
```

内部就是将 **NSCache** 扩展为了 SDMemoryCache 协议，并加入了 **NSMapTable<KeyType, ObjectType> \*weakCache** ，并为其添加了信号量锁来保证线程安全。这里的 weak-cache 是仅在 *iOS/tvOS* 平台添加的特性，因为在 macOS 上尽管收到系统内存警告，NSCache 也不会清理对应的缓存。weakCache 使用的是 strong-weak 引用不会有有额外的内存开销且不影响对象的生命周期。

weakCache 的作用在于恢复缓存，它通过 CacheConfig 的 **shouldUseWeakMemoryCache** 开关以控制，详细说明可以查看 [CacheConfig.h](https://github.com/SDWebImage/SDWebImage/blob/master/SDWebImage/Core/SDImageCacheConfig.h)。先看看其如何实现的：

```objective-c
- (id)objectForKey:(id)key {
    id obj = [super objectForKey:key];
    if (!self.config.shouldUseWeakMemoryCache) {
        return obj;
    }
    if (key && !obj) {
        // Check weak cache
        SD_LOCK(self.weakCacheLock);
        obj = [self.weakCache objectForKey:key];
        SD_UNLOCK(self.weakCacheLock);
        if (obj) {
            // Sync cache
            NSUInteger cost = 0;
            if ([obj isKindOfClass:[UIImage class]]) {
                cost = [(UIImage *)obj sd_memoryCost];
            }
            [super setObject:obj forKey:key cost:cost];
        }
    }
    return obj;
}
```

由于 NSCache 遵循  `NSDiscardableContent`  策略来存储临时对象的，当内存紧张时，缓存对象有可能被系统清理掉。此时，如果应用访问 MemoryCache 时，缓存一旦未命中，则会转入 diskCache 的查询操作，可能导致 image 闪烁现象。而当开启 shouldUseWeakMemoryCache 时，因为 weakCache 保存着对象的弱引用 （在对象 被 NSCache 被清理且没有被释放的情况下)，我们可通过 weakCache 取到缓存，将其塞会 NSCache 中。从而减少磁盘 I/O。









