## MRC和ARC



#### ARC下strong/weak和MRC下retain/assign

------

ARC下的变量修饰符strong和weak分别对应MRC下的retain/assign。ARC模式下的property变量修饰词为strong、weak，相当于MRC模式下的retain、assign。

strong :代替retain，缺省关键词，代表强引用。

weak：代替assign，声明了一个可以自动设置nil的弱引用，但是比assign多一个功能，指针指向的地址被释放之后，指针本身也会自动被释放



#### MRC和ARC混编

-----

在ARC的项目中，对MRC的文件可以添加编译选项`-fno-objc-arc`的标识；

在MRC的项目中，对ARC的文件可以添加编译选项`-fobjc-arc`的标识。



#### 与内存相关的属性修饰符

-----

