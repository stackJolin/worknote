## AVVideoComposition介绍

-------

对两个或多个视频轨道组合在一起的方法给出了总体描述。由一组时间范围和描述组合行为的介绍内容组成，这些信息出现在组合资源内的任意时间点。除了包含描述输入视频层组合的信息之外，还提供了配置视频组合的渲染尺寸、缩放和帧时长等属性。视频组合配置确定了委托对象处理时 `AVComposition`
的呈现方式。这里的委托对象比如AVPlayer或AVAssetImageGenerator。

`AVVideoComposition`并不是 `AVComposition`的子类，没有直接关联。在视频播放、淡出或处理时会用 `AVVideoComposition`来控制资源视频轨道的视频组合行为。

