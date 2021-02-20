## UIButton上添加点击手势

------

由于`window sendEvent`首先会发送事件给`UIGestureEnvironment`，然后才是`UIView`，手势如果识别成功，默认会给`UIView`发送`Cancel`方法。所以button的`target-action`方法不会执行