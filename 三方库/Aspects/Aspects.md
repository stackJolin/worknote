## Aspects

<img src='../img/20160629125258569' width=500>

Aspects对于方法hook的时机，和我们平常使用的方法交换不太一致。它是在forwardInvocation阶段去hook函数的。



#### 涉及到的知识点

-----

- 实例方法的交换和类方法交换的差异
- NSMapTable和NSHasTable

#### 方法交换存在的坑点

------

- https://www.jianshu.com/p/f07ece04f7f5
- FBSDKSwizzle和TABAnimatioin的问题

#### 其他类似的库

---

- JRSwizzle
- RSSwizzle



#### 其他

----

- 如何Hook black：http://yulingtianxia.com/blog/2018/02/28/Hook-Objective-C-Block-with-Libffi/
- 什么是libffi
- block是怎么实现的
- kvo源码https://github.com/renjinkui2719/DIS_KVC_KVO