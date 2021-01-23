## 基于GCD的timer

-------

`NSTimer`存在的问题：

- 后台不执行
- 内存泄漏
- 不准确
- trackingMode的问题
- `NSTimer`的创建和销毁必须在同一个线程
- `NSTimer`是基于`Runloop`的，默认情况下，主线程的`runloop`是开启的，其他线程是关闭的。所以如果在其他线程使用`NSTimer`必须开启`Runloop`



