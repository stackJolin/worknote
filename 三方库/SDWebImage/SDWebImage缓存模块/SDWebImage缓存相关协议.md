## SDWebImage缓存相关协议和类

-------

相关协议：

- SDImageCache
- SDMemoryCache
- SDDiskCache

相关类(协议的默认实现)：

- SDImageCache
- SDMemoryCache
- SDDiskCache
- SDImageCachesManager

下面分别介绍



#### SDImageCache

-------

```objective-c
@protocol SDImageCache <NSObject>

@required
- (nullable id<SDWebImageOperation>)queryImageForKey:(nullable NSString *)key
                                             options:(SDWebImageOptions)options
                                             context:(nullable SDWebImageContext *)context
                                          completion:(nullable SDImageCacheQueryCompletionBlock)completionBlock;
                                          
- (nullable id<SDWebImageOperation>)queryImageForKey:(nullable NSString *)key
                                             options:(SDWebImageOptions)options
                                             context:(nullable SDWebImageContext *)context
                                           cacheType:(SDImageCacheType)cacheType
                                          completion:(nullable SDImageCacheQueryCompletionBlock)completionBlock;

- (void)storeImage:(nullable UIImage *)image
         imageData:(nullable NSData *)imageData
            forKey:(nullable NSString *)key
         cacheType:(SDImageCacheType)cacheType
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

- (void)removeImageForKey:(nullable NSString *)key
                cacheType:(SDImageCacheType)cacheType
               completion:(nullable SDWebImageNoParamsBlock)completionBlock;

- (void)containsImageForKey:(nullable NSString *)key
                  cacheType:(SDImageCacheType)cacheType
                 completion:(nullable SDImageCacheContainsCompletionBlock)completionBlock;

- (void)clearWithCacheType:(SDImageCacheType)cacheType
                completion:(nullable SDWebImageNoParamsBlock)completionBlock;

```

三个功能：

- 增

- 删

- 查

  



#### SDMemoryCache

------

```objective-c
@protocol SDMemoryCache <NSObject>

@required

- (nonnull instancetype)initWithConfig:(nonnull SDImageCacheConfig *)config;

- (nullable id)objectForKey:(nonnull id)key;

- (void)setObject:(nullable id)object forKey:(nonnull id)key;

- (void)setObject:(nullable id)object forKey:(nonnull id)key cost:(NSUInteger)cost;

- (void)removeObjectForKey:(nonnull id)key;

- (void)removeAllObjects;

@end
```

三个功能：

- 增

- 删

- 查

  

#### SDDiskCache

----------

```objective-c
@protocol SDDiskCache <NSObject>

@required

- (nullable instancetype)initWithCachePath:(nonnull NSString *)cachePath config:(nonnull SDImageCacheConfig *)config;

- (BOOL)containsDataForKey:(nonnull NSString *)key;

- (nullable NSData *)dataForKey:(nonnull NSString *)key;

- (void)setData:(nullable NSData *)data forKey:(nonnull NSString *)key;

- (nullable NSData *)extendedDataForKey:(nonnull NSString *)key;

- (void)setExtendedData:(nullable NSData *)extendedData forKey:(nonnull NSString *)key;

- (void)removeDataForKey:(nonnull NSString *)key;

- (void)removeAllData;

- (void)removeExpiredData;

- (nullable NSString *)cachePathForKey:(nonnull NSString *)key;

- (NSUInteger)totalCount;

- (NSUInteger)totalSize;

@end
```

