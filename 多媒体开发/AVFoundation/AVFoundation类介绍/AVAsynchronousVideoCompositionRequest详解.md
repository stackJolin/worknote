## AVAsynchronousVideoCompositionRequest详解

---------

`API介绍`

```objective-c
@class AVAsynchronousVideoCompositionRequestInternal;

API_AVAILABLE(macos(10.9), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos)
@interface AVAsynchronousVideoCompositionRequest : NSObject <NSCopying> {
@private
	AVAsynchronousVideoCompositionRequestInternal *_internal;
}

/* The AVVideoCompositionRenderContext making the request */
@property (nonatomic, readonly) AVVideoCompositionRenderContext *renderContext;

/* The time for which the frame should be composed */
@property (nonatomic, readonly) CMTime compositionTime;

/* Track ID of all the source buffers that are available to compose the frame. */
// 所有用于组合这一帧的可用的原始buffer对应的轨道ID
@property (nonatomic, readonly) NSArray<NSNumber *> *sourceTrackIDs;

/* The AVVideoCompositionInstruction to use to compose the frame. */
// 组合这一帧的组合指令
@property (nonatomic, readonly) id<AVVideoCompositionInstruction> videoCompositionInstruction;

// 从给定的轨道ID获取原始的帧数据
- (nullable CVPixelBufferRef)sourceFrameByTrackID:(CMPersistentTrackID)trackID;

/* callback the custom compositor should call when composition succeeded */
// 组合完成的时候调用
- (void)finishWithComposedVideoFrame:(CVPixelBufferRef)composedVideoFrame;

/* callback the custom compositor should call when composition failed. The error parameter should describe the actual error. */
- (void)finishWithError:(NSError *)error;

/* callback the custom compositor should call for a request that has been cancelled. */
- (void)finishCancelledRequest;

@end
```

