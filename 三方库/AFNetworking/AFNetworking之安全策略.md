## AFNetworking之安全策略

----------

主要关注的类是：`AFSecurityPolicy`

`AFNetworking`给出了SSL/TLS的三种验证证书的策略：

```objective-c
typedef NS_ENUM(NSUInteger, AFSSLPinningMode) {
    // 只验证证书是否在信任列表中，表示的是CA机构签发的证书
    AFSSLPinningModeNone,
    // 只验证服务端与客户端证书的公钥是否一致。表示的是服务器自签名证书。表示客户端需要保存一个服务端根证书公钥，用于验证服务端证书是否合法。客户端需要将服务端证书链上的任意一证书的公钥拖入xcode工程中。使用公钥验证，则需要从服务端证书中取出公钥，同时取出客户端中保存的公钥，逐一比较，如果有匹配的就认为验证成功。
    AFSSLPinningModePublicKey,
    // 验证证书是否在信任列表中，然后在对别服务器端证书和客户端证书是否一致。表示的是服务器端自签名证书表示客户端需要保存一个服务端根证书，用于验证服务端证书是否合法。客户端需要将服务端证书的证书链上的任意一个证书拖入xcode工程中。
    AFSSLPinningModeCertificate,
};
```

