## NSUrlSession



#### 相关类和协议

-----

###### 类

- NSUrlSessionTask

  是一个抽象类，我们一般使用的是他的子类`NSUrlSessonDataTask`、`NSUrlSessionDownloadTask`、`NSUrlSessionUploadTask`、`NSUrlSessionStreamTask`以及最新的`NSUrlSessionWebSocketTask`

  ```
  @interface NSURLSessionTask : NSObject <NSCopying, NSProgressReporting>
  ```

- NSUrlSessionDataTask

  我们平常使用的`GET`和`POST`都是使用的`NSUrlSessionDataTask`，除此之外，他可以实现`文件上传`和`文件下载`，`NSUrlSessionDownloadTask`和`NSUrlSessionUploadTask`都是基于`NSUrlSessionDataTask`实现的。

  ```
  @interface NSURLSessionDataTask : NSURLSessionTask
  ```

- NSUrlSessionUploadTask

  用于文件上传

  ```
  @interface NSURLSessionUploadTask : NSURLSessionDataTask
  ```

- NSUrlSessionDownloadTask

  用于文件下载。支持断点续传，然后不足的是：不支持离线下载。这个时候就需要使用`NSUrlSessionDataTask`，这也是`SDWebImage`为什么不使用`NSUrlSessionDownloadTask`，而是使用`NSUrlSessionDataTask`的原因

  ```
  @interface NSURLSessionDownloadTask : NSURLSessionTask
  ```

- NSUrlSessionStreamTask

  ```
  API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0))
  @interface NSURLSessionStreamTask : NSURLSessionTask
  ```

- NSUrlSessionWebSocketTask

  ```
  API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0))
  @interface NSURLSessionWebSocketTask : NSURLSessionTask
  ```

###### 协议

- NSUrlSessionDelegate

  ```
  @protocol NSURLSessionDelegate <NSObject>
  ```

- NSURLSessionTaskDelegate

  ```
  @protocol NSURLSessionTaskDelegate <NSURLSessionDelegate>
  ```

- NSURLSessionDataDelegate

  ```
  @protocol NSURLSessionDataDelegate <NSURLSessionTaskDelegate>
  ```

- NSURLSessionDownloadDelegate

  ```
  @protocol NSURLSessionDownloadDelegate <NSURLSessionTaskDelegate>
  ```

- NSURLSessionStreamDelegate

  ```
  @protocol NSURLSessionStreamDelegate <NSURLSessionTaskDelegate>
  ```

- NSURLSessionWebSocketDelegate

  ```
  @protocol NSURLSessionWebSocketDelegate <NSURLSessionTaskDelegate>
  ```

#### Task的状态

----

NSUrlSessionTask有一个属性：

```
@property (readonly) NSURLSessionTaskState state;
```

该属性，标识了任务的状态。一个Task总共有四种状态：

```
typedef NS_ENUM(NSInteger, NSURLSessionTaskState) {
    NSURLSessionTaskStateRunning = 0,                     /* The task is currently being serviced by the session */
    NSURLSessionTaskStateSuspended = 1,
    NSURLSessionTaskStateCanceling = 2,                   /* The task has been told to cancel.  The session will receive a URLSession:task:didCompleteWithError: message. */
    NSURLSessionTaskStateCompleted = 3,                   /* The task has completed and the session will receive no more delegate notifications */
}
```



#### NSURLSessionDelegate

---------

```objective-c
// 会话结束
- (void)URLSession:(NSURLSession *)session didBecomeInvalidWithError:(nullable NSError *)error;

// 请求客户端证书配置
- (void)URLSession:(NSURLSession *)session didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge
                                             completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential * _Nullable credential))completionHandler;

// 后台网络任务完成回调
- (void)URLSessionDidFinishEventsForBackgroundURLSession:(NSURLSession *)session API_AVAILABLE(ios(7.0), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);
```





#### NSURLSessionTaskDelegate

----

