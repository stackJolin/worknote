## Main Event Runloop



#### The Application Object Gets and Dispatches Events(`Application Object`获取和分发事件)

------





#### Core Objects Respond to Events and Draw the User Interface

--------

当应用启动的时候，同时也会初始化包含多个`core object`的一个`core group`，这些对象负责绘制用户接口(UI)以及接受事件(例如触摸)。这些`core objects`包含`windows`和很多的`View`。当应用从事件队列获取到一个事件的时候，`Application Object`会将这个事件分发给用户事件发生时所在的`Window`。这个`Windos`会将事件分发给`Window`里面最适合处理这个事件的`View`：

- 对于`多点触摸`和`鼠标事件`，这个`view`就是触摸点所在的View
- 对于键盘，运动和其他事件，这个`view`就是第一响应者

如果初始视图无法捕获这个事件，这个事件将会沿着`响应者链条`传递给其他的`View`

当捕获到这个事件后，`view`经常初始化一系列的动作，并且修改应用的UI，更新应用的状态和数据。当这些动作被完成的时候，控制权又返回到`Applicaiton Object`，它将从事件循环中获取下一个事件