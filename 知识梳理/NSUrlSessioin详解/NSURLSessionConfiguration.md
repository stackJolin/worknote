## NSURLSessionConfiguration

---------

| 名称                               | 类型                           |                                    |
| ---------------------------------- | ------------------------------ | ---------------------------------- |
| identifier                         | String                         |                                    |
| requestCachePolicy                 | NSURLRequestCachePolicy        |                                    |
| timeoutIntervalForRequest          | NSTimeInterval                 |                                    |
| timeoutIntervalForResource         | NSTimeInterval                 |                                    |
| networkServiceType                 | NSURLRequestNetworkServiceType | 网络请求的服务类型                 |
| allowsCellularAccess               | Bool                           | 是否使用蜂窝网络                   |
| allowsExpensiveNetworkAccess       | Bool                           |                                    |
| allowsConstrainedNetworkAccess     | Bool                           |                                    |
| waitsForConnectivity               | Bool                           |                                    |
| discretionary                      | Bool                           |                                    |
| sharedContainerIdentifier          |                                |                                    |
| sessionSendsLaunchEvents           |                                |                                    |
| connectionProxyDictionary          |                                |                                    |
| TLSMinimumSupportedProtocol        | SSLProtocol                    |                                    |
| TLSMaximumSupportedProtocol        | SSLProtocol                    |                                    |
| TLSMinimumSupportedProtocolVersion | tls_protocol_version_t         |                                    |
| TLSMaximumSupportedProtocolVersion | tls_protocol_version_t         |                                    |
| HTTPShouldUsePipelining            | Bool                           |                                    |
| HTTPShouldSetCookies               | Bool                           |                                    |
| HTTPCookieAcceptPolicy             | NSHTTPCookieAcceptPolicy       |                                    |
| HTTPAdditionalHeaders              | NSDictionary                   | HTTP请求报文首部字段，添加其他内容 |
| HTTPMaximumConnectionsPerHost      | NSInteger                      | 单机最大连接数                     |
| HTTPCookieStorage                  | NSHTTPCookieStorage            | Cookie存储器                       |
| URLCredentialStorage               | NSURLCredentialStorage         | 证书存储器                         |
| URLCache                           | NSURLCache                     | URLCache                           |
|                                    |                                |                                    |
|                                    |                                |                                    |



#### NSHTTPCookieAcceptPolicy

```objective-c
typedef NS_ENUM(NSUInteger, NSHTTPCookieAcceptPolicy) {
    NSHTTPCookieAcceptPolicyAlways, // 默认策略，接收所有cookie
    NSHTTPCookieAcceptPolicyNever, // 拒绝所有cookie
    NSHTTPCookieAcceptPolicyOnlyFromMainDocumentDomain
};
```

