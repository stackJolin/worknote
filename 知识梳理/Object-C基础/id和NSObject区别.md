## id、instance、NSObject的区别和联系

---------



#### id

--------

`id`是`objective-C`对象，但是并不一定是`NSObject对象`。`id`和`NSObject`之间有一些共同的方法。比如`retain`和`release`

使用`id`，编译器不会做类型检查，比如我们声明delegate的时候，会使用`id`，真正调用的时候，内部去做类型判断

