## AVVideoCompositionInstruction

-----------

`AVVideoComposition`是由一组 `AVVideoCompositionInstruction`对象格式定义的指令组成的。这个对象所提供的最关键的一段数据是组合对象时间轴内的时间范围信息，这一时间范围是在某一组合形式出现时的时间范文，要执行的组合特质是通过其 `layerInstructions`集合定义的。

首先，有一个协议叫做`AVVideoCompositionInstruction`，还有一个类叫做`AVVideoCompositionInstruction`



#### 协议`AVVideoCompositionInstruction`

-----------

```objective-c
@protocol AVVideoCompositionInstruction<NSObject>

@required

/* Indicates the timeRange during which the instruction is effective. Note requirements for the timeRanges of instructions described in connection with AVVideoComposition's instructions key above. */
@property (nonatomic, readonly) CMTimeRange timeRange;

/* If NO, indicates that post-processing should be skipped for the duration of this instruction.
   See +[AVVideoCompositionCoreAnimationTool videoCompositionToolWithPostProcessingAsVideoLayer:inLayer:].*/
@property (nonatomic, readonly) BOOL enablePostProcessing;

/* If YES, rendering a frame from the same source buffers and the same composition instruction at 2 different
   compositionTime may yield different output frames. If NO, 2 such compositions would yield the
   same frame. The media pipeline may me able to avoid some duplicate processing when containsTweening is NO */
@property (nonatomic, readonly) BOOL containsTweening;

/* List of video track IDs required to compose frames for this instruction. If the value of this property is nil, all source tracks will be considered required for composition */
@property (nonatomic, readonly, nullable) NSArray<NSValue *> *requiredSourceTrackIDs;

/* If for the duration of the instruction, the video composition result is one of the source frames, this property should
   return the corresponding track ID. The compositor won't be run for the duration of the instruction and the proper source
   frame will be used instead. The dimensions, clean aperture and pixel aspect ratio of the source buffer will be
   matched to the required values automatically */
@property (nonatomic, readonly) CMPersistentTrackID passthroughTrackID; // kCMPersistentTrackID_Invalid if not a passthrough instruction

@end
```



#### 类AVVideoCompositionInstruction

-------

```objc
@class AVVideoCompositionInstructionInternal;

API_AVAILABLE(macos(10.7), ios(4.0), tvos(9.0)) API_UNAVAILABLE(watchos)
@interface AVVideoCompositionInstruction : NSObject <NSSecureCoding, NSCopying, NSMutableCopying, AVVideoCompositionInstruction> {
@private
	AVVideoCompositionInstructionInternal	*_instruction;
}

/* Indicates the timeRange during which the instruction is effective. Note requirements for the timeRanges of instructions described in connection with AVVideoComposition's instructions key above. */
@property (nonatomic, readonly) CMTimeRange timeRange;

/* Indicates the background color of the composition. Solid BGRA colors only are supported; patterns and other color refs that are not supported will be ignored.
   If the background color is not specified the video compositor will use a default backgroundColor of opaque black.
   If the rendered pixel buffer does not have alpha, the alpha value of the backgroundColor will be ignored. */
@property (nonatomic, readonly, retain, nullable) __attribute__((NSObject)) CGColorRef backgroundColor;

/* Provides an array of instances of AVVideoCompositionLayerInstruction that specify how video frames from source tracks should be layered and composed.
   Tracks are layered in the composition according to the top-to-bottom order of the layerInstructions array; the track with trackID of the first instruction
   in the array will be layered on top, with the track with the trackID of the second instruction immediately underneath, etc.
   If this key is nil, the output will be a fill of the background color. */
@property (nonatomic, readonly, copy) NSArray<AVVideoCompositionLayerInstruction *> *layerInstructions;

/* If NO, indicates that post-processing should be skipped for the duration of this instruction.  YES by default.
   See +[AVVideoCompositionCoreAnimationTool videoCompositionToolWithPostProcessingAsVideoLayer:inLayer:].*/
@property (nonatomic, readonly) BOOL enablePostProcessing;

/* List of video track IDs required to compose frames for this instruction. The value of this property is computed from the layer instructions. */
@property (nonatomic, readonly) NSArray<NSValue *> *requiredSourceTrackIDs API_AVAILABLE(macos(10.9), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

/* If the video composition result is one of the source frames for the duration of the instruction, this property
   returns the corresponding track ID. The compositor won't be run for the duration of the instruction and the proper source
   frame will be used instead. The value of this property is computed from the layer instructions */
@property (nonatomic, readonly) CMPersistentTrackID passthroughTrackID API_AVAILABLE(macos(10.9), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos); // kCMPersistentTrackID_Invalid if not a passthrough instruction

@end
```



#### 类AVMutableVideoCompositionInstruction

-----

```objective-c
@class AVMutableVideoCompositionInstructionInternal;

API_AVAILABLE(macos(10.7), ios(4.0), tvos(9.0)) API_UNAVAILABLE(watchos)
@interface AVMutableVideoCompositionInstruction : AVVideoCompositionInstruction {
@private
	AVMutableVideoCompositionInstructionInternal	*_mutableInstruction __attribute__((unused));
}

/*  
 @method		videoCompositionInstruction
 @abstract		Returns a new instance of AVMutableVideoCompositionInstruction.
 @discussion
   The returned AVMutableVideoCompositionInstruction will have a timeRange of kCMTimeRangeInvalid, a NULL backgroundColor, and a nil array of layerInstructions.
*/
+ (instancetype)videoCompositionInstruction;

/* Indicates the timeRange during which the instruction is effective. Note requirements for the timeRanges of instructions described in connection with AVVideoComposition's instructions key above. */
@property (nonatomic, assign) CMTimeRange timeRange;

/* Indicates the background color of the composition. Solid BGRA colors only are supported; patterns and other color refs that are not supported will be ignored.
   If the background color is not specified the video compositor will use a default backgroundColor of opaque black.
   If the rendered pixel buffer does not have alpha, the alpha value of the backgroundColor will be ignored. */
@property (nonatomic, retain, nullable) __attribute__((NSObject)) CGColorRef backgroundColor;

/* Provides an array of instances of AVVideoCompositionLayerInstruction that specify how video frames from source tracks should be layered and composed.
   Tracks are layered in the composition according to the top-to-bottom order of the layerInstructions array; the track with trackID of the first instruction
   in the array will be layered on top, with the track with the trackID of the second instruction immediately underneath, etc.
   If this key is nil, the output will be a fill of the background color. */
@property (nonatomic, copy) NSArray<AVVideoCompositionLayerInstruction *> *layerInstructions;

/* If NO, indicates that post-processing should be skipped for the duration of this instruction.  YES by default.
   See +[AVVideoCompositionCoreAnimationTool videoCompositionToolWithPostProcessingAsVideoLayer:inLayer:].*/
@property (nonatomic, assign) BOOL enablePostProcessing;

@end
```

