##Windows开发入门

####Windows10技术框架

------

Windows10是Windows 8和Windows Phone发展而来的产品

Windows CE内核(win7) -> Windows NT内核(Win8),并且Windows 8使用了Windows运行时架构

WinRT

Window 10通用应用程序只支持运行在沙盒内

COM组件，本质上是非托管的本地API

UWP：Windows 10通用应用程序 - 通用平台统一的应用程序，Windows 10支持多种开发语言来开发应用程序，

UWP可以选择的开发语言模型：

- C# & XAML
- VB & XAML
- C++ & XAML
- JavaScript & HTML5
- 游戏开发的话，还是采用C++的DirectX框架

<font color=red>C#&XAML、VB&XAML、C++&XAML这三种开发模型其实是类似的技术框架，他们都是使用XAML作为界面的编程语言，使用C#/VC/C++作为后台的开发语言</font>



####Windows 新建工程后的文件解析

####Windows应用程序的生命周期

- Running
- NotRunning
- Suspended

```
1.应用启动(从其他状态到Running状态)
2.引用激活(从NotRunning状态到Running状态)
3.应用挂起(从Running状态到Suspended状态)
4.应用恢复(从Suspended状态到Running状态)
```



####Package.appxmanifest文件是Windows 10应用程序的清单文件，声明应用的标识、应用的功能以及用来进行部署和更新的信息





##不同平台的适配



