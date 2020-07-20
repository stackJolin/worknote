## Swift内存分配

该文章主要介绍iOS中的内存分配和Swift不同数据类型的内存分配问题

在Swift中，值类型都是存储在栈中的，引用类型都是存储在堆中的。苹果官方建议多使用值类型，因为栈的读取快。’多使用‘也不是一定，下面会讨论这个问题





<font color='orange'>不同数据类型在内存中的情况:</font>

- 基本数据类型
- Swift Struct
- Swift Class
- NSObject的派生类



#### 基本数据类型

-----



#### Swift Struct

-----

<font color='orange'>不包含引用属性的Struct:</font>





<font color='orange'>包含引用属性的Struct:</font>





####Swift Class

-----



#### NSObject派生类

-----



#### 其他Tips

----

<font color='orange'>值类型的复制时机：</font>

```
我们一般的理解是：值类型在赋值的时候发生复制行为。但是这是错误的，Swift语言始终认为，在没有必要复制的情况下，是不需要复制的。所以正确的结论是：对于Int、Double、String、Struct等基础类型是在赋值的时候发生复制的。而对于集合类型(Array、Dictionary、Set)来说，是在数据发生改变的时候触发复制操作的
文章链接:https://juejin.im/post/5a7b04c86fb9a0634b4d632a
```



#### 相关文章

---

- https://swiftrocks.com/memory-management-and-performance-of-value-types

  这个主要重点在 值类型 在 堆区 分配内存的问题

- https://www.jianshu.com/p/ecbc1bc37d52

- https://tech.meituan.com/2018/11/01/swift-compile-performance-optimization.html

- https://medium.com/@jjuou2/optimizing-swift-performance-2c0ade5d1d28