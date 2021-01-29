## Custom Input Source

------

这里自定义的`Source`指的是`source0`，而不是`source1`。因为`source1`都是由内核发出的，应用层开发者基本上没有办法自定义。定制`source0`必须从另一个线程手动发出信号

Note:`什么是source0、source1见同目录下的其他文章`



#### 自定义`source0`所需要的基本信息

--------

- 应用层开发者希望输入源的处理信息
- 一个调度(scheduler)函数，让感兴趣的客户端知道如何联系输入源
- 

