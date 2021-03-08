## CIFilter滤镜大全

------







#### CISourceOverCompositing

---------

`源覆盖组合`滤镜，滤镜把前景图（inputImage）覆盖在背景图（inputBackgroundImage）上

```objective-c
- (CIImage *)addToBgImg:(CIImage *)image {
    CIFilter *filter = [CIFilter filterWithName:@"CISourceOverCompositing"];
    [filter setDefaults];
    [filter setValue:self forKey:@"inputImage"];
    [filter setValue:image forKey:@"inputBackgroundImage"];
    return filter.outputImage;
}
```











#### 相关文章

------

https://www.jianshu.com/p/926227e46516

