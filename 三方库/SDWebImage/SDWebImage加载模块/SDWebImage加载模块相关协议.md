## SDWebImage加载模块相关类和协议

--------------

###### 相关协议：

- SDImageLoader

###### 相关类：

- SDWebImageDownLoader
- SDWebImageLoadersManager



#### SDImageLoader

---------

```objective-c
@optional
- (BOOL)canRequestImageForURL:(nullable NSURL *)url
                      options:(SDWebImageOptions)options
                      context:(nullable SDWebImageContext *)context;

@required
- (nullable id<SDWebImageOperation>)requestImageWithURL:(nullable NSURL *)url
                                                options:(SDWebImageOptions)options
                                                context:(nullable SDWebImageContext *)context
                                               progress:(nullable SDImageLoaderProgressBlock)progressBlock
                                              completed:(nullable SDImageLoaderCompletedBlock)completedBlock;

@optional

// 是否不再加载之前加载失败的URL
- (BOOL)shouldBlockFailedURLWithURL:(nonnull NSURL *)url
                              error:(nonnull NSError *)error
                            options:(SDWebImageOptions)options
                            context:(nullable SDWebImageContext *)context;
```



