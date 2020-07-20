## SDWebImage解码设计

###### 相关类和协议

- SDImageCoder:协议

  ```
  - (BOOL)canDecodeFromData:(nullable NSData *)data;
  - (nullable UIImage *)decodedImageWithData:(nullable NSData *)data
                                     options:(nullable SDImageCoderOptions *)options;
  - (BOOL)canEncodeToFormat:(SDImageFormat)format NS_SWIFT_NAME(canEncode(to:));
  - (nullable NSData *)encodedDataWithImage:(nullable UIImage *)image
                                     format:(SDImageFormat)format
                                    options:(nullable SDImageCoderOptions *)options;
  ```

- SDProgressiveImageCoder:协议

- SDAnimatedImageProvider:协议

- SDAnimatedImageCoder

  ```
  @protocol SDAnimatedImageCoder <SDImageCoder, SDAnimatedImageProvider>
  
  - (nullable instancetype)initWithAnimatedImageData:(nullable NSData *)data options:(nullable SDImageCoderOptions *)options;
  ```

- SDImageCodersManager

- SDImageCoderHelper

- SDImageCoder

- SDImageIOAnimatedCoder

  ```
  @interface SDImageIOAnimatedCoder : NSObject <SDProgressiveImageCoder, SDAnimatedImageCoder>
  ```

- SDImageAPNGCoder、SDImageGIFCoder、SDImageHEICCoder

  ```
  @interface SDImageAPNGCoder : SDImageIOAnimatedCoder <SDProgressiveImageCoder, SDAnimatedImageCoder>
  
  @interface SDImageGIFCoder : SDImageIOAnimatedCoder <SDProgressiveImageCoder, SDAnimatedImageCoder>
  
  @interface SDImageHEICCoder : SDImageIOAnimatedCoder <SDProgressiveImageCoder, SDAnimatedImageCoder>
  ```

- SDImageFrame

- SDImageIOCoder

- 



#### 其他知识点

----

- ImageIO.framewor
- k