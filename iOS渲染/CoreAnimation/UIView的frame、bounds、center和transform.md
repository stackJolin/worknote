## UIView的frame、bounds、center和transform

---------

###### frame：

```markdown
描述当前视图在其父视图中的位置和大小
```

###### bounds:

```markdown
描述当前视图在其自身坐标系统中的位置和大小
```

###### center:

```markdown
描述当前视图的中心点在其父视图中的位置
```



`frame`和bounds除了都影响`view`的大小之外，对于一个`view`来说，比如说叫做`View B`，改变`frame`的`origin`会改变`View B`在父视图中的位置，而改变`bounds`的`origin`会改变`View B`中的子视图在`View B`中的`展示位置`

`bounds`的`origin`属性，代表的是坐标原点，它是以实际的视图展示的左上角在`origin`的坐标原点的位置来表示的。比如：

```objective-c
view.bounds = CGRect(x: -50, y: -50, width: 200, height: 200)
// 此时的view的左上角，在本地参考系中的坐标是(-50，-50)，也就是原点向`右-下`各偏移了50
```





#### UIView的frame和transform

--------

