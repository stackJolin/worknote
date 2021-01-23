## NSCache-OC源码详解

源码:

- NSCache.h：https://github.com/gnustep/libs-base/tree/master/Headers/Foundation
- NSCache.m：https://github.com/gnustep/libs-base/blob/master/Source/NSCache.m



NSCache.h

```objective-c
@class NSString;
@class NSMapTable;
@class GS_GENERIC_CLASS(NSMutableArray, ElementT);

@interface GS_GENERIC_CLASS(NSCache, KeyT, ValT) : NSObject
{
  // 最大缓存大小
  NSUInteger _costLimit;
  
  // 当前总的缓存大小
  NSUInteger _totalCost;
  
  // cache中的最大缓存数量大小
  NSUInteger _countLimit;
  
  // 代理 - 缓存对象被移除的时候，会发送推送
  id _delegate;
  
  // 是否应该回收废弃内容，默认是YES
  BOOL _evictsObjectsWithDiscardedContent;
  
  // NSCache实例的名称
  NSString *_name;
  
  // `名称`到`缓存`对象的映射
  NSMapTable *_objects;
  
  // NSMutableArray类型的变量
  GS_GENERIC_CLASS(NSMutableArray, ValT) *_accesses;

  int64_t _totalAccesses;
}

- (GS_GENERIC_TYPE(ValT)) objectForKey:
    (GS_GENERIC_TYPE(KeyT))key;

- (void) removeAllObjects;

- (void) removeObjectForKey: (GS_GENERIC_TYPE(KeyT))key;

- (void) setObject: (GS_GENERIC_TYPE(ValT))obj
            forKey: (GS_GENERIC_TYPE(KeyT))key
              cost: (NSUInteger)num;

- (void) setObject: (GS_GENERIC_TYPE(ValT))obj
            forKey: (GS_GENERIC_TYPE(KeyT))key;

- (void) setTotalCostLimit: (NSUInteger)lim;
@end

@protocol NSCacheDelegate
- (void) cache: (NSCache*)cache willEvictObject: (id)obj;
@end
```



NSCache.m

