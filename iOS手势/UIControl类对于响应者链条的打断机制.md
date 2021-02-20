## UIControl类对于响应者链条的打断机制

--------

通过分析事件产生、分发、响应的整个过程，我们知道了`UIGestureRecognizer`的优先级是要高于`UIView`，举个例子说明这一点：如果一个`button`实现了`target-action`事件，同时也实现了`UITapGestureRecognizer`事件的话，那么只会响应`UITapGestureRecognizer`事件

这里的`打断`是什么意思呢，准确的说当前的`View`响应事件了，不需要再往低层查找事件响应者了，也就是父控件(View或者vc)的`touches`系列方法都不会在执行了。