## work list

- bank需求
- csc-mac客服头像本地化
- csc-mac强制更新



#### 知识点

------

```
NSTextFiled如何限制两行，同时结尾以。。。结束，同时按字符换行
filed.lineBreakMode = .byCharWrapping
filed.cell?.truncatesLastVisibleLine = true
如果是AttributeString的话，需要指定AttributeString的paragraphStyle的lineBreakMode为byCharWrapping
主要这个时候，需要指定filed的高度大小要合适，不能过高，也不能过低，必须两行高度到三行高度之间
```



```
mac图标如何不显示在Dock中：
info.plist中添加如下内容：
<key>LSUIElement</key>
<true/>
```



```
Mac开发-App开机启动

```



#### Issue

----

- swift脚本打包二进制可执行文件
- Sparkle如何获取到系统执行权限
- NSTask、NSPipe
- Mac App sanbox