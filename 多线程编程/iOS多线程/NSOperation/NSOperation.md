## NSOperation

---------------



#### API

-------

```objective-c
@interface NSOperation : NSObject
  
typedef NS_ENUM(NSInteger, NSOperationQueuePriority) {
	NSOperationQueuePriorityVeryLow = -8L,
	NSOperationQueuePriorityLow = -4L,
	NSOperationQueuePriorityNormal = 0,
	NSOperationQueuePriorityHigh = 4,
	NSOperationQueuePriorityVeryHigh = 8
};
  
@property (readonly, getter=isCancelled) BOOL cancelled;
@property (readonly, getter=isExecuting) BOOL executing;
@property (readonly, getter=isFinished) BOOL finished;
@property (readonly, getter=isConcurrent) BOOL concurrent;
@property (readonly, getter=isAsynchronous) BOOL asynchronous;
@property (readonly, getter=isReady) BOOL ready;
// 操作优先级
@property NSOperationQueuePriority queuePriority;
@property (nullable, copy) void (^completionBlock)(void);
@property (readonly, copy) NSArray<NSOperation *> *dependencies;
// 已经被删除
@property double threadPriority;
@property NSQualityOfService qualityOfService;
@property (nullable, copy) NSString *name;

- (void)cancel;
- (void)start;
- (void)main;
// 添加依赖
- (void)addDependency:(NSOperation *)op;
// 移除依赖
- (void)removeDependency:(NSOperation *)op;
// 会阻塞当前线程，等待`operation`完成
- (void)waitUntilFinished;

@end
```





#### NSOperation优先级

---------------

```objective-c
typedef NS_ENUM(NSInteger, NSOperationQueuePriority) {
	NSOperationQueuePriorityVeryLow = -8L,
	NSOperationQueuePriorityLow = -4L,
	NSOperationQueuePriorityNormal = 0,
	NSOperationQueuePriorityHigh = 4,
	NSOperationQueuePriorityVeryHigh = 8
};
```



NSOperation 提供了`queuePriority`（优先级）属性，`queuePriority`属性适用于同一操作队列中的操作，不适用于不同操作队列中的操作。默认情况下，所有新创建的操作对象优先级都是`NSOperationQueuePriorityNormal`。但是我们可以通过`setQueuePriority:`方法来改变当前操作在同一队列中的执行优先级。

而对于添加到队列中的操作，首先进入准备就绪的状态（就绪状态取决于操作之间的依赖关系），然后进入就绪状态的操作的开始执行顺序（非结束执行顺序）由操作之间相对的优先级决定（优先级是操作对象自身的属性）。

那么，什么样的操作才是进入就绪状态的操作呢？
当一个操作的所有依赖都已经完成时，操作对象通常会进入准备就绪状态，等待执行。
举个例子，现在有4个优先级都是 NSOperationQueuePriorityNormal（默认级别）的操作：op1，op2，op3，op4。其中 op3 依赖于 op2，op2 依赖于 op1，即 op3 -> op2 -> op1。现在将这4个操作添加到队列中并发执行。

- 因为 op1 和 op4 都没有需要依赖的操作，所以在 op1，op4 执行之前，就是处于准备就绪状态的操作。
- 而 op3 和 op2 都有依赖的操作（op3 依赖于 op2，op2 依赖于 op1），所以 op3 和 op2 都不是准备就绪状态下的操作。
  理解了进入就绪状态的操作，那么我们就理解了queuePriority 属性的作用对象。
- queuePriority 属性决定了进入准备就绪状态下的操作之间的开始执行顺序。并且，优先级不能取代依赖关系。
- 如果一个队列中既包含高优先级操作，又包含低优先级操作，并且两个操作都已经准备就绪，那么队列先执行高优先级操作。比如上例中，如果 op1 和 op4 是不同优先级的操作，那么就会先执行优先级高的操作。
- 如果，一个队列中既包含了准备就绪状态的操作，又包含了未准备就绪的操作，未准备就绪的操作优先级比准备就绪的操作优先级高。那么，虽然准备就绪的操作优先级低，也会优先执行。优先级不能取代依赖关系。如果要控制操作间的启动顺序，则必须使用依赖关系。



#### NSOperation操作依赖

----------







#### 自定义`NSOperationQueue`

-------

