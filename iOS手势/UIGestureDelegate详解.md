## UIGestureRecognizerDelegate

------

```objective-c
@protocol UIGestureRecognizerDelegate <NSObject>
// 开始进行手势识别调用的方法
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer;

// 返回YES的话，允许多个手势同时出发
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer;

// 这个方法返回YES，第二个手势的优先级高于第一个手势
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRequireFailureOfGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
// 这个方法返回YES，第一个手势的优先级高于第二个手势
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldBeRequiredToFailByGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer

// 在`touchesBegan:withEvent:`之前调用. 返回NO的话，禁止手势识别器识别这个触摸事件
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch;
// 在`pressesBegan:withEvent:`之前调用. 返回NO的话，禁止手势识别器识别这个按压动作
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceivePress:(UIPress *)press;


// 在 `-gestureRecognizer:shouldReceiveTouch:` 或者 `-gestureRecognizer:shouldReceivePress:`之前调用。返回NO的时候，禁止手识别器响应这个事件
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveEvent:(UIEvent *)event

@end
```

上面这几个方法，都是输入`事件响应链`传递阶段，其中后三个方法是在`UIGestureRecognizer`的`touch begin`方法之前执行，如果这三个方法中的一个失败了，那么`UIGestureRecognizer`中的`touch begin`不会执行，响应链继续往下走。