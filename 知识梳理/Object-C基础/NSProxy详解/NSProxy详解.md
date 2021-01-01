## NSProxy详解

首先说，NSProxy和NSObject的Level是一样的，都是一个基础类，也是继承自<NSObject>协议。只不过，NSProxy是一个抽象类，没有具体实现，用法也是只能通过继承NSProxy，然后实现子类其子类。那么这个有什么用处呢？来看一下Apple官方的介绍：

>NSProxy is an abstract superclass defining an API for objects that act as stand-ins for other objects or for objects that don’t exist yet. Typically, a message to a proxy is forwarded to the real object or causes the proxy to load (or transform itself into) the real object. Subclasses of NSProxy can be used to implement transparent distributed messaging (for example, [NSDistantObject](https://link.jianshu.com?t=https://developer.apple.com/library/mac/documentation/Cocoa/Reference/Foundation/Classes/NSDistantObject_Class/index.html#//apple_ref/occ/cl/NSDistantObject)) or for lazy instantiation of objects that are expensive to create.

总的来说，NSProxy是一个虚类，你可以通过继承它，并重写这两个方法以实现消息转发到另一个实例

```
- (void)forwardInvocation:(NSInvocation *)anInvocation;
- (NSMethodSignature *)methodSignatureForSelector:(SEL)sel;
```



#### 主要应用

------

