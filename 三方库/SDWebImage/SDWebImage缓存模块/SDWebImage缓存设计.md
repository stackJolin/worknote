## SDWebImage缓存设计

-----------------------------



#### 内存缓存SDMemoryCache

-----

SDMemoryCache继承自NSCache。NSCache天然的缺点：收到内存警告，会优先释放NSCache。为了避免NSCache被回收，SDMemoryCache内部设计了另一个缓存，集体代码如下：

```
@property (nonatomic, strong, nonnull) NSMapTable<KeyType, ObjectType> *weakCache;
```



#### SDWebimage缓存相关协议

-----

- SDImageCache

  这个协议里面的方法都是required的，也就是必须要实现的

  

  ```objective-c
  @protocol SDImageCache <NSObject>
  ```

  

#### SDWebImage缓存相关类

-----

- SDImageCacheManager

  ```
  SDImageCacheManager是SDWebImage的缓存管理类。持有一个cache数组:
  NSMutableArray<id<SDImageCache>> *_imageCaches;
  ```

- SDImageCach

- e

  ```
  SDImageCache是一个协议，同时，也是一个类。协议，方便使用这自己实现一份Cache添加到SDImageCacheManager
  作为一个类的话，主要持有一下属性：
  @property (nonatomic, strong, readwrite, nonnull) id<SDMemoryCache> memoryCache;
  @property (nonatomic, strong, readwrite, nonnull) id<SDDiskCache> diskCache;
  ```

- SDMemoryCache

  ```
  SDWebImage的内存缓存，继承自NSCache。由于NSCache有一个天然缺陷：收到内存警告后，会优先清理NSCache。所以为了避免内存缓存被清理，SDMemoryCache额外维护了另一份cache
  @property (nonatomic, strong, nonnull) NSMapTable<KeyType, ObjectType> *weakCache;
  ```

- SDDiskCache

  ```
  SDWebImage的磁盘缓存管理类。使用NSFileManager存储文件，sys/attr.h来存储图片的额外信息
  ```

  