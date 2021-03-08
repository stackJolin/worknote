## AVVideoCompositionLayerInstruction介绍

-----

用于定义对给定视频轨道应用的模糊、变形和裁剪效果。

它提供了一些方法用于在特定的时间点火灾一个时间范围内对这些值进修修改。在一段时间内对这些值应用渐变操作可以让开发者创建出动态的过渡效果，比如溶解和渐淡效果。

与所有AVFoundation媒体编辑类一样，视频组合API具有不可变和可变两种形式。不可变超类形式使用于客户端对象，比如AVAssetExportSession，不过当创建自己的视频组合应用程序时，应该使用不可变子类。



#### AVVideoCompositionLayerInstruction

```objc
@class AVVideoCompositionLayerInstructionInternal;

API_AVAILABLE(macos(10.7), ios(4.0), tvos(9.0)) API_UNAVAILABLE(watchos)
@interface AVVideoCompositionLayerInstruction : NSObject <NSSecureCoding, NSCopying, NSMutableCopying> {
@private
	AVVideoCompositionLayerInstructionInternal	*_layerInstruction;
};

/* Indicates the trackID of the source track to which the compositor will apply the instruction. */
@property (nonatomic, readonly, assign) CMPersistentTrackID trackID;

/*  
 @method		getTransformRampForTime:startTransform:endTransform:timeRange:
 @abstract		Obtains the transform ramp that includes the specified time.
 @param			time
   If a ramp with a timeRange that contains the specified time has been set, information about the effective ramp for that time is supplied.
   Otherwise, information about the first ramp that starts after the specified time is supplied.
 @param			startTransform
   A pointer to a float to receive the starting transform value for the transform ramp. May be NULL.
 @param			endTransform
   A pointer to a float to receive the ending transform value for the transform ramp. May be NULL.
 @param			timeRange
   A pointer to a CMTimeRange to receive the timeRange of the transform ramp. May be NULL.
 @result
   An indication of success. NO will be returned if the specified time is beyond the duration of the last transform ramp that has been set.
*/
- (BOOL)getTransformRampForTime:(CMTime)time startTransform:(nullable CGAffineTransform *)startTransform endTransform:(nullable CGAffineTransform *)endTransform timeRange:(nullable CMTimeRange *)timeRange;

/*  
 @method		getOpacityRampForTime:startOpacity:endOpacity:timeRange:
 @abstract		Obtains the opacity ramp that includes the specified time.
 @param			time
   If a ramp with a timeRange that contains the specified time has been set, information about the effective ramp for that time is supplied.
   Otherwise, information about the first ramp that starts after the specified time is supplied.
 @param			startOpacity
   A pointer to a float to receive the starting opacity value for the opacity ramp. May be NULL.
 @param			endOpacity
   A pointer to a float to receive the ending opacity value for the opacity ramp. May be NULL.
 @param			timeRange
   A pointer to a CMTimeRange to receive the timeRange of the opacity ramp. May be NULL.
 @result
   An indication of success. NO will be returned if the specified time is beyond the duration of the last opacity ramp that has been set.
*/
- (BOOL)getOpacityRampForTime:(CMTime)time startOpacity:(nullable float *)startOpacity endOpacity:(nullable float *)endOpacity timeRange:(nullable CMTimeRange *)timeRange;

/*  
 @method		getCropRectangleRampForTime:startCropRectangle:endCropRectangle:timeRange:
 @abstract		Obtains the crop rectangle ramp that includes the specified time.
 @param			time
   If a ramp with a timeRange that contains the specified time has been set, information about the effective ramp for that time is supplied.
   Otherwise, information about the first ramp that starts after the specified time is supplied.
 @param			startCropRectangle
   A pointer to a CGRect to receive the starting crop rectangle value for the crop rectangle ramp. May be NULL.
 @param			endCropRecrangle
   A pointer to a CGRect to receive the ending crop rectangle value for the crop rectangle ramp. May be NULL.
 @param			timeRange
   A pointer to a CMTimeRange to receive the timeRange of the crop rectangle ramp. May be NULL.
 @result
   An indication of success. NO will be returned if the specified time is beyond the duration of the last crop rectangle ramp that has been set.
*/
- (BOOL)getCropRectangleRampForTime:(CMTime)time startCropRectangle:(nullable CGRect *)startCropRectangle endCropRectangle:(nullable CGRect *)endCropRectangle timeRange:(nullable CMTimeRange *)timeRange API_AVAILABLE(macos(10.9), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

@end
```



