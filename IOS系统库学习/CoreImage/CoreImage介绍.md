## CoreImage 介绍

-------

`iOS 5`引入的一个图像处理框架，提供了强大高效的图像处理功能，用来对基于像素的图像进行操作和分析，内置了很多强大的滤镜(目前超过了180多种)。而且这些滤镜，都是支持`滤镜链`，也就是设计模式中的装饰者模式。除此之外，在`iOS8`之后，可以自定义`CIFilter`

重点：

- 支持180多种滤镜
- 可以自定义CIFilter
- 滤镜链 - 装饰者模式

`Core Image`的`API`主要分为三种：

- `CIImage`保存图像数据的类，可以通过`UIImage`等数据源来创建
- `CIFilter`，滤镜类
- `CIContext`，表示上下文。和`Core Graphics`的上下文一样，`Core Image`的上下文也是实现对图像处理的具体对象，可以从中获取图片信息。另外，`CIContext`可以利用`OpenGL`做处理，用来提供性能



