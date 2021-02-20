## KVO（Key-Value-Observer）

#### 相关知识点

- NSKeyValueObserving.h

KVO是根据KVC来实现的



#### 如何手动触发KVO

----

```markdown
// 三个重要的方法
- (void)willChangeValueForKey:(NSString *)key;
- (void)didChangeValueForKey:(NSString *)key;
+ (BOOL)automaticallyNotifiesObserversForKey:(NSString *)key;
```



#### 原理分析

-------

在对`obj`执行`addObserver`的操作之后，会动态生成一个obj的子类对象，这个对象是`NSKVONotifyin_obj`。





#### 相关文章

-----

- KVO进阶1，2，3，4

  https://www.jianshu.com/p/306a21ee23ae

  https://www.jianshu.com/p/a8809c1eaecc

  https://www.jianshu.com/p/1cc8a5eedf35

  https://www.jianshu.com/p/a25488928a30

- https://juejin.im/post/5d19fa62f265da1ba91595bb

- https://juejin.cn/post/6844903593925935117

#### 相关应用

----

**willChangeValueForKey**

**didChangeValueForKey**

