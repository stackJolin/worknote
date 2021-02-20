## UIGestureRecognizer详解

-------

###### `UIGestureRecognizer`状态:

```objective-c
//手势状态枚举值
typedef NS_ENUM(NSInteger, UIGestureRecognizerState) {
    UIGestureRecognizerStatePossible,   // 默认的状态，这个时候的手势并没有具体的情形状态
    UIGestureRecognizerStateBegan,      // 手势开始被识别的状态
    UIGestureRecognizerStateChanged,    // 手势识别发生改变的状态
    UIGestureRecognizerStateEnded,      // 手势识别结束，将会执行触发的方法
    UIGestureRecognizerStateCancelled,  // 手势识别取消
    UIGestureRecognizerStateFailed,     // 识别失败，方法将不会被调用
    UIGestureRecognizerStateRecognized = UIGestureRecognizerStateEnded 
};
```



iOS开发中的`UIGestureRecognizer`分为`离散手势`和`连续手势`

- 离散手势
  - tapGesture
  - swipeGesture
- 连续手势
  - panGesture
  - rotationGesture



#### UIGestureRecognizer API

------

```objective-c
@property(nonatomic, readonly) UIGestureRecognizerState state;
@property(nullable,nonatomic,weak) id <UIGestureRecognizerDelegate> delegate;
// 为no的话，就和没有添加`UIGestureRecognizer`的View没有什么区别。如果这个手势正在执行的话，改为NO的话，当前手势会被cancelled掉
@property(nonatomic, getter=isEnabled) BOOL enabled;
@property(nullable, nonatomic,readonly) UIView *view;

// 默认是YES。会对手势所在的View，执行`touch:cancel`方法。设置为NO的话，不在执行`touch:cancel`方法。不执行`touch:cancel`方法的话，如果当前View是`UIControl`类的话，`UIControl`方法的事件也会执行。同理，`UITableView`的代理方法`didSelected`也是基于`UIResponder`的响应者方法，如果此时给TableView添加一个`UITapGestureRecognizer`的话，`UITableView`的`didSelected`方法将不会执行，这个时候，也需要将`UITapGestureRecognizer`的`cancelsTouchesInView`设置为NO，使得，`UITableView`正常响应`touch`事件
@property(nonatomic) BOOL cancelsTouchesInView;

// 是否延迟发送触摸事件给触摸到的控件
// 默认是NO，这种状况下当发生一个触摸时，手势识别器先捕捉到到触摸，而后发给触摸到的控件，二者各自作出响应。若是设置为YES，手势识别器在识别的过程当中（注意是识别过程），不会将触摸发给触摸到的控件，即控件不会有任何触摸事件。只有在识别失败以后才会将触摸事件发给触摸到的控件，这种状况下控件view的响应会延迟约0.15ms。
@property(nonatomic) BOOL delaysTouchesBegan;       

// 
@property(nonatomic) BOOL delaysTouchesEnded;         // default is YES. causes touchesEnded or pressesEnded events to be delivered to the target view only after this gesture has failed recognition. this ensures that a touch or press that is part of the gesture can be cancelled if the gesture is recognized

@property(nonatomic, copy) NSArray<NSNumber *> *allowedTouchTypes API_AVAILABLE(ios(9.0)); // Array of UITouchTypes as NSNumbers.
@property(nonatomic, copy) NSArray<NSNumber *> *allowedPressTypes API_AVAILABLE(ios(9.0)); // Array of UIPressTypes as NSNumbers.

// Indicates whether the gesture recognizer will consider touches of different touch types simultaneously.
// If NO, it receives all touches that match its allowedTouchTypes.
// If YES, once it receives a touch of a certain type, it will ignore new touches of other types, until it is reset to UIGestureRecognizerStatePossible.
@property (nonatomic) BOOL requiresExclusiveTouchType API_AVAILABLE(ios(9.2)); // defaults to YES

// create a relationship with another gesture recognizer that will prevent this gesture's actions from being called until otherGestureRecognizer transitions to UIGestureRecognizerStateFailed
// if otherGestureRecognizer transitions to UIGestureRecognizerStateRecognized or UIGestureRecognizerStateBegan then this recognizer will instead transition to UIGestureRecognizerStateFailed
// example usage: a single tap may require a double tap to fail
// 指定一个手势需要另外一个手势识别失败的时候才会执行
- (void)requireGestureRecognizerToFail:(UIGestureRecognizer *)otherGestureRecognizer;

// individual UIGestureRecognizer subclasses may provide subclass-specific location information. see individual subclasses for details
- (CGPoint)locationInView:(nullable UIView*)view;                                // a generic single-point location for the gesture. usually the centroid of the touches involved

@property(nonatomic, readonly) NSUInteger numberOfTouches;                                          // number of touches involved for which locations can be queried
- (CGPoint)locationOfTouch:(NSUInteger)touchIndex inView:(nullable UIView*)view; // the location of a particular touch

@property (nullable, nonatomic, copy) NSString *name API_AVAILABLE(ios(11.0), tvos(11.0)); // name for debugging to appear in logging

// Values from the last event processed.
// These values are not considered as requirements for the gesture.
@property (nonatomic, readonly) UIKeyModifierFlags modifierFlags API_AVAILABLE(ios(13.4)) API_UNAVAILABLE(tvos, watchos);
@property (nonatomic, readonly) UIEventButtonMask buttonMask API_AVAILABLE(ios(13.4)) API_UNAVAILABLE(tvos, watchos);

@end
```





#### cancelsTouchesInView

-------------------

举例子说明：

有一个TableView，实现了`didSelected`方法，同时又另外添加了一个`UITapGestureRecognizer`。有下面两个不同的场景：

- 点击cell的时候，`UITapgestureRecognizer`和`didSelected`同时响应
- 点击cell的时候，只响应didSelected；点击table的时候，只响应`UITapGestureRecognizer`

第一个需求的解决方案:

```markdown
只需要将`UITapGestureRecognizer`的`cancelsTouchesInView`属性设置为NO，就好了。因为`UITableView`的`didSelected`方法，低层也是走的`UIResponder`的响应链条
```

第二个需求的解决方案：

```markdown
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
在下面方法中判断，touch的view是否在UITableViewCellContentView中
```



#### delaysTouchesBegan和delaysTouchesEnd

---------































