## super和self的区别

-------

- self调用的是自己的方法，super调用的是父类方法
- super是预编译指令
- [self class]和[super class]是一样的





#### [self message]和[super message]的实现

----------

`[self message]`会转换成如下方法：

```c++
id objc_msgSend(id theReceiver, SEL theSelector, ...) 
```

`[super message]`会转换成如下方法：

```c++
id objc_msgSendSuper(struct objc_super *super, SEL op, ...) 
  
struct objc_super {
    __unsafe_unretained _Nonnull id receiver; // 消息接受者
    __unsafe_unretained _Nonnull Class super_class; // 父类
    /* super_class is the first class to search */
};

// 其实调用的是 _objc_msgSendSuper2
```

