## SDWebImage解码设计



#### 相关类和协议

---

###### 协议

- SDWebImageDownloaderRequestModifier

  用于修改原始的request，你可以修改HTTP header，cachePolicy，etc

  ```
  - (nullable NSURLRequest *)modifiedRequestWithRequest:(nonnull NSURLRequest *)request;
  ```

- SDWebImageDownloaderResponseModifier

  ```
  - (nullable NSURLResponse *)modifiedResponseWithResponse:(nonnull NSURLResponse *)response;
  ```

- SDWebImageDownloaderDecryptor

  ```
  - (nullable NSData *)decryptedDataWithData:(nonnull NSData *)data response:(nullable NSURLResponse *)response;
  ```

- SDWebImageDownloaderOperation

  ```
  
  ```

  

- SDWebImageOperation

  ```
  - (void)cancel;
  ```

- SDImageLoader

  ```
  - (BOOL)canRequestImageForURL:(nullable NSURL *)url;
  
  - (nullable id<SDWebImageOperation>)requestImageWithURL:(nullable NSURL *)url
                                                  options:(SDWebImageOptions)options
                                                  context:(nullable SDWebImageContext *)context
                                                 progress:(nullable SDImageLoaderProgressBlock)progressBlock
                                                completed:(nullable SDImageLoaderCompletedBlock)completedBlock;
  ```

###### 类

- SDWebImageDownloader

- SDWebImageDownloadToken

- SDWebImageDownloaderRequestModifier

  ```
  @interface SDWebImageDownloaderRequestModifier : NSObject <SDWebImageDownloaderRequestModifier>
  ```

- SDWebImageDownloaderResponseModifier

  ```
  - (nonnull instancetype)initWithBlock:(nonnull SDWebImageDownloaderResponseModifierBlock)block;
  + (nonnull instancetype)responseModifierWithBlock:(nonnull SDWebImageDownloaderResponseModifierBlock)block;
  ```

- SDWebImageDownloaderDecryptor

  ```
  - (nonnull instancetype)initWithBlock:(nonnull SDWebImageDownloaderDecryptorBlock)block;
  + (nonnull instancetype)decryptorWithBlock:(nonnull SDWebImageDownloaderDecryptorBlock)block;
  ```

- SDWebImageDownloaderOperation

  ```
  
  ```

  



#### SDWebImageDownloaderConfig

--------

























