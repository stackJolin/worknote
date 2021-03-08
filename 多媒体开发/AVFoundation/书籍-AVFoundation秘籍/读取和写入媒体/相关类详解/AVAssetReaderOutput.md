## AVAssetReaderOutput

------

`AVAssetReaderOutput`是一个协议，一共有三个子类：

- AVAssetReaderTrackOutput
- AVAssetReaderAudioMixOutput
- AVAssetReaderVideoCompositionOutput

我们先来看`AVAssetReaderOutput`

```objc
@interface AVAssetReaderOutput : NSObject {
    @private
	  AVAssetReaderOutputInternal	*_internal;
}

// 读取到的样本(samples)类型，比如视频、音频、字幕样本等
@property (nonatomic, readonly) AVMediaType mediaType;

// buffer传递给output的时候，是否要copy之后，在传递给客户端。默认是YES
// 一般情况下，比如说视频，当他没有其他的output时，可以设置为NO，这样不会copy数据，可以提升性能。但是如果有多个output的时候，需要注意场景，如果设置为NO，当一个output修改了这个buffer，其他的output也会受到影响
@property (nonatomic) BOOL alwaysCopiesSampleData;

// 同步copy当前轨道的下一个 sample buffer
- (nullable CMSampleBufferRef)copyNextSampleBuffer;

@end
```



#### AVAssetReaderTrackOutput

-----

这个子类的作用，就是可以读取`AVAssetReader`的`assets`资源的某一个`track`，并且，可以自定义一些配置，获取到想要的 sample buffer

```objc
@interface AVAssetReaderTrackOutput : AVAssetReaderOutput {
		@private
		AVAssetReaderTrackOutputInternal	*_trackOutputInternal;
}

// 初始化。AVAssetReaderTrackOutput`实例
+ (instancetype)assetReaderTrackOutputWithTrack:(AVAssetTrack *)track outputSettings:(nullable NSDictionary<NSString *, id> *)outputSettings;
- (instancetype)initWithTrack:(AVAssetTrack *)track outputSettings:(nullable NSDictionary<NSString *, id> *)outputSettings；

@property (nonatomic, readonly) AVAssetTrack *track;
@property (nonatomic, readonly, nullable) NSDictionary<NSString *, id> *outputSettings;

/*!
   用于管理 `缩放音频、音调的处理算法`。一共有四种:
		AVAudioTimePitchAlgorithmLowQualityZeroLatency：iOS默认方式，音频质量低，适合短暂的快进和倒带
		AVAudioTimePitchAlgorithmTimeDomain：音频质量适中，适合声音。可变率从 1/32 - 32。AVPlayer在变速播放的时候，可以使用
		AVAudioTimePitchAlgorithmSpectral：osx下默认，音频质量高，计算成本高。可变率 1/32 - 32
		AVAudioTimePitchAlgorithmVarispeed：音频质量高，没有音调矫正，音调随速度变化。可变率从1/32 - 32
*/
@property (nonatomic, copy) AVAudioTimePitchAlgorithm audioTimePitchAlgorithm;

@end
```



#### AVAssetReaderAudioMixOutput

------

作为`AVAssetReaderOutput`的又一个子类，他可以从`AVAssetReader`中的`AVAsset`实例中，读取多个音频轨道的混音后的样本

```objective-c
@interface AVAssetReaderAudioMixOutput : AVAssetReaderOutput {
    @private
	  AVAssetReaderAudioMixOutputInternal	*_audioMixOutputInternal;
}

// 初始化方法
+ (instancetype)assetReaderAudioMixOutputWithAudioTracks:(NSArray<AVAssetTrack *> *)audioTracks audioSettings:(nullable NSDictionary<NSString *, id> *)audioSettings;
- (instancetype)initWithAudioTracks:(NSArray<AVAssetTrack *> *)audioTracks audioSettings:(nullable NSDictionary<NSString *, id> *)audioSettings;

@property (nonatomic, readonly) NSArray<AVAssetTrack *> *audioTracks;
@property (nonatomic, readonly, nullable) NSDictionary<NSString *, id> *audioSettings;
// 此属性的值是AVAudioMix，可用于指定从每个源轨道读取的音频样本的音量在源资产的时间轴上如何变化。
@property (nonatomic, copy, nullable) AVAudioMix *audioMix;
@property (nonatomic, copy) AVAudioTimePitchAlgorithm audioTimePitchAlgorithm;

@end
```



#### AVAssetReaderVideoCompositionOutput

--------



```objc
/*! 
@class AVAssetReaderVideoCompositionOutput
 @abstract
	AVAssetReaderVideoCompositionOutput is a concrete subclass of AVAssetReaderOutput that defines an interface for reading video frames that have been composited together from the frames in one or more AVAssetTracks of an AVAssetReader's AVAsset.
 
 @discussion
	Clients can read the video frames composited from one or more asset tracks by adding an instance of AVAssetReaderVideoCompositionOutput to an AVAssetReader using the -[AVAssetReader addOutput:] method.
 */
