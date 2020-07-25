## weak和assign的区别

我们先来讨论一个问题，assgin关键字修饰的属性能否是`对象类型`?

```objective-c
@property(nonatomic , assign) NSObject *obj1;
```

编译不会报错，说明可以修饰，但是会带来什么问题呢？

野指针！出现野指针的时候，往往会出现下面三种情形：

- 该空间一直空闲，调用该属性会引发EXC_BAD_ACCESS；
- 该空间被其他对象申请并占用，当调用该野指针的方法`methodA`时，这个空间上被分配的新对象并没有方法`methodA`， 会出现方法找不到：`[xxx methodA] unrecognized selector sent to instance 0x1xxxxxxxx`;
- 该空间被其他对象申请并占用，当调用该野指针的方法`methodA`时，这个空间上被分配的新对象刚好也包含方法`methodA`， 调用之后不会出现任何crash;

出现了这么多问题，也就是说assign不能用来修饰对象了吧？那么和weak对比又有什么意义吗？毕竟weak只能修饰object类型

```objective-c
@property(nonatomic , weak) NSInteger adfd;
// 报编译错误`Property with 'weak' attribute must be of object type`
```

其实，对比他们两个主要是基于2点：

- 二者都可以修饰对象
- 二者都不会引起object的引用计数发生变化

weak相对于assign不同的是，weak在全局维护了一张弱引用表(记住，这是object-c的机制。Swift则是维护在class内部，性能更高)。每当，对象的引用计数为0 的时候，都会去检查弱引用，将他们置为nil，这样就不会有也指着错误