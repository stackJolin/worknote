## View Update Cycle

UIView的绘制策略是`按需绘制(on-demand)`



#### View Drawing Cycle 和 主Runloop

--------

一个 iOS 应用的主 RunLoop 负责处理所有的用户输入事件并触发相应的响应。所有的用户交互都会被加入到一个事件队列中。下图中的 `Application` object 会从队列中取出事件并将它们分发到应用中的其他对象上。本质上它会解释这些来自用户的输入事件，然后调用在应用中的 Core objects 相应的处理代码，而这些代码再调用开发者写的代码。当这些方法调用返回后，控制流回到主 RunLoop 上，然后开始 *update cycle（更新周期）*。Update cycle 负责布局并且重新渲染视图们（接下来会讲到）。下面的图片展示了应用是如何和设备交互并且处理用户输入的。



#### 相关名词

- Update Cycle







#### 相关文章

- https://juejin.cn/post/6844903567610871816
- https://medium.com/better-programming/demystifying-the-view-drawing-cycle-fb1f7ac519f1
- https://juejin.cn/post/6844903431086276615



iOS开发自动布局文档：https://developer.apple.com/library/archive/documentation/UserExperience/Conceptual/AutolayoutPG/ModifyingConstraints.html#//apple_ref/doc/uid/TP40010853-CH29-SW2

