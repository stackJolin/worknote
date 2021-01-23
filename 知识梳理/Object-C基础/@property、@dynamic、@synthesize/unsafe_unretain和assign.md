## unsafe_unretain和assign

------

用法上是一样的，都可以修饰指针类型和值类型。但是官方建议修饰指针类型。那么两个有什么区别呢？

`unsafe_unretain`是`iOS4`之后，为了方便兼容而存在。而`assign`不是，具体见下面回答：

https://stackoverflow.com/questions/14669463/unsafe-unretained-and-weak-and-assign-under-arc/14669490#14669490

