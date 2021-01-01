## iOS逆向简介

先来回答两个问题：

- 为什么要越狱
- 为什么要砸壳

<font color='orange'>**为什么要越狱：**</font>

wiki的回答：[https://zh.wikipedia.org/wiki/%E8%B6%8A%E7%8D%84_(iOS)](https://zh.wikipedia.org/wiki/越獄_(iOS))

<font color='orange'>**为什么要砸壳：**</font>

我们提交的`.ipa`文件到`appStore`的过程中，`Apple`会对二进制加密，也就是所谓的`加壳`，`加壳`的目的是压缩程序以及防止程序被非法修改以及静态反编译。

加壳后的二进制，在磁盘中是以加壳后的文件存在的，但是在加载到内存后，会进行还原。`砸壳`的目的就是为了`查看程序的实现流程`，并以此为基础，实现自己的`App`或者修改他人的`App`

所以，要想砸壳(动态砸壳)，必须先要手机进行越狱，获取`Root`权限。在获取`Root`权限后，可以将运行的程序从内存中`copy`处理，此时，内存中的程序是脱了壳的



需要逆向的场景：

- 微信抢红包
- 动态库注入
- 非Xcode安装的App如何动态调试
- 越狱



#### 逆向需要的工具

-----

Mac上安装：

- iFunbox

- yololib

  ```c++
  yololib可以给mach-o文件注入framework
  ```

- dumpdecrypted

  ```c++
  砸壳工具
  ```

- clutch

  ```c++
  砸壳工具
  ```

- iOSOpenDev

  ```c++
  Tweak工具
  ```

- Theos

  ```c++
  Tweak工具.（推荐 Theos，安装麻烦，而且会因为 Theos 版本问题导致 Tweak 报错，喜欢探究的可以用一下。）
  ```

- iTools

- dpkg

  ```c++
  安装传送到越狱设备上的Debian Package
  ```

- PP助手（Mac/PC）

  ```c++
  下载已经砸完壳的引用，传文件等
  ```

- scp

  ```c++
  文件传输
  ```

- IDA

  ```c++
  静态分析工具(二进制文件作为输入，经过处理后返回这个文件的汇编代码)
  ```

- Hopper

  ```c++
  静态分析工具(二进制文件作为输入，经过处理后返回这个文件的汇编代码)
  ```

- Class-dump

  ```c++
  静态分析工具
  ```

- Reveal：UI分析

手机安装：

- 越狱

- Cydia

  ```c++
  越狱手机安装包管理文件
  ```

  

- OpenSSH(cydia)

  ```c++
  链接越狱设备
  ```

- Usbmuxd：

  ```c++
  链接越狱设备
  ```

- iFile(Cydia)

- Cycipt(Cydia)

  ```c++
  动态分析工具
  ```



#### 其他理论知识

------

- 二进制分析方法(静态分析、动态分析查找目标函数，hook相关方法)
- 苹果重签名机制，免越狱插件



#### 相关文章

-----

https://niyaoyao.github.io/2017/05/09/Learning-Reverse-From-Today-D4/