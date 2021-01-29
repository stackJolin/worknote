## NSOperationQueue 

---------

API:

```objective-c
@interface NSOperationQueue : NSObject <NSProgressReporting> 
  
@property (readonly, strong) NSProgress *progress;
@property (class, readonly, strong, nullable) NSOperationQueue *currentQueue;
@property (class, readonly, strong) NSOperationQueue *mainQueue;
@property NSInteger maxConcurrentOperationCount;

@property (getter=isSuspended) BOOL suspended;

@property (nullable, copy) NSString *name;

@property NSQualityOfService qualityOfService;

@property (nullable, assign /* actually retain */) dispatch_queue_t underlyingQueue;

- (void)addOperation:(NSOperation *)op;
- (void)addOperations:(NSArray<NSOperation *> *)ops waitUntilFinished:(BOOL)wait;
- (void)addOperationWithBlock:(void (^)(void))block;

/// @method addBarrierBlock:
/// @param barrier      A block to execute
/// @discussion         The `addBarrierBlock:` method executes the block when the NSOperationQueue has finished all enqueued operations and
/// prevents any subsequent operations to be executed until the barrier has been completed. This acts similarly to the
/// `dispatch_barrier_async` function.
- (void)addBarrierBlock:(void (^)(void))barrier;

// 取消所有的操作
- (void)cancelAllOperations;

// 会阻塞当前线程，等待`queue`中的所有的任务都完成，才会继续执行
- (void)waitUntilAllOperationsAreFinished;

@end
```





#### NSOperationQueue控制串行和并发

--------

