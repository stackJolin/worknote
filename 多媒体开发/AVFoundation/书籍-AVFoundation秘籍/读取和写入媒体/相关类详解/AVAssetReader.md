## AVAssetReader

------

最主要的属性或者方法：

```objective-c
typedef NS_ENUM(NSInteger, AVAssetReaderStatus) {
    AVAssetReaderStatusUnknown = 0,
    AVAssetReaderStatusReading = 1,
    AVAssetReaderStatusCompleted = 2,
    AVAssetReaderStatusFailed = 3,
    AVAssetReaderStatusCancelled = 4,
};

@interface AVAssetReader : NSObject {
		@private
		AVAssetReaderInternal		*_priv;
}

// 初始化方法
+ (nullable instancetype)assetReaderWithAsset:(AVAsset *)asset error:(NSError * _Nullable * _Nullable)outError;
- (nullable instancetype)initWithAsset:(AVAsset *)asset error:(NSError * _Nullable * _Nullable)outError;

@property (nonatomic, retain, readonly) AVAsset *asset;
@property (readonly) AVAssetReaderStatus status;
@property (readonly, nullable) NSError *error;
// 指定读取的范围
@property (nonatomic) CMTimeRange timeRange;
@property (nonatomic, readonly) NSArray<AVAssetReaderOutput *> *outputs;

- (BOOL)canAddOutput:(AVAssetReaderOutput *)output;
- (void)addOutput:(AVAssetReaderOutput *)output;
- (BOOL)startReading;
- (void)cancelReading;

@end
```

一个`AVAssetReader`可以关联多个`AVAssetReaderOutput`，比如一个`AVAsset`资源，可能有多个`track`，一个视频轨道、一个或者两个音频轨道，这个时候，每个`track`需要对应一个`AVAssetReaderOutput`。而`AVAssetReaderOutput`是一个抽象类