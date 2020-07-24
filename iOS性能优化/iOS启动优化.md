## iOS启动过程研究

从手指点击App Icon到看到首页，App主要经历下面三个阶段：

- main()函数之前，我们称作是第一阶段
- 从main()函数开始，到appDelegate的didFinishLaunchingWithOptions方法完毕，我们称作是第二阶段
- 从appDelegate的didFinishLaunchingWithOptions到首页显示出来，我们称作是第三阶段



#### 第一阶段(pre-main)在做什么

----

- 将app对应的mach-o二进制文件加载到内存
- 加载Dyld动态连接器
- 利用Dyld动态连接器加载其他动态库
  - 根据header文件，递归加载动态库
  - Rebase Image内部指针地址重定向
  - Bind把指针指向Image外面的内容
  - Objc setup
    - 注册Objc类
    - 在类中插入category定义的方法
    - 保证每个selector唯一
  - Initializers
    - Objc的+load()函数
    - C++的构造函数
    - 非基本类型的C++的静态全局变量

由上可知，第一阶段影响启动的速度的原因有：

- 动态库过多
- Objc类多，方法多
- Objc的+load()方法多
- C++静态函数多
- C++静态对象多

优化：

- 代码瘦身
- +load()优化

#### 第二阶段在做什么

----

通过Instrument的Time profiler查看启动的时候，哪些具体的函数会耗时，该放后台放后台，该延迟启动就延迟启动

第三阶段在做什么

-----





#### 相关文章

----

- https://asciiwwdc.com/2016/sessions/406
- https://tech.meituan.com/2018/12/06/waimai-ios-optimizing-startup.html

