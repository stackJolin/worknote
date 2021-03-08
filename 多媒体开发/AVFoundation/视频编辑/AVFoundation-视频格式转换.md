## AVFoundation - 视频格式转换

-------

这里可以利用`AVAssetReader`和`AVAssetWriter`实现视频格式的转换，比如`.mp4`转`.m4a`

- 初始化`AVAssetReader`、`AVAssetReaderOutput-video`和`AVAssetReaderOutput-audio`

- 初始化`AVAssetWriter`、`AVAssetWriterInput-video`和`AVAssetWriterInput-audio`

- `[reader startReader]`、`[writer startWriting]`

- `AVAssetWriterInput-video`主动拉取视频轨道数据、`AVAssetWriterInput-audio`主动拉取音频轨道数据

  ```objective-c
  [videoTrackInput requestMediaDataWhenReadyOnQueue:videoQueue usingBlock:^{
      while ([videoTrackInput isReadyForMoreMediaData] && !self.videoFinished) {
          CMSampleBufferRef sampleBuffer = [videoTrackOutput copyNextSampleBuffer];
          [videoTrackInput appendSampleBuffer:sampleBuffer];
      }
  }];
  
  [audioTrackInput requestMediaDataWhenReadyOnQueue:audioQueue usingBlock:^{
      while ([self.assetWriterAudioInput isReadyForMoreMediaData] && !completedOrFailed) {
          CMSampleBufferRef sampleBuffer = [self.assetReaderAudioOutput copyNextSampleBuffer];
  				[audioTrackInput appendSampleBuffer:sampleBuffer];
      }
  }];
  ```

- 导出完成