```objectivec
/*
系统在发起请求之前会调一个这个回调，
然后在这个completionHandler里面你告诉系统这个请求是否要发出去，是否要修改
（告诉代理现在将开始加载延迟的URL会话任务。
当具有延迟开始时间的后台会话任务（由earliestBeginDate属性设置）准备就绪时，将调用此方法。只有在等待网络负载时请求可能变陈旧并需要被新请求替换时，才应实现此委托方法。
为了继续加载，委托人必须调用完成处理程序，并传递一个处理方式来指示任务应该如何进行。传递NSURLSessionDelayedRequestCancel处置等效于直接调用任务的取消。）
*/
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                        willBeginDelayedRequest:(NSURLRequest *)request
                              completionHandler:(void (^)(NSURLSessionDelayedRequestDisposition disposition, NSURLRequest * _Nullable newRequest))completionHandler
```

```objectivec
/*
告诉代理，在开始网络加载之前，任务正在等待，直到合适的连接可用。
 如果NSURLSessionConfiguration的waitsForConnectivity属性为true并且没有足够的连接，则调用此方法。 代表可以利用这个机会来更新用户界面;
 例如通过呈现离线模式或仅限蜂窝模式。
 此方法最多只能在每个任务中调用一次，并且仅在连接最初不可用时调用。 它永远不会被调用后台会话，因为这些会话会忽略waitsForConnectivity。
 */
- (void)URLSession:(NSURLSession *)session taskIsWaitingForConnectivity:(NSURLSessionTask *)task
```

```objectivec
/* 
告诉委托远程服务器请求HTTP重定向。
此方法仅适用于默认和临时会话中的任务。 后台会话中的任务会自动遵循重定向。
需要后台返回statusCode=302/301这两种状态同时要配置上新的request
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                     willPerformHTTPRedirection:(NSHTTPURLResponse *)response
                                     newRequest:(NSURLRequest *)request
                              completionHandler:(void (^)(NSURLRequest * _Nullable))completionHandler;
```

```objectivec
/* 
响应来自远程服务器的认证请求，从代理请求凭证。
 该方法处理任务级别的身份验证挑战。 NSURLSessionDelegate协议还提供了会话级别的身份验证委托方法。所调用的方法取决于身份验证挑战的类型：
对于会话级挑战-NSURLAuthenticationMethodNTLM，NSURLAuthenticationMethodNegotiate，NSURLAuthenticationMethodClientCertificate或NSURLAuthenticationMethodServerTrust - NSURLSession对象调用会话委托的URLSession：didReceiveChallenge：completionHandler：方法。如果您的应用程序未提供会话委托方法，则NSURLSession对象会调用任务委托人的URLSession：task：didReceiveChallenge：completionHandler：方法来处理该挑战。
对于非会话级挑战（所有其他挑战），NSURLSession对象调用会话委托的URLSession：task：didReceiveChallenge：completionHandler：方法来处理挑战。如果您的应用程序提供会话委托，并且您需要处理身份验证，那么您必须在任务级别处理身份验证，或者提供明确调用每会话处理程序的任务级别处理程序。会话委托的URLSession：didReceiveChallenge：completionHandler：方法不针对非会话级别的挑战进行调用。
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                            didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge 
                              completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential * _Nullable credential))completionHandler;
```

```objectivec
/* 
当任务需要新的请求主体流发送到远程服务器时，告诉委托。
这种委托方法在两种情况下被调用：
1、如果使用uploadTaskWithStreamedRequest创建任务，则提供初始请求正文流：
2、如果任务因身份验证质询或其他可恢复的服务器错误需要重新发送包含正文流的请求，则提供替换请求正文流。
注：如果代码使用文件URL或NSData对象提供请求主体，则不需要实现此功能。
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                              needNewBodyStream:(void (^)(NSInputStream * _Nullable bodyStream))completionHandler;
```

```objectivec
/* 
定期通知代理向服务器发送主体内容的进度。(上传进度) 
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                                didSendBodyData:(int64_t)bytesSent
                                 totalBytesSent:(int64_t)totalBytesSent
                       totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend;
```

