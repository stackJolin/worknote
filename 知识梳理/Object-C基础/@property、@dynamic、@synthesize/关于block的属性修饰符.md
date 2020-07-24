## 关于Block的属性修饰符

现在的Xcode工程默认都是ARC，如果需要全局使用ARC的话需要，需要关闭ARC自动引用计数：

<img src='./img/1.jpg' align=left >

如果仅仅是某个文件需要关闭ARC引用计数，使用MRC手动管理引用计数的话，需要在Build Phase -> Compile Sources找到相应的类，添加`-fno-objc-arc`

<img src='./img/2.jpg' align=left>

