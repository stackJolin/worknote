## Swift函数派发

<font color='orange'>什么是函数派发？</font>

程序运行的时候，执行/唤起某些方法，可能会存在多个方法，也可能一个方法都不存在，那么，处理这个方法调用的过程称为函数派发

函数从被书写到生成二进制，然后在程序运行的时候被调用，会经历两个最重要的过程：编译过程和运行过程。至于函数调用的时候，至于哪个方法被调用也是主要受到这两个过程中，编译系统和语言机制的影响

有的方法在编译器就已经确定了函数的地址，当然这个地址是相对地址，实际运行的时候会被重定向。有的方法，在运行时才能确定函数真正的执行地址。由此产生了<font color='red'>静态派发(static dispatch)</font>和<font color='red'>动态派发(dynamic dispatch)</font>



<font color='orange'>Swift目前有下面三种函数派发方式:</font>

- 直接派发/静态派发(Direct Dispatch)
- 函数表派发(Table Dispatch)
- 消息机制派发(Message Dispatch)

三种派发机制的性能 <font color=red>直接派发</font> > <font color='red'>函数表派发</font> > <font color='red'>消息派发</font>

```直接派发
直接派发:

```

```函数表派发
函数表派发:

```

```消息转发
消息转发:

```



#### Swift函数派发机制受哪些因素印象

---

- 函数的声明/实现的上下文(class，NSObject subClass，struct，enum，protocol，extension)
- 方法关键字修饰符(final、dynamic、@objc/@nonobjc、@inline)
- 编译器优化





#### 函数的声明/实现的上下文

-------

|                     | 声明       | Extension    |
| ------------------- | ---------- | :----------- |
| 值类型Struct        | 直接派发   | 直接派发     |
| 引用类型Swift-Class | 函数表派发 | 直接派发     |
| 协议Protocol        | 函数表派发 | 直接派发     |
| NSObject派生类      | 函数表     | 消息机制转发 |
|                     |            |              |



#### 方法关键字修饰符

---



#### 编译器优化

---



#### 相关文章

----

- https://www.jianshu.com/p/e0659093eaac
- https://mp.weixin.qq.com/s/U95QmOOjeXkk-yC23cuZCQ
- https://www.jianshu.com/p/e0659093eaac
- https://www.rightpoint.com/rplabs/switch-method-dispatch-table