API_AVAILABLE(macos(10.7), ios(4.1), tvos(9.0)) API_UNAVAILABLE(watchos)
@interface AVAssetReaderVideoCompositionOutput : AVAssetReaderOutput
{
@private
	AVAssetReaderVideoCompositionOutputInternal	*_videoCompositionOutputInternal;
}
AV_INIT_UNAVAILABLE

/*!
 @method assetReaderVideoCompositionOutputWithVideoTracks:videoSettings:
 @abstract
	Creates an instance of AVAssetReaderVideoCompositionOutput for reading composited video from the specified video tracks and supplying media data according to the specified video settings.

 @param tracks
	An NSArray of AVAssetTrack objects from which the resulting AVAssetReaderVideoCompositionOutput should read video frames for compositing.
 @param videoSettings
	An NSDictionary of video settings to be used for video output.  See AVVideoSettings.h for more information about how to construct a video settings dictionary.
 @result
	An instance of AVAssetReaderVideoCompositionOutput.

 @discussion
	Each track must be one of the tracks owned by the target AVAssetReader's asset and must be of media type AVMediaTypeVideo.
 	
	A value of nil for videoSettings configures the output to return samples in a convenient uncompressed format, with properties determined according to the properties of the specified video tracks.  Initialization will fail if the video settings cannot be used with the specified tracks.
	
	AVAssetReaderVideoCompositionOutput can only produce uncompressed output.  This means that the video settings dictionary must follow the rules for uncompressed video output, as laid out in AVVideoSettings.h.  In addition, the following keys are not supported:

		AVVideoCleanApertureKey
		AVVideoPixelAspectRatioKey
		AVVideoScalingModeKey
 */
+ (instancetype)assetReaderVideoCompositionOutputWithVideoTracks:(NSArray<AVAssetTrack *> *)videoTracks videoSettings:(nullable NSDictionary<NSString *, id> *)videoSettings;

/*!
 @method initWithVideoTracks:videoSettings:
 @abstract
	Creates an instance of AVAssetReaderVideoCompositionOutput for reading composited video from the specified video tracks and supplying media data according to the specified video settings.

 @param tracks
	An NSArray of AVAssetTrack objects from which the resulting AVAssetReaderVideoCompositionOutput should read video frames for compositing.
 @param videoSettings
	An NSDictionary of video settings to be used for video output.  See AVVideoSettings.h for more information about how to construct a video settings dictionary.
 @result An instance of AVAssetReaderVideoCompositionOutput.

 @discussion
	Each track must be one of the tracks owned by the target AVAssetReader's asset and must be of media type AVMediaTypeVideo.
 	
	A value of nil for videoSettings configures the output to return samples in a convenient uncompressed format, with properties determined according to the properties of the specified video tracks.  Initialization will fail if the video settings cannot be used with the specified tracks.
	
	AVAssetReaderVideoCompositionOutput can only produce uncompressed output.  This means that the video settings dictionary must follow the rules for uncompressed video output, as laid out in AVVideoSettings.h.  In addition, the following keys are not supported:
 
		AVVideoCleanApertureKey
		AVVideoPixelAspectRatioKey
		AVVideoScalingModeKey
 */
- (instancetype)initWithVideoTracks:(NSArray<AVAssetTrack *> *)videoTracks videoSettings:(nullable NSDictionary<NSString *, id> *)videoSettings NS_DESIGNATED_INITIALIZER;

/*!
 @property videoTracks
 @abstract
	The tracks from which the receiver reads composited video.

 @discussion
	The value of this property is an NSArray of AVAssetTracks owned by the target AVAssetReader's asset.
 */
@property (nonatomic, readonly) NSArray<AVAssetTrack *> *videoTracks;

/*!
 @property videoSettings
 @abstract
	The video settings used by the receiver.

 @discussion
	The value of this property is an NSDictionary that contains values for keys as specified by AVVideoSettings.h.  A value of nil indicates that the receiver will return video frames in a convenient uncompressed format, with properties determined according to the properties of the receiver's video tracks.
 */ 
@property (nonatomic, readonly, nullable) NSDictionary<NSString *, id> *videoSettings;

/*!
 @property videoComposition
 @abstract
	The composition of video used by the receiver.

 @discussion
	The value of this property is an AVVideoComposition that can be used to specify the visual arrangement of video frames read from each source track over the timeline of the source asset.
 
	This property cannot be set after reading has started.
 */
@property (nonatomic, copy, nullable) AVVideoComposition *videoComposition;

/*!
 @property customVideoCompositor
 @abstract
 	Indicates the custom video compositor instance used by the receiver.

 @discussion
 	This property is nil if there is no video compositor, or if the internal video compositor is in use.
 */
@property (nonatomic, readonly, nullable) id <AVVideoCompositing> customVideoCompositor API_AVAILABLE(macos(10.9), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

@end
```