#### AVMutableVideoCompositionLayerInstruction

-------

```objc
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

/*!
	@class		AVVideoCompositionLayerInstruction
 
	@abstract	An AVVideoCompositionLayerInstruction object represents the transform, opacity, and cropping ramps to apply to a given track.
*/

@class AVVideoCompositionLayerInstructionInternal;

API_AVAILABLE(macos(10.7), ios(4.0), tvos(9.0)) API_UNAVAILABLE(watchos)
@interface AVVideoCompositionLayerInstruction : NSObject <NSSecureCoding, NSCopying, NSMutableCopying> {
@private
	AVVideoCompositionLayerInstructionInternal	*_layerInstruction;
};

/* Indicates the trackID of the source track to which the compositor will apply the instruction. */
@property (nonatomic, readonly, assign) CMPersistentTrackID trackID;

/*  
 @method		getTransformRampForTime:startTransform:endTransform:timeRange:
 @abstract		Obtains the transform ramp that includes the specified time.
 @param			time
   If a ramp with a timeRange that contains the specified time has been set, information about the effective ramp for that time is supplied.
   Otherwise, information about the first ramp that starts after the specified time is supplied.
 @param			startTransform
   A pointer to a float to receive the starting transform value for the transform ramp. May be NULL.
 @param			endTransform
   A pointer to a float to receive the ending transform value for the transform ramp. May be NULL.
 @param			timeRange
   A pointer to a CMTimeRange to receive the timeRange of the transform ramp. May be NULL.
 @result
   An indication of success. NO will be returned if the specified time is beyond the duration of the last transform ramp that has been set.
*/
- (BOOL)getTransformRampForTime:(CMTime)time startTransform:(nullable CGAffineTransform *)startTransform endTransform:(nullable CGAffineTransform *)endTransform timeRange:(nullable CMTimeRange *)timeRange;

/*  
 @method		getOpacityRampForTime:startOpacity:endOpacity:timeRange:
 @abstract		Obtains the opacity ramp that includes the specified time.
 @param			time
   If a ramp with a timeRange that contains the specified time has been set, information about the effective ramp for that time is supplied.
   Otherwise, information about the first ramp that starts after the specified time is supplied.
 @param			startOpacity
   A pointer to a float to receive the starting opacity value for the opacity ramp. May be NULL.
 @param			endOpacity
   A pointer to a float to receive the ending opacity value for the opacity ramp. May be NULL.
 @param			timeRange
   A pointer to a CMTimeRange to receive the timeRange of the opacity ramp. May be NULL.
 @result
   An indication of success. NO will be returned if the specified time is beyond the duration of the last opacity ramp that has been set.
*/
- (BOOL)getOpacityRampForTime:(CMTime)time startOpacity:(nullable float *)startOpacity endOpacity:(nullable float *)endOpacity timeRange:(nullable CMTimeRange *)timeRange;

/*  
 @method		getCropRectangleRampForTime:startCropRectangle:endCropRectangle:timeRange:
 @abstract		Obtains the crop rectangle ramp that includes the specified time.
 @param			time
   If a ramp with a timeRange that contains the specified time has been set, information about the effective ramp for that time is supplied.
   Otherwise, information about the first ramp that starts after the specified time is supplied.
 @param			startCropRectangle
   A pointer to a CGRect to receive the starting crop rectangle value for the crop rectangle ramp. May be NULL.
 @param			endCropRecrangle
   A pointer to a CGRect to receive the ending crop rectangle value for the crop rectangle ramp. May be NULL.
 @param			timeRange
   A pointer to a CMTimeRange to receive the timeRange of the crop rectangle ramp. May be NULL.
 @result
   An indication of success. NO will be returned if the specified time is beyond the duration of the last crop rectangle ramp that has been set.
*/
- (BOOL)getCropRectangleRampForTime:(CMTime)time startCropRectangle:(nullable CGRect *)startCropRectangle endCropRectangle:(nullable CGRect *)endCropRectangle timeRange:(nullable CMTimeRange *)timeRange API_AVAILABLE(macos(10.9), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

@end

/*!
	@class		AVMutableVideoCompositionLayerInstruction
 
	@abstract	AVMutableVideoCompositionLayerInstruction is a mutable subclass of AVVideoCompositionLayerInstruction that is used to modify the transform, cropping, and opacity ramps to apply to a given track in a composition.
*/

@class AVMutableVideoCompositionLayerInstructionInternal;

API_AVAILABLE(macos(10.7), ios(4.0), tvos(9.0)) API_UNAVAILABLE(watchos)
@interface AVMutableVideoCompositionLayerInstruction : AVVideoCompositionLayerInstruction {
@private
	AVMutableVideoCompositionLayerInstructionInternal	*_mutableLayerInstruction __attribute__((unused));
};

/*  
 @method		videoCompositionLayerInstructionWithAssetTrack:
 @abstract		Returns a new instance of AVMutableVideoCompositionLayerInstruction with no transform or opacity ramps and a trackID set to the specified track's trackID.
 @param			track
   A reference to an AVAssetTrack.
*/
+ (instancetype)videoCompositionLayerInstructionWithAssetTrack:(AVAssetTrack *)track;

/*  
 @method		videoCompositionLayerInstruction
 @abstract		Returns a new instance of AVMutableVideoCompositionLayerInstruction with no transform or opacity ramps and a trackID initialized to kCMPersistentTrackID_Invalid.
*/
+ (instancetype)videoCompositionLayerInstruction;

/* Indicates the trackID of the source track to which the compositor will apply the instruction. */
@property (nonatomic, assign) CMPersistentTrackID trackID;

/*  
 @method		setTransformRampFromStartTransform:toEndTransform:timeRange:
 @abstract		Sets a transform ramp to apply during the specified timerange.
 @param			startTransform
   The transform to be applied at the starting time of the timeRange. See the discussion below of how transforms are applied to video frames.
 @param			endTransform
   The transform to be applied at the end time of the timeRange.
 @param			timeRange
   The timeRange over which the value of the transform will be interpolated between startTransform and endTransform.
 @discussion
   For purposes of spatial positioning of video frames, the origin is in the top-left corner, so
   (a) positive translation values in an affine transform move a video frame right and down; and
   (b) with an identity transform a video frame is positioned with its top-left corner in the top-left corner of the composited frame.
   Video frames shall be interpreted at their display sizes (as described by CVImageBufferGetDisplaySize,
   ie, taking pixel aspect ratio attachments into account) before any affine transform is applied.

   During a transform ramp, the affine transform is interpolated between the values set at the ramp's start time and end time.
   Before the first specified time for which a transform is set, the affine transform is held constant at the value of CGAffineTransformIdentity;
   after the last time for which a transform is set, the affine transform is held constant at that last value;
*/
- (void)setTransformRampFromStartTransform:(CGAffineTransform)startTransform toEndTransform:(CGAffineTransform)endTransform timeRange:(CMTimeRange)timeRange;

/*  
 @method		setTransform:atTime:
 @abstract		Sets a value of the transform at a time within the timeRange of the instruction.
 @param			transform
   The transform to be applied at the specified time. See the discussion below of how transforms are applied to video frames.
 @param			time
   A time value within the timeRange of the composition instruction.
 @discussion
   For purposes of spatial positioning of video frames, the origin is in the top-left corner, so
   (a) positive translation values in an affine transform move a video frame right and down; and
   (b) with an identity transform a video frame is positioned with its top-left corner in the top-left corner of the composited frame.
   Video frames shall be interpreted at their display sizes (as described by CVImageBufferGetDisplaySize,
   ie, taking pixel aspect ratio attachments into account) before any affine transform is applied.

   Sets a fixed transform to apply from the specified time until the next time at which a transform is set; this is the same as setting a flat ramp for that time range.
   Before the first specified time for which a transform is set, the affine transform is held constant at the value of CGAffineTransformIdentity;
   after the last time for which a transform is set, the affine transform is held constant at that last value;
*/
- (void)setTransform:(CGAffineTransform)transform atTime:(CMTime)time;

/*  
 @method		setOpacityRampFromStartOpacity:toEndOpacity:timeRange:
 @abstract		Sets an opacity ramp to apply during the specified timerange.
 @param			startOpacity
   The opacity to be applied at the starting time of the timeRange. The value must be between 0.0 and 1.0.
 @param			endOpacity
   The opacity to be applied at the end time of the timeRange. The value must be between 0.0 and 1.0.
 @param			timeRange
   The timeRange over which the value of the opacity will be interpolated between startOpacity and endOpacity.
 @discussion
   During an opacity ramp, opacity is computed using a linear interpolation.
   Before the first time for which an opacity is set, the opacity is held constant at 1.0; after the last specified time, the opacity is held constant at the last value.
*/
- (void)setOpacityRampFromStartOpacity:(float)startOpacity toEndOpacity:(float)endOpacity timeRange:(CMTimeRange)timeRange;

/*  
 @method		setOpacity:atTime:
 @abstract		Sets a value of the opacity at a time within the timeRange of the instruction.
 @param			opacity
   The opacity to be applied at the specified time. The value must be between 0.0 and 1.0.
 @param			time
   A time value within the timeRange of the composition instruction.
 @discussion
   Sets a fixed opacity to apply from the specified time until the next time at which an opacity is set; this is the same as setting a flat ramp for that time range.
   Before the first time for which an opacity is set, the opacity is held constant at 1.0; after the last specified time, the opacity is held constant at the last value.
*/
- (void)setOpacity:(float)opacity atTime:(CMTime)time;

/*  
 @method		setCropRectangleRampFromStartCropRectangle:toEndCropRectangle:timeRange:
 @abstract		Sets an crop rectangle ramp to apply during the specified timerange.
 @param			startCropRectangle
   The crop rectangle to be applied at the starting time of the timeRange. See the discussion below of how crop rectangles are applied to video frames.
 @param			endCropRectangle
   The crop rectangle to be applied at the end time of the timeRange.
 @param			timeRange
   The timeRange over which the value of the opacity will be interpolated between startCropRectangle and endCropRectangle.
 @discussion
   The origin of the crop rectangle is the top-left corner of the buffer clean aperture rectangle. The crop rectangle is defined in
   square pixel space, i.e. without taking the pixel aspect ratio into account. Crop rectangles extending outside of the clean aperture,
   are cropped to the clean aperture.

   During a crop rectangle ramp, the rectangle is interpolated between the values set at the ramp's start time and end time. 
   When the starting or ending rectangle is empty, interpolations take into account the origin and size of the empty rectangle.
   Before the first specified time for which a crop rectangle is set, the crop rectangle is held constant to CGRectInfinite
   after the last time for which a crop rectangle is set, the crop rectangle is held constant at that last value.
*/
- (void)setCropRectangleRampFromStartCropRectangle:(CGRect)startCropRectangle toEndCropRectangle:(CGRect)endCropRectangle timeRange:(CMTimeRange)timeRange API_AVAILABLE(macos(10.9), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

/*  
 @method		setCropRectangle:atTime:
 @abstract		Sets a value of the crop rectangle at a time within the timeRange of the instruction.
 @param			cropRectangle
   The crop rectangle to be applied at the specified time. See the discussion below of how crop rectangles are applied to video frames.
 @param			time
   A time value within the timeRange of the composition instruction.
 @discussion
   The origin of the crop rectangle is the top-left corner of the buffer clean aperture rectangle. The crop rectangle is defined in
   square pixel space, i.e. without taking the pixel aspect ratio into account. Crop rectangles extending outside of the clean aperture,
   are cropped to the clean aperture.

   Sets a fixed crop rectangle to apply from the specified time until the next time at which a crop rectangle is set; this is the same as setting a flat ramp for that time range.
   Before the first specified time for which a crop rectangle is set, the crop rectangle is held constant to CGRectInfinite
   after the last time for which a crop rectangle is set, the crop rectangle is held constant at that last value.
*/
- (void)setCropRectangle:(CGRect)cropRectangle atTime:(CMTime)time API_AVAILABLE(macos(10.9), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

@end
```

