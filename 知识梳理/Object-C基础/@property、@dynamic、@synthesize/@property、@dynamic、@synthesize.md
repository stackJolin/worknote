## @property、@dynamic、@synthesize

------



#### @synthesize和@property的历史

---------

第一版：在iOS6之前，也就是`LLVM`诞生之前

```objective-c
.h文件：
@interface ViewController ： UIViewController
{
//   属性的实例变量  
NSString *mytitle;
}
@property（copy,nonatomic） NSString *myTitile;
@end
  
.m文件
//编译器遇到@synthesize会自动实现setter和getter方法
//编译器遇到@synthesiz会去访问同名的myTitle，如果没有找到就会报错。
@synthesize myTitle
```



第二版：不需要手动声明成员变量，关键字@synthesize会默认去访问myTitle的同名变量，如果找不到则会自动生成一个

```c++
.h文件
@property （nonatomic,strong） NSString *myTitle;

.m文件
//编译器遇到@synthesize会自动实现setter和getter方法，访问同名变量myTitle。如果找不到的话会自动创建一个私有同名变量myTitle，因此之后可以不用手动声明一个实例变量
@synthesize myTitle;
```



第三版：考虑到变量名和getter的方法名、setter的参数名一样，引起警告，所以默认给属性生成的同名变量带下划线

```c++
.m文件
//指定_myTitle作为myTitle的实例变量
@synthesize myTitle = _myTitle；
```



第四版： 随着ios升级，@synthesize也可以省略了，只用在.h文件中声明属性即可。声明属性的时候编译器自动生成的实例变量，实例变量的本质是成员变量，self.myTitle操作属性的时候实质上是在操作成员变量_myTitle(也就是实例变量)

```
.h文件：
@property （nonatomic，strong） NSString * myTitle;
```



#### @property和@synthesize的区别

------------

@synthesize的作用：

- 自动生成setter和getter方法
- 指定与属性对应的成员变量

@property的作用：

- 生成带下划线的成员变量
- 生成setter方法和getter方法

其实在最早的版本中，@property只能写在.h文件中，而@synthesize只能写在.m文件中。而现在@property都可以写了



#### @dynamic的作用

---------

`iOS6`之后，LLVM编译器引入了`property autosynthesis`，即`自动属性合成`。即编译器会自动为每个@property添加@synthesize，形式如下：

```c++
@synthesize propertyName = _propertyName;
```

如果要阻止自动合成，记得使用 @dynamic。经典的使用场景是你知道已经在某处实现了getter/setter 方法，而编译器不知道的情况。

> 此处我的理解是为了防止编译器使用自动合成生成新的setter／getter 会覆盖已经存在的旧的 setter／getter。

