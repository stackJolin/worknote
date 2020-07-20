## iOS启动过程研究

从手指点击App Icon到用户看到App的首页，这中间分为三个过程，也可以说是三个时间段：

- 第一阶段：main()函数执行之前
- 第二阶段：main()函数开始执行到didFinishLaunchingWithOptions执行完成
- 第三阶段：窗口展示到App的首页展示

下面分析每个阶段iOS系统和App都在做哪些事情？



#### 第一阶段 main()函数执行之前

-----

main()之前的工作，都是由系统完成的，大致过程如下：

- 执行exec()函数，操作系统分配进行，为该进程分配内存空间，将App对应的可执行文件(Mach-O格式)，加载到内存

- 将动态加载器(Dyld)加载到内存中

- 使用Dyld动态加载器加载和链接动态库列表

- Rebase

  ```swift
  Rebase在Image内部调整指针的指向。在过去，会把动态库加载到指定地址，所有指针和数据对于代码都是对的，而现在地址空间布局是随机化，所以需要在原来的地址根据随机的偏移量做一下修正
  ```

- Bind

  ```swift
  Bind是把指针正确地指向Image外部的内容。这些指向外部的指针被符号(symbol)名称绑定，dyld需要去符号表里查找，找到symbol对应的实现
  ```

- Objc

  - 注册Objc类 (class registration)
  - 把category的定义插入方法列表 (category registration)
  - 保证每一个selector唯一

- Initializers

  -  Objc的+load()函数

  - C++的构造函数属性函数

  - 非基本类型的C++静态全局变量的创建(通常是类或结构体)

    

<font color='orange' size='3'>造成改过程慢的可能因素：</font>

- 动态库加载过多
- ObjC类太多，方法太多
- ObjC +(load)方法太多
- C++构造函数、属性函数
- C++全局静态变量太多

<font color='lightgreen' size='3'>改进措施：</font>

- 合并一动态库
- 删除无用的类和方法



####第二阶段main()函数开始执行到didFinishLaunchingWithOptions执行完成

---

改阶段主要是主界面的构建，涉及到下面这些内容：

- TabVC、NavVC以及HomeVC的界面元素的初始化
- 图片I/O，解码
- unarchive本地数据
- 一些库的初始化工作(尤其是一些库要求在主线程操作的库)

可以通过Instrument中的Time Profile诊断每个方法所占用的性能

<font color='lightgreen' size='3'>改进措施：</font>

- 图片压缩，解码放后台进程
- 三方库初始化延迟操作
- 能够放后台的放到后台进程去做