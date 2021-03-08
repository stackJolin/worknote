## AVAssetWriterInputPixelBufferAdaptor

-------

定义了一个适配器，该适配器将打包为`CVPixelBuffer`对象的样本添加到`AVAssetWriterInput`对象

```objective-c
@interface AVAssetWriterInputPixelBufferAdaptor : NSObject {
    @private
	  AVAssetWriterInputPixelBufferAdaptorInternal	*_internal;
}

// 初始化方法
+ (instancetype)assetWriterInputPixelBufferAdaptorWithAssetWriterInput:(AVAssetWriterInput *)input sourcePixelBufferAttributes:(nullable NSDictionary<NSString *, id> *)sourcePixelBufferAttributes;
- (instancetype)initWithAssetWriterInput:(AVAssetWriterInput *)input sourcePixelBufferAttributes:(nullable NSDictionary<NSString *, id> *)sourcePixelBufferAttributes;

@property (nonatomic, readonly) AVAssetWriterInput *assetWriterInput;
@property (nonatomic, readonly, nullable) NSDictionary<NSString *, id> *sourcePixelBufferAttributes;
@property (nonatomic, readonly, nullable) CVPixelBufferPoolRef pixelBufferPool;
// 添加samplebuffer
- (BOOL)appendPixelBuffer:(CVPixelBufferRef)pixelBuffer withPresentationTime:(CMTime)presentationTime;

@end
```

