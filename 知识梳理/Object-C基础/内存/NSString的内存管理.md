## NSString的内存管理

---------------

在iOS开发中，NSString的内存管理和OC中的其他对象相比，略微比较复杂。

使用`clang rewrite -objc`转化后，根据NSString对象的`ISA`指针，大致可以将NSString分为以下几类：

- __NSCFConstantString

  ```markdown
  字符串常量，是一种编译时常量，retainCount值很大，对其操作，不会引起引用计数变化。
  这种对象存储在字符串常量区。
  ```

- __NSCFString

  ```markdown
  __NSCFString对象是在运行时创建的一种NSString子类，创建后引用计数会加1。
  这种对象存储在堆上。
  ```

- __NSTaggedPointerString

  ```markdown
  标签指针，是苹果在64位环境下对NSString、NSNumber等对象做的一些优化。
  
  简单来讲，可以理解为把指针指向的内容直接放在了指针变量的内存地址中，因为在 64 位环境下指针变量的大小达到了 8 位足以容纳一些长度较小的内容。于是使用了标签指针这种方式来优化数据的存储方式。在运行时根据实际情况创建。
  
  对于 NSString 对象，当字符串是由数字、英文字母组合且长度小于等于 9 的时候会自动成为 NSTaggedPointerString 类型。如果有中文或其他特殊符号，则会直接成为 __NSCFString 类型。
  
  ```

  

#### 相关文章

----

- https://blog.csdn.net/u013378438/article/details/82790332