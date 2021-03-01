## YYModel源码解析

--------------

涉及到的主要的类：

- _YYModelMeta
- _YYModelPropertyMeta
- YYClassInfo
- YYClassPropertyInfo
- YYClassMethodInfo
- YYClassIvarInfo



#### _YYModelMeta

-----------------





#### _YYModelPropertyMeta

-----------















#### 知识点

-----

-类型编码(Type-Encoding)

`Type-Encoding 是指定的一套类型编码，在使用 runtime 获取某个类的成员变量、属性、方法的时候，能同时获取到它们的类型编码，通过这个编码就能辨别这些成员变量、属性、方法的数据类型（也包括属性修饰符、方法修饰符等）`下面是类型编码的官方文档

https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Articles/ocrtTypeEncodings.html

https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Articles/ocrtPropertyIntrospection.html



##### 相比JSONModel，YYModel的优势有哪些

------

###### 性能

```
1.YYModel内部使用了大量的内联函数，内联函数的好处就是，编译时，直接代码替换，省去了方法调用的性能损耗
```

###### 代码结构

```
1.YYModel是通过对NSObject的扩展来实现的，所以相比较JSONModel来说，是没有入侵性的
```





#### YYModel存在的问题

------

