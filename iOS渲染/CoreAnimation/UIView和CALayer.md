## UIView和CALayer

----

`UIView`负责处理事件，而`CALayer`负责显示UI

默认情况下，每个`UIView`的实例，都会关联一个`CALayer`。从此之后，这个`UIView`才有了展示的功能。`UIView`是在`CALayer`的基础上，做了一层封装，实现了

出现这种情况的原因，主要是为了分层处理问题：`macos`中的`NSView`是基于`单点触控`，而`iOS`中的`UIView`是基于`多点触控`。但是两者的显示基础都是一样的，就是`CALayer`



接下来我们会研究关于`Layer`的几个问题：

- 显示的内容如何提供

- 内容如何刷新展示(更新机制)

  

一般和`CALayer`的下面几个方法和属性相关

- 和刷新相关的
  - `display`
  - `setNeedsDisplay`
  - `displayIfNeeded`
- 和展示内容提供相关的
  - `drawInContext:`和`drawLayer:inContext:`



#### CALayer的绘制时机

-----

`iOS`的刷新机制是固定稳定的刷新频率在刷新，当然也提供了立刻刷新的机制。

例如：快速滑动tableView时，这种场景并不特殊。如果每个layer更新都要系统刷新一遍，那么会导致紊乱的帧率，有时特别卡有时又很闲。

所以机制是反过来的，系统有基本稳定的刷新频率(即屏幕刷新频率)，然后在layer内容(注意：是内容不是Frame)改变的时候，把这个layer做个**需要刷新**的标记，这就是setNeedsDisplay。每次刷新时，把上次刷新之后被标记的`layer`一次性全部提交给`Render Server`，所以这里还有一个东西，就是**事务(CATransaction)**。

`layer`刷新就是被调用`display`，但这个我们不主动调用，让系统调用，它可以把握更好的时机。我们只需要`setNeedsDisplay`做标记。如果你真的非常急需，就用`displayIfNeeded`,对于已被标记为`Needed`的layer就立马刷新。



这个过程主要涉及到三个方法：

- `setNeedsDisplay`
- `displayIfNeeded`
- display:这个方法是被动调用的，被系统调用的(固定的刷新频率下或者主动的调用`dispalyIfNeeded`)



#### CALayer的内容显示

------

当创建一个`UIView`的时候，`UIView`会调用其一个私有的方法`+(void)_createLayerWithFrame`，来创建一个`Layer`。此时，这个`Layer`的`contents`是空的，它只有一个backgroudColor(默认白色)。

我们知道，`CALayer`是由三部分组成的：`backgroudColor`、`contents`、border。这里的讨论，只涉及`contents`，绘制的过程中，会涉及下面几个方法：

- `display`
- `delegate`的`displayLayer:`
- `drawInContext:`
- `delegate`的`drawLayer:inContext:`

这四个方法，但凡有一个方法实现了，就不会继续往下进行了，就认为你已经提供了内容。第1和第2个方法是对应的，第3和第4个方法也是对应的，前面两个没有构建内容缓冲区(Backing Store),需要直接提供contents,一种方法就是直接赋值一个`CGImageRef:`

后两种方法，会给`layer`开辟一块内存用来绘制内容，在这两个方法里，可以使用`CoreGraphics`来绘制内容

从上面还可以搞清楚一个问题，就是**layer的delegate的作用:delegate控制layer的内容**，这也是为什么UIView自带的layer的delegate是默认指定到view自身的，而也因为这样，绝大多数时候我们直接修改view的属性(颜色位置透明度等等)，layer的呈现就自动发生变化了。

下面是测试代码：

```
class CATestView:UIView {
    override class var layerClass: AnyClass {
        return CATestLayer.self
    }
    
    override func display(_ layer: CALayer) {
        layer.backgroundColor = UIColor.red.cgColor
    }
    
    override func draw(_ layer: CALayer, in ctx: CGContext) {
        super.draw(layer, in: ctx)
    }
    
    override func draw(_ rect: CGRect) {
        super.draw(rect)
    }
    /// 当且仅当layer内部实现draw方法或者view实现draw方法的时候，才会被调用，并且在draw之前调用
    override func layerWillDraw(_ layer: CALayer) {
        super.layerWillDraw(layer)
    }
    
    override func layoutSublayers(of layer: CALayer) {
        super.layoutSublayers(of: layer)
    }
}

class CATestLayer:CALayer {
    /// 如果实现了这个方法，`layer`的`draw`和`view`的`display`以及`view`的`draw`方法都不执行(前提是不执行`super.display`	，否则下相当于这个方法没有实现)
    override func display() {
        /// 如果里面调用了super，还是会继续往下走
        super.display()
    }
    
    override func draw(in ctx: CGContext) {
        /// 如果里面调用了super，还是会继续往下走
        super.draw(in: ctx)
    }
}
```

有几点需要注意的是：

`layoutSublayers:`方法，首先被调用。之后才是展示内容的方法被调用



#### CALayerDelegate

------

###### \-(void)displayLayer:(CALayer *)layer;



###### \- (void)drawLayer:(CALayer *)layer inContext:(CGContextRef)ctx;



###### \- (void)layerWillDraw:(CALayer *)layer



###### \- (void)layoutSublayersOfLayer:(CALayer *)layer;



###### \- (nullable id<CAAction>)actionForLayer:(CALayer *)layer forKey:(NSString *)event;





#### UIView 的刷新方法

-------

- setNeedsLayout
- layoutIfNeed
- setNeedsDisplay
- setNeedsDisplayInRect



