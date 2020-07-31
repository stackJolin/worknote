## CocoaPods源码解析



- downloder
- Comomd
- installer
- config
- XCodeProj



#### DSL - domain specific languages

-----

领域特定语言：在模型之上建立的一种更加灵活的对模型化的理解和使用方式。相对应的就是General Purpose Language(GPL) - 通用编程语言

定义：A specialized computer language designed for a specific task

- DSL需要完成的工作

  - 设计语义和语法，定义DSL中的元素是什么样的，元素代表什么意思
  - 实现parser，对DSL解析，最终通过解释器来执行 

- DSL的设计原则

  DSL 最大的设计原则就是**简单**，通过简化语言中的元素，降低使用者的负担；无论是 Regex、SQL 还是 HTML 以及 CSS，其说明文档往往只有几页，非常易于学习和掌握。但是，由此带来的问题就是，DSL 中缺乏抽象的概念，比如：模块化、变量以及方法等

- 嵌入式DSL



#### 模块

-----

- Command命令模块：主要负责从CLI环境接受命令，调用相应的模块来执行命令

- Config配置模块，单例类。负责初始化各种文件路径及相应文件类的实例对象

- Installer安装模块，核心安装类,负责调用其他模块完成分析,检查等验证之后创建pods文件并安装

- Analyzer分析模块，检查分析模块,负责pod、Podfile、target等文件和目录分析

- UI视图打印模块，负责提供方便的打印输出到CLI中的方法

- Downloader下载模块，负责下载pod文件等到基础模块

- Generator生成模块，负责生成Pod工程文件及其infoPlist、xcconfig等依赖文件

- Xcodeproj工程文件，负责创建和修改Xcode工程

- Sandbox文件模块，负责生成和管理pods目录

  

#### 相关知识

-----

- RubyGem：Ruby的包管理工具

- 面向信仰编程

- Cocoapods/CLAide：A small command-line interface framework.

  CLAide是一个命令的解析器，通过简单的 API 提供命令的构造、参数解析、生成help等功能

- Bundler思想

- CocoaPods/Molinillo:依赖仲裁算法

- Cocoapods-core：DSL解析器

- Cocoapods-downloader：下载模块

- Xcoreproj：Xcode文件生成

- RoR开发模式：Ruby On Rails的缩写

- eval特性

- 元编程



#### Ruby的各种相关概念

-------

- RVM

  用于帮你安装Ruby环境，帮你管理多个Ruby环境，帮你管理你开发的每个Ruby应用使用机器上哪个Ruby环境。Ruby环境不仅仅是Ruby本身，还包括依赖的第三方Ruby插件。都由RVM管理

- Rails

  网页开发框架

- RubyGems

  RubyGems是一个方便而强大的Ruby程序包管理器（ package manager），类似RedHat的RPM.它将一个Ruby应用程序打包到一个gem里，作为一个安装单元。无需安装，最新的Ruby版本已经包含RubyGems了

- Gem

  Gem是封装起来的Ruby应用程序或代码库。在终端使用的gem命令，是指通过RubyGems管理Gem包。

- Gemfile

  定义你的应用依赖哪些第三方包，bundle根据该配置去寻找这些包。

- Rake

  Rake是一门构建语言，和make类似。Rake是用Ruby写的，它支持自己的DSL用来处理和维护Ruby程序。 Rails用rake扩展来完成多种不容任务，如数据库初始化、更新等。

- Rakefile

  Rakefile是由Ruby编写，Rake的命令执行就是由Rakefile文件定义。

- Bundle

  相当于多个RubyGems批处理运行。在配置文件gemfilel里说明你的应用依赖哪些第三方包，他自动帮你下载安装多个包，并且会下载这些包依赖的包。

#### 相关链接

-----

- DSL及DSL的应用 <https://draveness.me/dsl>
- Fastlane，iOS自动打包上传AppStore <https://juejin.im/post/5a7d51986fb9a063435ece35>



#### Pod install 命令背后执行了这么10件大事

-------

- 准备工作
- 