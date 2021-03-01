## YYCache源码解析

-----------

主要的类：

- YYCache
- YYMemoryCache
- YYDiskCache
- YYKVStorage



#### YYMemoryCache

-------------

```objective-c
@interface _YYLinkedMapNode : NSObject {
    @package
    // 上一个节点，这个并没有强引用
    __unsafe_unretained _YYLinkedMapNode *_prev; // retained by dic
    // 下一个节点，这里并没有强引用
    __unsafe_unretained _YYLinkedMapNode *_next; // retained by dic
    id _key;
    id _value;
    // 节点的内存开销
    NSUInteger _cost;
    // 操作节点的时间
    NSTimeInterval _time;
}

@interface _YYLinkedMap : NSObject {
    @package
    // NSDictionary的key需要遵循NSCoding协议, 而CFMutableDictionaryRef则不需要, 另外CFMutableDictionaryRef更加靠近底层, 效率更高, 但是创建的_dic的内存需要我们自己手动回收.
    CFMutableDictionaryRef _dic; // do not set object directly
    NSUInteger _totalCost;
    NSUInteger _totalCount;
    _YYLinkedMapNode *_head; // MRU, do not change it directly
    _YYLinkedMapNode *_tail; // LRU, do not change it directly
    BOOL _releaseOnMainThread;
    BOOL _releaseAsynchronously;
}
```





#### YYDiskCache

---------

内部是通过`YYKVStorate`实现的



#### YYKVStorage

------------

```objective-c
typedef NS_ENUM(NSUInteger, YYKVStorageType) {
    // 文件读写
    YYKVStorageTypeFile = 0,
    // 数据库读写
    YYKVStorageTypeSQLite = 1,
    // 根据策略决定使用文件还是数据库读写数据
    YYKVStorageTypeMixed = 2,
};
```







#### YYCache主要技术点

----

- 内存缓存(YYMemoryCache)
  - 思想： hashMap+双向链表
    - hasMap用于高速读取
    - 双向链表用于维护缓存表(基于LRU算法)
- 磁盘缓存(YYDiskCache)
  - 文件缓存
  - SQLite3数据库

#### 类

------

###### 内存缓存

- YYMemoryCache
- _YYLinkedMap
- _YYLinkedMapNode

###### 磁盘缓存