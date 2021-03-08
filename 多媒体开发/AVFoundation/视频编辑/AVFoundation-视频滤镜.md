## AVFoundation - 视频滤镜

-----

首先介绍一下实现原理：

首先，我们使播放视频的时候，首先有一个`AVPlayerItem`，而`AVPlayerItem`有下面一个属性:

```objective-c
/*!
 @property videoComposition
 @abstract Indicates the video composition settings to be applied during playback.
 */
// 视频合成设置，如果设置了这个合成，播放的时候，就会应用这个合成效果
@property (nonatomic, copy, nullable) AVVideoComposition *videoComposition;
```

而`AVVideoComposition`有一个很关键的属性：视频合成器类，如下所示:

```objective-c
/* indicates the custom compositor class to use. If nil, the default, internal video compositor is used */
// 如果设置了这个合成器类，那就这个合成就使用这个合成器类。如果是nil的话，就使用默认的合成器 
@property (nonatomic, retain, nullable) Class<AVVideoCompositing> customVideoCompositorClass
```

所以，需要我们自己定义实现了`AVVideoCompositing`协议的类，关于`AVVideoCompositing`的主要内容，后面介绍，我们继续介绍流程

目前为止，我们知道了，首先我们要有一个`AVVideoCompistion`，然后这个设置这个`AVVideoCompostion`的`customVideoCompositorClass`的属性，为我们自定义的类。那么怎么创建一个`AVVideoComposition`呢？创建如下:

```objective-c
// 通过AVAsset构造合成
AVMutableVideoComposition *videoComposition = [AVMutableVideoComposition videoCompositionWithPropertiesOfAsset:asset];
```

但是上面的这个合成，他的指令属性`instructions`中的每一个属性还是默认的`AVVideoCompositionLayerInstruction`

```objective-c
// AVMutableVideoComposition的合成指令
@property (nonatomic, copy) NSArray<id <AVVideoCompositionInstruction>> *instructions;
// 类AVVideoCompositionInstruction的合成图层指令
@property (nonatomic, readonly, copy) NSArray<AVVideoCompositionLayerInstruction *> *layerInstructions;
```



###### 关于滤镜的处理：

当我们拿到了`CVPixelBufferRef`后，需要做一些处理，然后返回处理后的`CVPixelBufferRef`，处理这些的方式比较好的有：

- OpenGL ES
- CIImage
- Metal
- GPUImage
- OpenCV

后面会介绍这些方案



#### AVVideoCompositing协议详解

--------





