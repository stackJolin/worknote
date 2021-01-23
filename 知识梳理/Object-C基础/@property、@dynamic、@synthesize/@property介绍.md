## @property简介

#### 介绍

----

#### 属性生命关键字

----

| **种类**     | **关键字**                | **描述**                                                     |
| ------------ | ------------------------- | ------------------------------------------------------------ |
| 赋值操作类   | strong                    | ARC                                                          |
| 赋值操作类   | weak                      | ARC                                                          |
| 赋值操作类   | copy                      | MRC & ARC 拷贝副本(深度拷贝)                                 |
| 赋值操作类   | retain                    | 进行保持操作，持有对象，引用计数+1，(仅MRC可用)              |
| 赋值操作类   | assign                    |                                                              |
| 赋值操作类   | unsafe_unretained         |                                                              |
|              |                           |                                                              |
| 读写权限类   | readonly                  | 表明变量只读，只生成getter方法                               |
| 读写权限类   | readwrite                 | 表明变量可读写，默认是readwrite                              |
|              |                           |                                                              |
| 修改方法名类 | setter = 新的setter方法名 | 修改默认生成的方法名( selector )                             |
| 修改方法名类 | getter = 新的getter方法名 | 修改默认生成的方法名( selector )                             |
|              |                           |                                                              |
| 原子操作类   | noatomic                  | 非原子属性，线程不安全                                       |
| 原子操作类   | atomic                    | 原子性操作，线程安全                                         |
|              |                           |                                                              |
| 空类         | nonnull                   | 不能为空                                                     |
| 空类         | nullable                  | 可以为空                                                     |
| 空类         | null_resettable           | setter 方法可以是 nil，getter 方法不能返回 nil，要重写 getter 方法 |
| 空类         | null_unspecified          | 不确定是否为空                                               |



口诀：6赋值，2读写，2方法，2原子，4空



#### 注意事项



----

**<font color='red'>copy使用注意事项一：</font>**

```
copy修饰的属性，其实在被赋值的时候，执行的是[obj copy]的方法，如果obj没有实现NSCopying协议，那么就会crash。
```





