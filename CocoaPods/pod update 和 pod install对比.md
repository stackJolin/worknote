## pod update 和 pod install对比

-----------



#### 对于pod install

-------------------------

官方用法：

- 第一次获取库的时候，即第一次执行`pod install`
- 每次编辑`Podfile`文件的时候使用

每一次运行`pod install`命令后，都会去下载安装新的库，并且会修改`Podfile.lock`文件中记录的库的版本。`Podfile.lock`文件是用来追踪和锁定这些库的版本的。

运行`pod install`后，它仅仅只能解决`Podfile.lock`中没有列出来的依赖关系。

在`Podfile.lock`中列出的那些库，也仅仅只是去下载`Podfile.lock`中指定的版本，并不会去检查最新的版本。

没有在`Podfile.lock`中列出的那些库，会去检索`Podfile`中指定的版本，比如`pod ‘myPod’, ‘~>1.2’`。



#### 对于pod update

----------

当你运行了 `pod update PODNAME`命令，**CocoaPods**将不会考虑`Podfile.lock`中列出的版本，而直接去查找该库的新版本。它将更新到这个库尽可能新的版本，只要符合`Podfile`中的版本限制要求。

如果使用`pod update` 命令不带库名称参数，**CocoaPods**将会去更新`Podfile`中每一个库的尽可能新的版本。



#### 正确用法

---------

使用`pod update PODNAME`可以去更新一个库的指定版本（检查相应的库是否存在更新的版本，并且更新）,相对应的，使用`pod install`将不会更新那些已经下载安装了的库。

当你在`Podfile`中添加了一个新的库时，你应该使用`pod install`命令，而不是`pod udpate`，这样安装了新增的库，也不会重复安装已经存在的库。

使用`pod update`仅仅只是去更新指定库的版本（或者全部库）。































