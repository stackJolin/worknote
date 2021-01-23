## iOS动态库和静态库介绍

----



#### 基本概念

-----

###### 什么是库?

```markdown
库(Library)说白了就是一段编译好的二进制代码，加上头文件就可以供别人使用。

什么时候我们会用到库呢？一种情况是某些代码需要给别人使用，但是我们不希望别人看到源码，就需要以库的形式进行封装，只暴露出头文件。另外一种情况是，对于某些不会进行大的改动的代码，我们想减少编译的时间，就可以把它打包成库，因为库是已经编译好的二进制了，编译的时候只需要 Link 一下，不会浪费编译时间。

上面提到库在使用的时候需要 Link，Link 的方式有两种，静态和动态，于是便产生了静态库和动态库。
```



###### 什么是静态库?

```markdown
静态库即静态链接库（Windows 下的 .lib，Linux 和 Mac 下的 .a）。之所以叫做静态，是因为静态库在编译的时候会被直接拷贝一份，复制到目标程序里，这段代码在目标程序里就不会再改变了。

静态库的好处很明显，编译完成之后，库文件实际上就没有作用了。目标程序没有外部依赖，直接就可以运行。当然其缺点也很明显，就是会使用目标程序的体积增大。
```



###### 什么是动态库

````markdown
动态库即动态链接库（Windows 下的 .dll，Linux 下的 .so，Mac 下的 .dylib/.tbd）。与静态库相反，动态库在编译时并不会被拷贝到目标程序中，目标程序中只会存储指向动态库的引用。等到程序运行时，动态库才会被真正加载进来。

动态库的优点是，不需要拷贝到目标程序中，不会影响目标程序的体积，而且同一份库可以被多个程序使用（因为这个原因，动态库也被称作共享库）。同时，运行时才载入的特性，也可以让我们随时对库进行替换，而不需要重新编译代码。动态库带来的问题主要是，动态载入会带来一部分性能损失
````



#### iOS动态库和静态库的形式

------

静态库：`.a`

动态库：`.dylib`、`.tdb`

为什么不包含`Framework`呢？，因为`Framework`只是一种打包形式，将库的二进制文件，头文件和有关的资源文件打包到一起，方便管理和分发，和静态库动态库的本质是没有什么关系。

```markdown
在 iOS 8 之前，iOS 平台不支持使用动态 Framework，开发者可以使用的 Framework 只有苹果自家的 UIKit.Framework，Foundation.Framework 等。因为 iOS 应用都是运行在沙盒当中，不同的程序之间不能共享代码，同时动态下载代码又是被苹果明令禁止的，没办法发挥出动态库的优势，实际上动态库也就没有存在的必要了。
由于上面提到的限制，开发者想要在 iOS 平台共享代码，唯一的选择就是打包成静态库 .a 文件，同时附上头文件。

iOS 8/Xcode 6 推出之后，iOS 平台添加了动态库的支持，同时 Xcode 6 也原生自带了 Framework 支持。
```





#### iOS `Framework`分类

-------

Framework:

- Dynamic Framwork

  ```markdown
  .
  ├── EmbeddedFramework
  ├── Headers
  │   └── EmbeddedFramework.h
  ├── Info.plist
  ├── Modules
  │   └── module.modulemap
  └── _CodeSignature
      └── CodeResources
  ```

  - 系统级别的`Dynamic Framework`

  - 用户/开发者级别的`Dynamic Framework`-`Embedded Framework`

    ```markdown
    首先，他需要开发者账号支持并开启`Group`功能，Embedded Framework 可以在 Extension可执行文件 和 APP可执行文件 之间共享，但是不能像系统的动态库一样，在不同的 APP（进程） 中共享。Embedded Framework是iOS8引入的为了方便Extention和宿主APP公用一份代码库而引入的，Embedded Framework必须是Dynamic framework(在buildSeting中设置为Dynamic)
    ```

    

- Static Framework

  ```markdown
  .
  ├── Headers
  │   └── StaticFramework.h
  ├── Info.plist
  ├── Modules
  │   └── module.modulemap
  ├── StaticFramework
  └── _CodeSignature
      ├── CodeDirectory
      ├── CodeRequirements
      ├── CodeRequirements-1
      ├── CodeResources
      └── CodeSignature
  ```

  

- Umbrella framework：将几个已经封装好的 framework 封装成一个，封装的这种 framework 就是 umbrella framework。我们最熟悉的就是`pod`



#### 官方文档

-----

https://developer.apple.com/library/archive/documentation/MacOSX/Conceptual/BPFrameworks/Frameworks.html#//apple_ref/doc/uid/10000183-SW1



#### 其他文章

--------

https://juejin.cn/post/6844903848255946759

https://cloud.tencent.com/developer/news/252403



#### 问题

------

1.为什么动态库相比于静态库会更慢



