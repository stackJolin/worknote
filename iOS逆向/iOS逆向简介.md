## iOS逆向简介

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
  静态分析工具
  ```

- Hopper

  ```c++
  静态分析工具
  ```

- Class-dump

  ```c++
  静态分析工具
  ```

  

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