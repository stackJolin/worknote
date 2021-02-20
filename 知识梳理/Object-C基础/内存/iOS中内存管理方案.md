## iOS中内存管理方案

--------

对于不同的场景，系统提供了不同的内存管理方案，主要有以下几种：

- TaggedPointer：主要是一些小对象，比如NSNumber等
- NonPointer_ISA：64位架构下的iOS应用
- Hashamp：主要是引用计数和弱引用表(sideTables，refCountMap、weak_table_t)

接下来我们逐个介绍



#### TaggedPointer

-------

文章：https://blog.devtang.com/2014/05/30/understand-tagged-pointer/



#### NonPointer_ISA - 非指针类型的ISA

--------

这个设计和`TaggedPointer`有点类似

https://xiaopengmonsters.github.io/2018/06/13/%E5%86%85%E5%AD%98%E7%AE%A1%E7%90%86--%E5%86%85%E5%AD%98%E7%AE%A1%E7%90%86%E6%96%B9%E6%A1%88/



#### HashMap

--------

具体的请参考其他文章