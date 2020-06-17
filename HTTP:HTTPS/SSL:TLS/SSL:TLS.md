## SSL/TLS

#### SSL连接建立第一阶段:SSL握手

-----

- 客户端发送Client Hello报文，报文包含以下信息:
  - SSL版本
  - 随机数 RN1 (用于后面生成对称机密秘钥)
  - 安全策略或者叫做加密套件-CipherSuites(主要是客户端所支持的加密方法)
  - 会话ID,如果是第一次连接,那么为空
- server回应ServerHello，主要包含以下信息:
  - 指定SSL版本
  - 随机数 RN2 (用于后面生成对称加密秘钥)
  - 指定的加密套件
  - 会话ID
- server发送certificate(证书)给客户端
- 





#### 相关文章

-----

- http://www.ruanyifeng.com/blog/2014/02/ssl_tls.html
- [https://cshihong.github.io/2019/05/09/SSL%E5%8D%8F%E8%AE%AE%E8%AF%A6%E8%A7%A3/](https://cshihong.github.io/2019/05/09/SSL协议详解/)

