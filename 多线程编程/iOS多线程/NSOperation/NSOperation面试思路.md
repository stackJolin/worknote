## NSOperation面试思路

---------



#### 基本介绍

--------

是`iOS`端的多线程编程技术，基于`GCD`，相比于`GCD`更加面向对象。



#### 和GCD对比

----------





#### 基本用法

-----------

###### 用法一：创建一个`operation`，然后执行

```objective-c

```



###### 用法二：创建一个`operationQueue`，添加多个`operation`

```objective-c

```



###### 用法三：设置依赖

```objective-c

```





#### `NSOperation`需要注意的点

-------------

- `NSBlockOperation`可以追加多个`block`，但是必须在`start`方法之前，否则`报错`
- 优先级问题
- 依赖
- 自定义`Operation`的时候，需要注意自动释放池的问题



#### `NSOperationQueue`需要注意的点

-------

- 最大并发数
- cancel