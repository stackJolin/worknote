## SwiftPM简介

我们知道，目前Mac端有很多比较好的包管理工具，比如CocoaPods、Carthage，这些工具已经很轻大了，为什么还要开发Swift Package Manager工具呢？

- Swift是一个跨平台语言(mac、Ubuntu)，所以需要一个跨平台的包管理工具



#### SwiftPM相关概念

-----

https://swift.org/package-manager/

- Modules

  ```
  模块中包含代码，每个模块有自己的命名空间。该模块内的可以被外界访问的代码，是有严格的访问控制的。
  一个程序，可能将其所有的代码包含在一个module中，或者他可以依赖其他模块。除了少数系统提供的模块，比如Darwin on mac或者Glibc on Linux，大部分所依赖的代码需要下载下来，按照一定的顺序构建
  ```

- Packages

  ```
  一个Package包含Swift文件和一个清单文件，清单文件叫做Package.swift，这个清单文件是通过PackageDescription Module模块来定义包的名称和内容
  一个Package有一个或者多个targets，每个target指定了一个产品，同时可以声明一个或者多个依赖
  ```

- Products

  ```
  target被构建后的结果，可以是一个库，或者是一个可执行文件。库包含了一些代码，可以被其他Swift代码导入引用；可执行文件，可以直接被操作系统运行
  ```

- Dependencies

  ```
  一个target的所有的依赖，都是一个个的module。一个依赖，包含一个相对的或者完整的URL，URL指明了package的位置，当然也可以指定版本号。PM的的作用就是 ‘通过自动下载和构建所依赖的项目，来减少协调成本’，并且这个过程是递归的：因为一个package可能会依赖其他package，最终会形成一个依赖拓扑，PM会一直执行download和built过程，直到拓扑完成
  ```

- Targes

  ```
  无需解释
  ```



#### SPM依赖颗粒度

-----

- 模糊版本号

  `.package(url: "https://github.com/...git", from: "1.1.3")`指定1.1.3以上的版本

  `.package(url: "https://github.com/...git", "1.1.0"..."1.2.1")`指定版本范围

- 精确版本号

  `.package(url: "https://github.com/...git", .exact("1.2.3"))`

- beta版本号

  `.package(url: "https://github.com/...git", from: "1.1.3-beta.4")`

- 分支

  `.package(url: "https://github.com/...git", .branch("bugfix/issue-121"))`

- Commit号

  `.package(url: "https://github.com/...git",
  .revision("04136e97a73b826528dd077c3ebab07d9f8f48e2"))`



#### SPM常用的命令

-------



#### 学习SPM时的问题

----

- Package.resolved这个东西要不要加入.gitignore文件中
- CocoaPods迁移到SwiftPM
  - 如果某个库，不支持SwiftPM怎么办
    - Fork the repository (if it's on GitHub, it's just a few clicks)
    - Add the package description file (see example above)
    - Commit & push your changes (using git)
    - Use your own forked repository URL in Xcode
    - +1 create a Pull Request! 🎉
- CocoaPods能否和SwiftPM共存
- Xcode11开始，由于有了SwiftPM，编译缓存多了`SourcePackages`这样一个目录
- 如何声明自己的一个库



#### SPM和CocoaPods共存

----

https://github.com/CocoaPods/CocoaPods/issues/8885

https://github.com/CocoaPods/Xcodeproj/pull/687





#### 源码模块

-----

- Commands：命令行模块
- PackageLoading：下载模块
- PackageGraph:
- PackageDescription
- PackageModel
- SourceControl：Git源控制模块
- swift-build
- swift-package
- swift-run
- swift-test



#### PPT要讨论的几个问题

----

- 为什么要有SPM
- CocopaPods如何迁移到SPM
- SPM能否和Cocopods共存
- SPM工作流程
- SPM依赖拓扑图
- SMP源码解读
- 如何创建一个Swift库，供其他人使用
- 之前发布的私有库或者公有库如何支持SPM



-------------

添加一个依赖：

	-	url
	-	version
	-	Name



#### 相关资料

----

- https://developer.apple.com/videos/play/wwdc2018/411/

- https://forums.swift.org/c/development/SwiftPM

- https://bugs.swift.org

- https://ci.swift.org

- https://swiftpm.co 查询支持swiftPM的库

- Swift服务端架构Vapor、Kitura、SwiftNIO

  - https://www.raywenderlich.com/890381-getting-started-with-server-side-swift-with-vapor
  - https://www.raywenderlich.com/1124580-swiftnio-a-simple-guide-to-async-on-the-server
  - https://www.raywenderlich.com/1323930-vapor-vs-kitura-choosing-a-server-side-swift-framework
  - https://www.raywenderlich.com/1079484-kitura-tutorial-getting-started-with-server-side-swift
  - https://swift.org/server/ - SSWG，swift工作小组

  -

- https://blog.bitrise.io/migrating-from-cocoapods-to-swift-package-manager cocoapods迁移到SwiftPM