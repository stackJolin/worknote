## AVVideoCompositing协议详解

-------

如果你想修改`AVPlayer`的渲染过程，需要从`AVPlayerItem`入手，主要分为以下几步：

- 自定义`AVVideoCompositing`类
- 构建`AVMutableVideoComposition`
- 自定义`AVVideoCompositionInstruction`
- 将自定义的`AVMutableVideoComposition`的`customVideoCompositorClass`属性，设置为我们自定义的实现了`AVVideoCompositing`协议的类
- 

