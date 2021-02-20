## SDImageLoader的默认实现SDWebImageDownLoader

--------

###### 相关枚举：

- SDWebImageDownloaderOptions



###### 相关协议：

- SDWebImageDownloaderRequestModifier
- SDWebImageDownloaderResponseModifier
- SDWebImageDownloaderDecryptor



###### 相关类：

- SDWebImageDownloaderConfig
- SDWebImageDownLoader
- SDWebImageDownloadToken
- SDWebImageDownloaderRequestModifier
- SDWebImageDownloaderResponseModifier
- SDWebImageDownloaderDecryptor
- SDWebImageDownloaderOperation



#### SDWebImageDownloaderOptions

---------

```objective-c
typedef NS_OPTIONS(NSUInteger, SDWebImageDownloaderOptions) {
    // 默认模式，将下载放入低优先级队列 和 低优先级任务中
    SDWebImageDownloaderLowPriority = 1 << 0,
    // 渐进式下载
    SDWebImageDownloaderProgressiveLoad = 1 << 1,
    // 默认情况下，http会请求阻止使用NSURLCache。使用这个标志，NSURLCache将于默认策略一起使用
    SDWebImageDownloaderUseNSURLCache = 1 << 2,

    /**
     * Call completion block with nil image/imageData if the image was read from NSURLCache
     * And the error code is `SDWebImageErrorCacheNotModified`
     * This flag should be combined with `SDWebImageDownloaderUseNSURLCache`.
     */
    SDWebImageDownloaderIgnoreCachedResponse = 1 << 3,
    
    /**
     * In iOS 4+, continue the download of the image if the app goes to background. This is achieved by asking the system for
     * extra time in background to let the request finish. If the background task expires the operation will be cancelled.
     */
    SDWebImageDownloaderContinueInBackground = 1 << 4,

    /**
     * Handles cookies stored in NSHTTPCookieStore by setting 
     * NSMutableURLRequest.HTTPShouldHandleCookies = YES;
     */
    SDWebImageDownloaderHandleCookies = 1 << 5,

    /**
     * Enable to allow untrusted SSL certificates.
     * Useful for testing purposes. Use with caution in production.
     */
    SDWebImageDownloaderAllowInvalidSSLCertificates = 1 << 6,
    // 将下载放入 高队列优先级 和 高任务优先级 中
    SDWebImageDownloaderHighPriority = 1 << 7,
    
    /**
     * By default, images are decoded respecting their original size. On iOS, this flag will scale down the
     * images to a size compatible with the constrained memory of devices.
     * This flag take no effect if `SDWebImageDownloaderAvoidDecodeImage` is set. And it will be ignored if `SDWebImageDownloaderProgressiveLoad` is set.
     */
    SDWebImageDownloaderScaleDownLargeImages = 1 << 8,
    
    /**
     * By default, we will decode the image in the background during cache query and download from the network. This can help to improve performance because when rendering image on the screen, it need to be firstly decoded. But this happen on the main queue by Core Animation.
     * However, this process may increase the memory usage as well. If you are experiencing a issue due to excessive memory consumption, This flag can prevent decode the image.
     */
    SDWebImageDownloaderAvoidDecodeImage = 1 << 9,
    
    /**
     * By default, we decode the animated image. This flag can force decode the first frame only and produce the static image.
     */
    SDWebImageDownloaderDecodeFirstFrameOnly = 1 << 10,
    
    /**
     * By default, for `SDAnimatedImage`, we decode the animated image frame during rendering to reduce memory usage. This flag actually trigger `preloadAllAnimatedImageFrames = YES` after image load from network
     */
    SDWebImageDownloaderPreloadAllFrames = 1 << 11,
    
    /**
     * By default, when you use `SDWebImageContextAnimatedImageClass` context option (like using `SDAnimatedImageView` which designed to use `SDAnimatedImage`), we may still use `UIImage` when the memory cache hit, or image decoder is not available, to behave as a fallback solution.
     * Using this option, can ensure we always produce image with your provided class. If failed, a error with code `SDWebImageErrorBadImageData` will been used.
     * Note this options is not compatible with `SDWebImageDownloaderDecodeFirstFrameOnly`, which always produce a UIImage/NSImage.
     */
    SDWebImageDownloaderMatchAnimatedImageClass = 1 << 12,
};
```

