## SDImageCacheConfig详解

-------

```objective-c
@interface SDImageCacheConfig : NSObject <NSCopying>
// 单例。默认设置
@property (nonatomic, class, readonly, nonnull) SDImageCacheConfig *defaultCacheConfig;

// 是否关闭icloud
@property (assign, nonatomic) BOOL shouldDisableiCloud;

// 是否开启内存缓存，默认是YES
@property (assign, nonatomic) BOOL shouldCacheImagesInMemory;

// 是否开启，内存缓存弱引用。默认是开启的
// 如果开启后，当前缓存除了会存储到缓存中，还会存储在一个`strongKey-weakValue`的`NSMapTable`中
// 如果，发生了内存警告，由于maptable并不会持有image的强引用。所以并不会影响image的正常的生命周期。
// 如果NSCache移除了图片的引用，但是这个图片依旧被imageView强引用的情况下，当下一次从NSCache通过该图片对应的key获取该图片的时候，可以从maptable获取而不用从磁盘或者网络从新获取
@property (assign, nonatomic) BOOL shouldUseWeakMemoryCache;

// 进入后台后，是否移除本地磁盘上的过期资源
@property (assign, nonatomic) BOOL shouldRemoveExpiredDataWhenEnterBackground;

/**
 * The reading options while reading cache from disk.
 * Defaults to 0. You can set this to `NSDataReadingMappedIfSafe` to improve performance.
 */
@property (assign, nonatomic) NSDataReadingOptions diskCacheReadingOptions;

/**
 * The writing options while writing cache to disk.
 * Defaults to `NSDataWritingAtomic`. You can set this to `NSDataWritingWithoutOverwriting` to prevent overwriting an existing file.
 */
@property (assign, nonatomic) NSDataWritingOptions diskCacheWritingOptions;

// 磁盘缓存最大时长，默认是1周
@property (assign, nonatomic) NSTimeInterval maxDiskAge;

// 磁盘缓存大小限制。默认是0，也就是没有限制
@property (assign, nonatomic) NSUInteger maxDiskSize;

// 内存缓存大小限制。默认是0，也就是没有限制
@property (assign, nonatomic) NSUInteger maxMemoryCost;

// 内存缓存数量限制。默认是0，也就是没有限制
@property (assign, nonatomic) NSUInteger maxMemoryCount;

/*
 * The attribute which the clear cache will be checked against when clearing the disk cache
 * Default is Modified Date
 */
@property (assign, nonatomic) SDImageCacheConfigExpireType diskCacheExpireType;

/**
 * The custom file manager for disk cache. Pass nil to let disk cache choose the proper file manager.
 * Defaults to nil.
 * @note This value does not support dynamic changes. Which means further modification on this value after cache initialized has no effect.
 * @note Since `NSFileManager` does not support `NSCopying`. We just pass this by reference during copying. So it's not recommend to set this value on `defaultCacheConfig`.
 */
@property (strong, nonatomic, nullable) NSFileManager *fileManager;

// 自定义的内存缓存类，这个类必须遵循`SDMemoryCache`协议。内置的实现是`SDMemoryCache`类
@property (assign, nonatomic, nonnull) Class memoryCacheClass;

// 自定义的磁盘缓存类，这个类必须遵循`SDDiskCache`协议。内置的实现是`SDDiskCache`类
@property (assign ,nonatomic, nonnull) Class diskCacheClass;
```

