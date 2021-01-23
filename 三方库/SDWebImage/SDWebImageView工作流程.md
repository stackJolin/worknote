## SDAnimatedImageView工作流程

--------

主要分为两个流程：

- 渲染流程
- 解码流程

`SDAnimatedImageView`的渲染是内置的`SDAnimatePlayer`来实现的，而每一帧图片，是由`SDAnimatedPlayer`中的`imageProvider`提供的，其实这个`imageProvider`就是`SDAnimatedImage`，而真正做解码工作的是`SDAnimatedImage`中的`animatedCoder`，而这个`animatedDecoder`默认是`SDImageIOAnimatedCoder`。当然，`SDWebImage 5.0`开始的版本，都是做了插件化以及协议化的，这个`animatedCoder`可以自己实现



接下来，我们就来看一下下面几个类是如何协同工作，从而完成图片的解码和渲染的

- SDAnimatedPlayer
- SDAnimatedImage
- SDImageIOAnimatedCoder



#### SDAnimatedPlayer的刷新

--------

`SDAnimatedPlayer`内置了一个`displayLink`，默认频率和系统一致，每秒钟刷新60次(近似值)。刷新调用的方法是`- (void)displayDidRefresh:(SDDisplayLink *)displayLink`







#### SDImageIOAnimatedCoder的解码

--------



