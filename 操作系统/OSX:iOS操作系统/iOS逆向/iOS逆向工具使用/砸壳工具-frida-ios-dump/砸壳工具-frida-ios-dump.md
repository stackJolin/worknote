## 砸壳工具-frida-ios-dump

关于`frida`的介绍，请见：https://zhuanlan.zhihu.com/p/41662447

简单来说`frida`是一款基于`python`和`javasrcipt`的hook框架，适用于`android`、`iOS`、`OSX`、`Windows`、`Linux`。主要使用了`二进制插桩技术`

`frida`源码链接：

下面我们来仔细介绍`frida-ios-dump`

源码链接：https://github.com/frida/frida



#### 原理

------

原理上和`dumpdecrypted`一致，都属于`动态砸壳`，即`直接从内存中copy出被解密加载后的Mach-O文件`。但是实现上有所差别，其中最大的差别在对`frida`的使用上



#### 源码解析

-----

源码链接：https://github.com/AloneMonkey/frida-ios-dump



#### 相关文章

-----

[https://benarvintec.com/2019/06/29/iOS%E9%80%86%E5%90%91%E5%AD%A6%E4%B9%A0%EF%BC%88%E4%B8%89%EF%BC%89%E7%A0%B8%E5%A3%B3%E5%8E%9F%E7%90%86%E4%B9%8Bfrida-ios-dump/](https://benarvintec.com/2019/06/29/iOS逆向学习（三）砸壳原理之frida-ios-dump/)

