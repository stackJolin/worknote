## iOS中的计时器

常见的几种定时器：

- NSTimer
- CADisplayLink
- GCD
- NSThread(performSelector:afterDelay:)



#### NSTimer简单介绍

----

NSTimer使我们比较常用的定时器，但是他存在以下问题：

- 延迟
- 切换runloop失效
- 内存泄漏
- 进入后台后timer就会停止运行
- timer的创建和撤销，必须在一个线程

NSTimer具体的使用，以上问题产生的原因以及相应解决方案，请见NSTimer详解



#### CADisplayLink简单介绍

----

通常来讲：iOS设备的刷新频率事60HZ也就是每秒60次。那么每一次刷新的时间就是1/60秒 大概16.7毫秒。当我们的frameInterval值为1的时候我们需要保证的是CADisplayLink调用的target的函数计算时间不应该大于 16.7否则就会出现严重的丢帧现象。 在mac应用中我们使用的不是CADisplayLink而是CVDisplayLink它是基于C接口的用起来配置有些麻烦但是用起来还是很简单的。CADisplayLink使用场合相对专一，适合做UI的不停重绘，比如自定义动画引擎或者视频播放的渲染。

CADisplayLink存在问题：

- 切换runloop失效
- 内存泄漏

CADisplayLink具体的使用，以上问题产生的原因以及相应解决方案，请见CADisplayLink详解



#### GCD timer的简单介绍

----

GCDTimer可以完美解决NSTimer中存在的问题



#### NSThread(performSelector:afterDelay)的简单介绍

-----







