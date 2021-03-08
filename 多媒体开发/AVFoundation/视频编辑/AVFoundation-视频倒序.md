## AVFoundation 视频倒序

------

这里主要有两个过程：

- 预览播放
- 导出



#### 预览播放

--------

`AVPlayer`是支持倒序播放的

```objective-c
// 1.判断playItem是否支持倒序播放
if ([self.avplayer.currentItem canPlaySlowReverse]) {
    // 2.如果支持倒序播放的话，将`rate`设置成-1
    self.avplayer.rate = -1.0;
    // 3.seek到结尾
    [self.avplayer seekToTime:self.avplayer.currentItem.duration];
}else{
    // 执行，先到处，在播放的策略
}
```

但是，这种方式并不能适应所有的视频，有的视频就不可以，猜测和关键帧(I帧)有关系。即便有的视频可以倒序，但是有的时候也会存在跳帧。

那么我们就可以换一种策略：`先将逆序视频做好，然后在播放`



#### 导出 - 倒序

--------

在看倒序之前，请先看一下正常的视频格式转换的流程`AVFoundation-视频格式转换`，这里介绍了正序的导出流程。那么倒序的流程有什么不同点呢？

第一点：倒序的话，必须从后先前遍历原始视频数据。如果都把`buffer`缓存下来的话，肯定会`OOM`，怎么解决呢？

```objective-c
//我们所接触到的视频，大部分的视频帧数在"26-35"帧之间，差不多就是1秒30帧，我们按照每10帧创建一个TimeRange，这样的话，一分钟的视频就会有180个TimeRange，此时我们只缓存TimeRange和10个sample buffer。具体做法如下：
int count = 0
while((sample = [assetReaderVideoOutPut nextVideoSample])) {
  
    // 获取该帧的时间信息
    presentationTime = CMSampleBufferGetPresentationTimeStamp(sample);
    NSValue *presentationValue = [NSValue valueWithBytes:&presentationTime objCType:@encode(CMTime)];
    [self.sampleTimeArray addObject:presentationValue];
    
    CFRelease(sample);
    sample = NULL;

    if (count == 0) {
        count++;
    }
    else if (processIndex == 9) {
        endTime = presentationTime;
				
        CMTimeRange timeRange = CMTimeRangeMake(startTime, CMTimeSubtract(endTime, startTime));
        NSValue *timeRangeValue = [NSValue valueWithCMTimeRange:timeRange];
        [self.clipTimeRangeArray addObject:timeRangeValue];
        
        // reset
        count = 0;
        startTime = presentationTime;
        endTime = kCMTimeZero;
    } 
}
```

拿到了时间片段后，做如下处理：

```objective-c
[self.clipTimeRangeArray enumerateObjectsWithOptions:NSEnumerationReverse usingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {

    CMSampleBufferRef buffer;
    // 设置了当前读取的timeRange
    [_readerTrackOutput_video resetForReadingTimeRanges:@[obj]];
    
    // 获取10个帧缓存
    while ((buffer = [self nextVideoSample])) {
        [bufferCaches addObject:(__bridge id _Nonnull)(buffer)];
        [ptsCaches addObject:self.sampleTimeArray[index]];
        index++;
    }
    
    // 倒序遍历帧缓存
    [bufferCaches enumerateObjectsWithOptions:NSEnumerationReverse usingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        CMTime pts = ptsCaches[ptsCaches.count - idx - 1].CMTimeValue;
				CVImageBufferRef CVPixelBuffer = CMSampleBufferGetImageBuffer(buffer);
        if (assetWriterVideoInput.readyForMoreMediaData && buffer != NULL) {
              // 追加buffer
              [_adaptor appendPixelBuffer:buffer withPresentationTime:pts];
              buffer = NULL;
        }
    }];

    // 释放
    [bufferCaches removeAllObjects];
    [ptsCaches removeAllObjects];
}];
```











