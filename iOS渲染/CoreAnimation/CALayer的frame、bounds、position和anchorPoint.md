## CALayer的frame、bounds、position和anchorPoint

---------

###### frame:

```markdown
描述当前视图的大小和在父视图中的位置
```

###### bounds:

```markdown
描述当前视图的大小，以及左上角相对于视图原点的位置
```

###### anchorPoint：

```markdown
`anchorPoint`属于`iOS CoreAnimation`层，是一个变换的`支点`。anchorPoint的默认值是`(0.5, 0.5)`，取值范围是`(0~1,0~1)`
```

###### position:

```markdown
`anchorPoint`这个点，在`superLayer`的坐标
```

Note:

```markdown
需要注意的是，`bounds`、`position`、`anchorPoint`以及`transfrom`是存储属性，而`frame`是计算属性
所以，当layer执行仿射变换的时候，frame的origin和size都会改变，但是bounds和postion并不会改变
```



#### 不同属性的改变结果

----------

###### 修改postition:

- `frame`改变，`anchorpoint`不变

###### 修改frame:

- `position`改变，`anchorpoint`不变

###### 修改`anchorpoint`：

- `frame`改变，`position不变`

