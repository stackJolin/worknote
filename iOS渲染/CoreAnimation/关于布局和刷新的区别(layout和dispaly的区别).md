## Layout和Display的区别

-----------------

这里的讨论，只是针对`UIView`的，并不是针对`CALayer`。UIView有下面几个和刷新有关的方法：

- setNeedsLayout
- layoutIfNeed
- setNeedsDisplay
- setNeedsDisplayInRect

首先，`布局(layout)`不等于`绘制(display)`。

`布局`仅仅是改变frame，实际上调用的是`layoutSubViews`的方法

`绘制`是重新绘制`CALayer`上`contents`的内容，调用的是`drawRect`方法。

仅仅改变`frame`并不会引起重新绘制，系统会根据`Frame`大小，自动适配之前的`contents`内容

