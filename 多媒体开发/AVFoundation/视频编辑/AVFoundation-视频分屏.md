## AVFoundation - 视频分屏

-----

主要分为以下几步：

- 创建`AVMutableComposition`，初始化`AVMutableComposition`的视频轨道和音频轨道
- 创建一个`AVMutableVideoComposition`，并指定`customVideoCompositorClass`属性，视频renderSize，视频帧率
- 创建视频合成指令`AVVideoCompositionInstruction`,并添加到`AVMutableVideoComposition`中
- 创建一个`AVMutableAudioMix`混音器
- 处理播放
- 处理导出



#### 创建多媒体合成器和该合成器的一个视频轨道和一个声音轨道

-------------------





#### 将多个视频资源文件的视频或者音频资源，有策略的插入到`AVMutableComposition`的视频轨道和音频轨道

-----------







#### 构建视频合成器 - AVMutableVideoComposition

-----------------

这里是最复杂的，主要分为以下几个步骤：

- 实现`AVVideoCompositing`协议，构建一个类。这个类的主要目的是，当播放视频或者导出视频的时候，可以通过这个类拿到具体的`buffer`
- 实现`AVVideoCompositionInstruction`，构建一个类。这个类的主要目的是去处理最后视频的显示帧



###### 每一帧的合成原理：

```objective-c
假设我们是四个视频资源，每个视频资源占每一帧大小的1/4，那么他们的位置就是:左上，左下，右上，右下
每一帧都有一个背景图，这个背景图是整个视频大小的背景图，被切成了四份，每一个资源，对应其中一个背景图
有了上面的理论，渲染一个完整的帧的过程如下：
- 创建一个空的图片`__block CIImage *result = [CIImage imageWithColor:[CIColor colorWithCGColor:[UIColor whiteColor].CGColor]];`
- 设置图片的大小(通过CIFilter的"CICrop"滤镜)
-	每一个资源，通过`AVAsynchronousVideoCompositionRequest`，获取到当前的应该展示的图片，然后按照一定的策略和背景图进行融合(CFilter滤镜)
-	将每个资源绘制到背景图上(CIFilter的滤镜"CISourceOverCompositing"，覆盖组合滤镜)
- [request finishWithComposedVideoFrame: newFrameBuffer];
```





#### 构建混音器

------------





#### 处理播放

-------------





#### 处理导出

----------------

