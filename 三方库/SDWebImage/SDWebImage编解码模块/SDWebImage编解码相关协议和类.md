## SDWebImage编解码相关协议和类

---------



#### 相关协议

---------------

###### SDImageCoder:

```markdown
- (BOOL)canDecodeFromData:(nullable NSData *)data;

- (nullable UIImage *)decodedImageWithData:(nullable NSData *)data options:(nullable SDImageCoderOptions *)options;

- (BOOL)canEncodeToFormat:(SDImageFormat)format;

- (nullable NSData *)encodedDataWithImage:(nullable UIImage *)image format:(SDImageFormat)format options:(nullable SDImageCoderOptions *)options;
```

###### SDProgressiveImageCoder<SDSImageCoder>:

```markdown
渐进式的解码协议
```

###### SDAnimatedImageProvider:

```markdown
为动态图片的渲染提供了基础的函数。`SDAnimatedImage`和`SDAnimteadImageCoder`使用了它
```

###### SDAnimatedImageCoder<SDImageCoder，SDAnimatedImageProvider>:

```markdown
为自定义的动图类，提供的编码协议。比如`SDAnimatedImage`
```



#### 相关类

--------------

###### SDImageIOCoder:

```markdown
@interface SDImageIOCoder : NSObject <SDProgressiveImageCoder>
// 内置的编码器，支持PNG、JPEG、TIFF，同时支持渐进式编码
```

###### SDImageIOAnimatedCoder:

```markdown
@interface SDImageIOAnimatedCoder : NSObject <SDProgressiveImageCoder, SDAnimatedImageCoder>
// 这个是所有的动图解码的抽象类，具体的方法需要子类去实现。相关的子类有：`SDImageAPNGCoder`、`SDImageAWebPCoder`、`SDImageGIFCoder`、`SDImageHEICCoder`
```

###### SDImageAPNGCoder:

```markdown

```

###### SDImageAWebPCoder:

```markdown

```

###### SDImageHEICCoder:

```markdown

```

###### SDImageCodersManager:

```markdown

```













