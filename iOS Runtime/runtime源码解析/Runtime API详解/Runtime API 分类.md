## Runtime API分类

------

我们这里按照函数名前缀分类：

- objc_：主要关注宏观使用，如类与协议的创建，关联对象等
- class_：主要关注类的内部，如获取实例变量、属性方法等
- object_：主要关注的是对象
- method_：关注于方法的内部，如：方法的参数及返回值以及方法的实现
- property_：关注属性内部，如属性的特性
- protocol_：关注类所遵循的协议
- ivar_
- sel_
- Imp_

共9中，接下来我们介绍一下具体作用



#### objc_

-------------------

```c++
// 获取类对象
objc_getClass
// 获取元类对象
objc_getMetaClass
// 分配空间，创建类。仅在创建之后，注册之前，才能够添加成员变量
objc_allocateClassPair
// 注册对象。创建类之后，需要注册该类，才能够使用该类创建对象
objc_registerClassPair
// 注销某个类
objc_disposeClassPair 注销某个类
// 开辟空间创建协议
objc_allocateProtocol
// 注册一个协议
objc_registerProtocol
// 构建一个实例对象(ARC下无效)
objc_constructInstance
// 析构一个实例对象(ARC下无效)
objc_destructInstance
// 为实例对象设置关联对象
objc_setAssociatedObject
// 获取实例对象的关联对象
objc_getAssociatedObject
// 清空实例对象所有的关联对象
objc_removeAssociatedObjects
```



#### class_

--------

```c++
// 为类添加实例变量
class_addIvar
// 为类添加属性
class_addProperty
// 为类添加方法
class_addMethod
// 为类添加遵循的协议
class_addProtocol
// 替换类的某个方法的实现
class_replaceMethod
// 获取类名
class_getName
// 判断是否为元类
class_isMetaClass
// 获取某个协议
objc_getProtocol
// 拷贝在运行时中注册过的协议列表  
objc_copyProtocolList
// 获取某类的父类
class_getSuperclass
// 设置某类的父类
class_setSuperclass
// 获取类的属性
class_getProperty
// 获取类的实例变量
class_getInstanceVariable
// 获取类的类变量
class_getClassVariable
// 获取实例方法
class_getInstanceMethod
// 获取类方法
class_getClassMethod
// 获取方法的实现
class_getMethodImplementation
// 获取类的实例的大小
class_getInstanceSize
// 判断是否实现了某个方法
class_respondsToSelector
// 判断是否遵循了某个协议
class_conformsToProtocol
// 创建类的实例
class_createInstance
// copy类的实例变量的列表
class_copyIvarList
// copy类的方法列表
class_copyMethodList
// copy类遵循的协议列表
class_copyProtocolList
// copy类的属性列表
class_copyPropertyList
```



#### object_

--------

```c++
// 对象copy(ARC无效)
object_copy
// 对象释放(ARC无效)
object_dispose
// 获取对象的类名
object_getClassName
// 获取对象的Class
object_getClass
// 设置对象的class
object_setClass
// 获取对象中，实例变量的值
object_getIvar
// 设置对象中实例变量的值
object_setIvar
// 获取对象中，实例变量的值（ARC无效，使用object_getIvar）
object_getInstanceVariable
// 设置对象中，实例变量的值(ARC无效，使用object_setIvar)
object_setInstanceVariable
```



#### method_

------

```c++
// 获取方法名
method_getName
// 获取方法的实现
method_getImplementation
// 获取方法的类型编码
method_getTypeEncoding
// 获取方法的某个参数
method_getNumberOfArguments
// copy方法的返回类型
method_copyReturnType
// 获取方法的返回类型
method_getReturnType
// copy方法的参数类型
method_copyArgumentType
// 获取方法的参数类型
method_getArgumentType
// 获取方法的描述
method_getDescription
// 设置方法的IMP指针
method_setImplementation
// 替换方法的实现
method_exchangeImplementations
```



#### property_

------

 ```objective-c
// 获取属性名
property_getName
// 获取属性的特性列表
property_getAttributes
// copy属性的特性列表
property_copyAttributeList
// copy属性中某特性的值
property_copyAttributeValue 拷贝属性中某特性的值
 ```



#### protocol_

------

```objective-c
// 判断一个协议是否遵循另一个协议
protocol_conformsToProtocol
// 判断两个协议是否一致
protocol_isEqual
// 获取协议的名称
protocol_getName
// copy协议的属性列表
protocol_copyPropertyList
// copy某协议所遵循的协议的列表
protocol_copyProtocolList
// copy协议的方法列表
protocol_copyMethodDescriptionList
// 使得协议遵循另一个协议
protocol_addProtocol
// 为协议添加属性
protocol_addProperty
// 获取协议中的某个属性
protocol_getProperty
// 为协议添加方法描述
protocol_addMethodDescription
// 获取协议中某个方法的描述
protocol_getMethodDescription
```



#### ivar_

--------

```c++
// 获取Ivar的名称
ivar_getName
// 获取Ivar的类型编码
ivar_getTypeEncoding
// 获取Ivar的内存偏移量
ivar_getOffset 获取偏移量
```



#### sel_

---------

```c++
// 获取方法名称
sel_getName
// 获取方法id
sel_getUid
// 注册方法 
sel_registerName
// 判断方法是否相等
sel_isEqual 判断方法是否相等
```



#### imp_

------

```c++
// 通过代码块创建IMP
imp_implementationWithBlock
// 获取函数指针中的代码块
imp_getBlock
// 移除IMP中的代码块
imp_removeBlock 移除IMP中的代码块
```



















