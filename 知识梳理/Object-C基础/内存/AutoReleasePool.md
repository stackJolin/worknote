## AutoReleasePool

#### 目录

- autorelease的本质
- autorelease对象什么时候释放
- autoreleasepool的工作原理
- autoreleasepool的内部结构
- autoreleasepool的嵌套
- autoreleasepoolpage
- NSThread、NSRunloop、和NSAutoreleasePool三者之间的关系



#### autorelease的本质

----

- autorelease`本质`上就是`延迟`调用release方法
- MRC环境，通过调用`[obj autorelease]`来`延迟`内存的释放
- ARC环境，甚至可以`完全不知道`autorelease也能管理好内存

#### 相关资料

---