```objective-c
@interface _GSCachedObject : NSObject
{
  @public
  id object;
  NSString *key;
  int accessCount;
  NSUInteger cost;
  BOOL isEvictable;
}
@end

@implementation NSCache
- (id) init {
  if (nil == (self = [super init])) {
      return nil;
  }
  
  ASSIGN(_objects,[NSMapTable strongToStrongObjectsMapTable]);
  
  _accesses = [NSMutableArray new];
  return self;
}

- (id)objectForKey:(id)key {
  // 获取key对应的obj
  _GSCachedObject *obj = [_objects objectForKey: key];

  // 如果obj不存在直接返回
  if (nil == obj) { return nil; }
  
  // 如果obj是可驱逐的，那么把obj放到_acccess的尾部
  if (obj->isEvictable) {
      [_accesses removeObjectIdenticalTo: obj];
      [_accesses addObject: obj];
  }
  
  // obj被访问的次数+1
  obj->accessCount++;
  
  // 总共被访问的次数+1
  _totalAccesses++;
  
  // 返回obj的value
  return obj->object;
}

- (void) removeAllObjects {
  NSEnumerator *e = [_objects objectEnumerator];
  _GSCachedObject *obj;
  
  while (nil != (obj = [e nextObject]))
    {
      [_delegate cache: self willEvictObject: obj->object];
    }
  
  // 清理_objects(NSMapTable)
  [_objects removeAllObjects];
  // 清理_accesses(NSMutableArray)
  [_accesses removeAllObjects];
  // 重置总访问数
  _totalAccesses = 0;
}

// 移除某个元素
- (void) removeObjectForKey: (id)key {
  // 获取key对应的元素
  _GSCachedObject *obj = [_objects objectForKey: key];

  // 获取元素存在
  if (nil != obj) {
      [_delegate cache: self willEvictObject: obj->object];
      
      // 计算总访问数
      _totalAccesses -= obj->accessCount;
      
      // 移除元素
      [_objects removeObjectForKey: key];
      [_accesses removeObjectIdenticalTo: obj];
  }
}

- (void) setObject: (id)obj forKey: (id)key {
  [self setObject: obj forKey: key cost: 0];
}

- (void)setObject:(id)obj forKey:(id)key cost:(NSUInteger)num {
  _GSCachedObject *oldObject = [_objects objectForKey: key];
  _GSCachedObject *newObject;

  // 如果老的元素不存在的话，先移除
  if (nil != oldObject) {
      [self removeObjectForKey: oldObject->key];
  }
  
  // 淘汰缓存
  [self _evictObjectsToMakeSpaceForObjectWithCost: num];
  
  // 新建元素变赋值
  newObject = [_GSCachedObject new];
  // Retained here, released when obj is dealloc'd
  newObject->object = RETAIN(obj);
  newObject->key = RETAIN(key);
  newObject->cost = num;
  
  // 如果被插入的元素，实现了NSDiscardableContent
  if ([obj conformsToProtocol: @protocol(NSDiscardableContent)]) {
      newObject->isEvictable = YES;
      [_accesses addObject: newObject];
  }
  
  [_objects setObject:newObject forKey:key];
  RELEASE(newObject);
  _totalCost += num;
}

- (void)_evictObjectsToMakeSpaceForObjectWithCost:(NSUInteger)cost {
  
    // 需要清理的空间大小
    NSUInteger spaceNeeded = 0;
    NSUInteger count = [_objects count];
  
    // 计算需要清理的空间大小
    if (_costLimit > 0 && _totalCost + cost > _costLimit) {
        spaceNeeded = _totalCost + cost - _costLimit;
    }

    // Only evict if we need the space.
    if (count > 0 && (spaceNeeded > 0 || count >= _countLimit)) {
        NSMutableArray *evictedKeys = nil;
        // Round up slightly.
        
        // 粗略的计算了一下平均访问次数，贼傻逼
        NSUInteger averageAccesses = ((_totalAccesses / (double)count) * 0.2) + 1;
        
        NSEnumerator *e = [_accesses objectEnumerator];
        _GSCachedObject *obj;
        
        if (_evictsObjectsWithDiscardedContent) {
            evictedKeys = [[NSMutableArray alloc] init];
        }
        
        while (nil != (obj = [e nextObject])) {
            // 如果obj的访问次数小于平均访问次数 && obj是可驱逐的
            if (obj->accessCount < averageAccesses && obj->isEvictable) {
                [obj->object discardContentIfPossible];
                
                if ([obj->object isContentDiscarded]) {
                    NSUInteger cost = obj->cost;
                    // Evicted objects have no cost.
                    obj->cost = 0;
                    // Don't try evicting this again in future; it's gone already.
                    obj->isEvictable = NO;
                    // Remove this object as well as its contents if required
                    if (_evictsObjectsWithDiscardedContent) {
                        [evictedKeys addObject: obj->key];
                    }
                    
                    _totalCost -= cost;
                    // If we've freed enough space, give up
                    if (cost > spaceNeeded) { break; }
                    
                    spaceNeeded -= cost;
                }
            }
        }
        
        // Evict all of the objects whose content we have discarded if required
        if (_evictsObjectsWithDiscardedContent) {
            NSString *key;
            e = [evictedKeys objectEnumerator];
            while (nil != (key = [e nextObject])) {
                [self removeObjectForKey: key];
            }
        }
        
        [evictedKeys release];
    }
}

- (void) dealloc
{
  [_name release];
  [_objects release];
  [_accesses release];
  [super dealloc];
}
@end
```





#### 相关类和协议

----

- _GSCachedObject

  ```
  @interface _GSCachedObject : NSObject
  {
    @public
    id object;
    NSString *key;
    int accessCount;
    NSUInteger cost;
    BOOL isEvictable;
  }
  @end
  
  @implementation _GSCachedObject
  - (void)dealloc {
    [object release];
    [key release];
    [super dealloc];
  }
  @end
  ```

- NSDiscardableContent

  NSDiscardableContent是一个协议，实现这个协议的目的是为了让我们的对象在不被使用时，可以将其丢弃，以让程序占用更少的内存。一个NSDiscardableContent对象的生命周期依赖于一个“counter”变量。一个NSDiscardableContent对象实际是一个可清理内存块，这个内存记录了对象当前是否被其它对象使用。如果这块内存正在被读取，或者仍然被需要，则它的counter变量是大于或等于1的；当它不再被使用时，就可以丢弃，此时counter变量将等于0。当counter变量等于0时，如果当前时间点内存比较紧张的话，内存块就可能被丢弃。这点类似于MRC&ARC，对象内存回收机制。

  ```
  @protocol NSDiscardableContent
  @required
  - (BOOL)beginContentAccess;
  - (void)endContentAccess;
  - (void)discardContentIfPossible;
  - (BOOL)isContentDiscarded;
  @end
  ```

#### 相关文章

----

- https://juejin.im/post/5d85dfe7e51d4561f777e28d
- Matt:https://nshipster.cn/nscache/