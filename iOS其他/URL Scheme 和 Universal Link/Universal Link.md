## Universal Link

-----

###### `Universal Link`出现解决的几个问题：

- 在没有`Universal Link`的年代，我们使用`URL Scheme`。调用`openUrl`方法后，如果`App`已经安装，那么就会跳转到`App`，如果`App`没有安装，那么久不会有任何反应。而`Universal Link`增加了一个引导的特性，由于他本身是一个`https`的一个链接，如果`App`不存在的话，可以跳转到一个网页，如果这个网页重定向到`App Store`的话，那么就能打开`App Store`了。交互上更加人性化

- 相对于`URL Scheme`来说，`Universal Links`是使用`https`协议，规定了他必须有一个`全球唯一的域名`，所以不会出现`URL Scheme`重复的问题，也就不会出现`URL Scheme被劫持的问题`

- 安全性，用universl link去打开的时候，只有你（开发这个app的人）可以通过创建和上传一个允许这个网页去通过这个URL去打开你的app的文件。

- 隐私性，提供Universal link给别的app进行app间的交流，然而对方并不能够用这个方法去检测你的app是否被安装。（之前的custom scheme URL的canOpenURL方法可以，具体可以看这里[iOS Review-DetectScheme](https://www.jianshu.com/p/97a0e708a6b2)。）

  

  