```objectivec
/*
 告诉代理该会话完成了该任务的收集指标。
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didFinishCollectingMetrics:(NSURLSessionTaskMetrics *)metrics
```

```objectivec
/* 
告诉代理该任务完成传输数据。
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                           didCompleteWithError:(nullable NSError *)error;
```



#### NSURLSessionDataDelegate

------

```objective-c
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                                 didReceiveResponse:(NSURLResponse *)response
                                  completionHandler:(void (^)(NSURLSessionResponseDisposition disposition))completionHandler;

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                              didBecomeDownloadTask:(NSURLSessionDownloadTask *)downloadTask;

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                                didBecomeStreamTask:(NSURLSessionStreamTask *)streamTask;

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                                     didReceiveData:(NSData *)data;

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                                  willCacheResponse:(NSCachedURLResponse *)proposedResponse 
                                  completionHandler:(void (^)(NSCachedURLResponse * _Nullable cachedResponse))completionHandler;
```



#### NSURLSessionDownloadDelegate

```objective-c
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
                              didFinishDownloadingToURL:(NSURL *)location;

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
                                           didWriteData:(int64_t)bytesWritten
                                      totalBytesWritten:(int64_t)totalBytesWritten
                              totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite;

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
                                      didResumeAtOffset:(int64_t)fileOffset
                                     expectedTotalBytes:(int64_t)expectedTotalBytes;
```



#### NSURLSessionStreamDelegate

```objective-c
- (void)URLSession:(NSURLSession *)session readClosedForStreamTask:(NSURLSessionStreamTask *)streamTask;

- (void)URLSession:(NSURLSession *)session writeClosedForStreamTask:(NSURLSessionStreamTask *)streamTask;

- (void)URLSession:(NSURLSession *)session betterRouteDiscoveredForStreamTask:(NSURLSessionStreamTask *)streamTask;

- (void)URLSession:(NSURLSession *)session streamTask:(NSURLSessionStreamTask *)streamTask
                                 didBecomeInputStream:(NSInputStream *)inputStream
                                         outputStream:(NSOutputStream *)outputStream;
```



#### NSURLSessionWebSocketDelegate

```objective-c
- (void)URLSession:(NSURLSession *)session webSocketTask:(NSURLSessionWebSocketTask *)webSocketTask didOpenWithProtocol:(NSString * _Nullable) protocol;

- (void)URLSession:(NSURLSession *)session webSocketTask:(NSURLSessionWebSocketTask *)webSocketTask didCloseWithCode:(NSURLSessionWebSocketCloseCode)closeCode reason:(NSData * _Nullable)reason;
```



#### NSURLSessionAuthChallengeDisposition

-------

```objc
// 处理挑战的方式
typedef NS_ENUM(NSInteger, NSURLSessionAuthChallengeDisposition) {
    NSURLSessionAuthChallengeUseCredential = 0, // 使用客户端指定的方式处理挑战
    NSURLSessionAuthChallengePerformDefaultHandling = 1, // 使用默认方式处理挑战，走端上系统中内置的公认机构的证书
    NSURLSessionAuthChallengeCancelAuthenticationChallenge = 2, // 不接受本次挑战，取消TLS链接
    NSURLSessionAuthChallengeRejectProtectionSpace = 3, // 拒绝本次挑战，下次重新挑战
} API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));
```



#### 

```objective-c
NSURLAuthenticationMethodDefault
// 基本的 HTTP 验证，通过 NSURLCredential 对象提供用户名和密码。
NSURLAuthenticationMethodHTTPBasic
// 类似于基本的 HTTP 验证，摘要会自动生成，同样通过 NSURLCredential 对象提供用户名和密码。
NSURLAuthenticationMethodHTTPDigest
// 不会用于 URL Loading System，在通过 web 表单验证时可能用到。
NSURLAuthenticationMethodHTMLForm
NSURLAuthenticationMethodNegotiate
NSURLAuthenticationMethodNTLM
// 验证客户端的证书
NSURLAuthenticationMethodClientCertificate
// 指明客户端要验证服务端提供的证书
NSURLAuthenticationMethodServerTrust
```