```objective-c
@protocol AVVideoCompositing<NSObject>

@required

/* Indicates the kinds of source frame pixel buffer attributes a video compositor can accept as input.
   The property is required to provide kCVPixelBufferPixelFormatTypeKey along with the attributes
   for which the compositor needs specific values to work properly. If the attribute kCVPixelBufferPixelFormatTypeKey
   is missing an exception will be raised. If the custom compositor is meant to be used with an AVVideoCompositionCoreAnimationTool
   created using the videoCompositionCoreAnimationToolWithAdditionalLayer constructor, kCVPixelFormatType_32BGRA 
   should be indicated as one of the supported pixel format types.
   Missing attributes will be set by the composition engine to values allowing the best performance.
   This property is queried once before any composition request is sent to the compositor. Changing
   source buffer attributes afterwards is not supported.
*/
@property (nonatomic, readonly, nullable) NSDictionary<NSString *, id> *sourcePixelBufferAttributes;

/* Indicates the pixel buffer attributes required by the video compositor for new buffers that it creates
 for processing. The property is required to provide kCVPixelBufferPixelFormatTypeKey along with attributes for which the compositor needs specific values to work properly. Omitted attributes will be supplied by the composition engine to allow for the best performance. If the attribute kCVPixelBufferPixelFormatTypeKey is missing an exception will be raised.
 The getter for requiredPixelBufferAttributesForRenderContext is typically invoked prior to the creation of
 a new render context; the combination of the attributes in the returned value and the additional attributes
 supplied by the composition engine will be used in the creation of subsequent render context's pixelBuffers.
 This property is queried once before any composition request is sent to the compositor. Changing
 required buffer attributes afterwards is not supported.
 */
@property (nonatomic, readonly) NSDictionary<NSString *, id> *requiredPixelBufferAttributesForRenderContext;

/*!
    @method			renderContextChanged:
	@abstract       Called to notify the custom compositor that a composition will switch to a different render context
	@param			newRenderContext
					The render context that will be handling the video composition from this point
    @discussion
					Instances of classes implementing the AVVideoComposting protocol can implement this method to be notified when
					the AVVideoCompositionRenderContext instance handing a video composition changes. AVVideoCompositionRenderContext instances
					being immutable, such a change will occur every time there is a change in the video composition parameters.
*/
- (void)renderContextChanged:(AVVideoCompositionRenderContext *)newRenderContext;

/*!
	@method			startVideoCompositionRequest:
	@abstract		Directs a custom video compositor object to create a new pixel buffer composed asynchronously from a collection of sources.
	@param			asyncVideoCompositionRequest
    				An instance of AVAsynchronousVideoCompositionRequest that provides context for the requested composition.
	@discussion
		The custom compositor is expected to invoke, either subsequently or immediately, either:
		-[AVAsynchronousVideoCompositionRequest finishWithComposedVideoFrame:] or
		-[AVAsynchronousVideoCompositionRequest finishWithError:]. If you intend to finish rendering the frame after your
		handling of this message returns, you must retain the instance of AVAsynchronousVideoCompositionRequest until after composition is finished.
		Note that if the custom compositor's implementation of -startVideoCompositionRequest: returns without finishing the composition immediately,
		it may be invoked again with another composition request before the prior request is finished; therefore in such cases the custom compositor should
		be prepared to manage multiple composition requests.

		If the rendered frame is exactly the same as one of the source frames, with no letterboxing, pillboxing or cropping needed,
		then the appropriate source pixel buffer may be returned (after CFRetain has been called on it).
*/
- (void)startVideoCompositionRequest:(AVAsynchronousVideoCompositionRequest *)asyncVideoCompositionRequest;

@optional

/*!
	@method			cancelAllPendingVideoCompositionRequests	
	@abstract		Directs a custom video compositor object to cancel or finish all pending video composition requests
	@discussion
		When receiving this message, a custom video compositor must block until it has either cancelled all pending frame requests,
		and called the finishCancelledRequest callback for each of them, or, if cancellation is not possible, finished processing of all the frames
		and called the finishWithComposedVideoFrame: callback for each of them.
*/
- (void)cancelAllPendingVideoCompositionRequests;

/*!
 @property supportsWideColorSourceFrames
 @abstract
	Indicates that clients can handle frames that contains wide color properties.
 
 @discussion 
	Controls whether the client will receive frames that contain wide color information. Care should be taken to avoid clamping.
 */
@property (nonatomic, readonly) BOOL supportsWideColorSourceFrames API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0)) API_UNAVAILABLE(watchos);

/*!
 @property supportsHDRSourceFrames
 @abstract
	Indicates that the client's video compositor can handle frames that contain high dynamic range (HDR) properties.
 
 @discussion
	Controls whether the client will receive frames that contain HDR information.
	If this field is omitted or set to NO, the framework will convert HDR frames to standard dynamic range (SDR) with BT.709 transfer function before sending to the client.
	If this field is set to YES, the value of supportsWideColorSourceFrames will be ignored and assumed to be YES.
 */
@property (nonatomic, readonly) BOOL supportsHDRSourceFrames API_AVAILABLE(macos(11.0), ios(14.0), tvos(14.0)) API_UNAVAILABLE(watchos);

/*!
	@method			anticipateRenderingUsingHint:
	@abstract		Informs a custom video compositor about upcoming rendering requests.
	@param			renderHint
					Information about the upcoming composition requests.
	@discussion
		In the method the compositor can load composition resources such as overlay images which will be needed in the anticipated rendering time range.

		Unlike -startVideoCompositionRequest, which is invoked only when the frame compositing is necessary, the framework typically calls this method every frame duration. It allows the custom compositor to load and unload a composition resource such as overlay images at an appropriate timing.

		In forward playback, renderHint's startCompositionTime is less than endCompositionTime. In reverse playback, its endCompositionTime is less than startCompositionTime. For seeking, startCompositionTime == endCompositionTime, which means the upcoming composition request time range is unknown and the compositor shouldn’t preload time associated composition resources eagerly.

		The method is guaranteed to be called before -startVideoCompositionRequest: for a given composition time.

		The method is synchronous. The implementation should return quickly because otherwise the playback would stall and cause frame drops.
*/
- (void)anticipateRenderingUsingHint:(AVVideoCompositionRenderHint *)renderHint API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0)) API_UNAVAILABLE(watchos);

/*!
	@method			prerollForRenderingUsingHint:
	@abstract		Tell a custom video compositor to perform any work in prerolling phase.
	@param			renderHint
					Information about the upcoming composition requests.
	@discussion
		The framework may perform prerolling to load media data to prime the render pipelines for smoother playback. This method is called in the prerolling phase so that the compositor can load composition resources such as overlay images which will be needed as soon as the playback starts.

		Not all rendering scenarios use prerolling. For example, the method won't be called while seeking.

		If called, the method is guaranteed to be invoked before the first -startVideoCompositionRequest: call.

		The method is synchronous. The prerolling won't finish until the method returns.
*/
-(void)prerollForRenderingUsingHint:(AVVideoCompositionRenderHint *)renderHint API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0)) API_UNAVAILABLE(watchos);

@end
```







#### 滤镜的实现方式一：用OpenGL来提供CoreImage绘制滤镜的性能

-------

首先，用`CPU`来绘制一个`CGImage`是非常耗时的操作，我们希望的是能够在屏幕上实时展示应用滤镜后的效果，幸运的是，`OpenGL`和`Core Image`的互操作性，可以使得我们这么做。

首先，要在`OpenGLES上下文`和`CoreImage上下文`共享资源，我们需要毅哥稍微不同的方式来创建我们的`CIContext`：

```objective-c
NSDictionary *op = {
                    kCIContextOutputColorSpace : [NSNull null],
                    kCIContextWorkingColorSpace : [NSNull null],
                    kCIContextUseSoftwareRenderer : @(NO)
}
EAGLContext *eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRederingAPIOpenGLES2];
CIContext *ciContext = [CIContext contextWithEAGLContext:eaglContext options:]
```

通过这种方式，`ciContext`处理后的图像，就可以使用`OpenGL`来绘制了，比如：

```objective-c

```



#### 滤镜的实现方式二：GPUImage库

-------















