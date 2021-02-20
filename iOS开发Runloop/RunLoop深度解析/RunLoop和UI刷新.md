## RunLoop和UI刷新

-------

iOS基本的绘制操作(UIKit)是由`CoreAnimation`来完成的，而`CoreAnimation`的低层是依赖`Core Graphics`和`OpenGL ES`。`Core Graphics`用于`CPU渲染`，而`OpenGL ES`用于`GPU`渲染

理论基础：

```markdown
`View Update Cycle`是在每次`RunLoop`的末尾，`CoreAnimation`会将当前的UI变化以事务的形式提交到`Render Server`。具体的实现是：`CoreAnimation`会监听`RunLoop`状态，如果是`kCFRunLoopBeforeWaiting(即将进入休眠)`或者是`kCFRunLoopExit(即将推出RunLoop)`这两个状态的话，`CoreAnimation`会自动提交当前的所有关于`UI的改动`
```

