## TextKit实现UILabel



#### TextKit的布局流程

-------

- 创建文本容器类：NSTextContainer，TextContainer-A
- 创建布局管理类：NSLayoutManager，TextLayoutManager-A
- 将`TextContainer-A`添加到`TextLayoutManager-A`
- 创建一个文本信息存储类：NSTextStorage，`TextStorage-A`
- 将布局管理类`TextLayoutManager-A`添加进入`TextStorage-A`中
- 将TextContainer和UILabel绑定