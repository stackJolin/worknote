## CF对象和OC对象的转换

-------

OC对象指的是Objective-C对象，CF对象指的是Core Foundation对象

首先需要说明的是：ARC 环境下编译器不会自动管理 CF 对象的内存，所以当创建一个 CF 对象时，使用结束后要用 CFRelease 函数将其手动释放，CF 对象和 OC 对象相互转化的时候该如何管理内存，有如下三个关键字：

- `__bridge`：CF对象和 OC 对象转化时只涉及对象类型，不涉及对象和所有权的转化。其本身的意思是：ARC 仍然具备这个 OC 对象的所有权
- `__bridge_retained`：(与_bridge_transfer相反)常用在 OC 对象转换成 CF 对象，将 OC 对象的所有权交给 CF 对象来管理，作用同（CFBridgingRetain())，意味着 ARC 将交出对象的所有权。
- `__bridge_transfer`：常用在 CF 对象装换成 OC 对象时，将 CF 对象的所有权交给 OC 对象，此时 ARC 就可以自动管理内存；作用和 CFBridgingRelease（）相同



#### OC对象转CF对象

------

```objective-c
- (void)OCConvertCF {
    NSString *stringOC = @"OC对象 --> CF对象";

    CFStringRef stringCF1 = (__bridge CFStringRef)stringOC; // 方式一
    CFStringRef stringCF2 = (__bridge_retained CFStringRef)stringOC; // 方式二，但stringCF2引用计数会加1

    NSLog(@"%@", stringCF1);
    NSLog(@"%@", stringCF2);
}
```



#### CF对象转OC对象

--------

```objective-c
- (void)OFConvertOC {

    CFStringRef stringCF = (CFStringRef)@"OF对象 --> OC对象";

    NSString *stringOC1 = (__bridge NSString *)stringCF; // 方式一
    NSString *stringOC2 = (__bridge_transfer NSString *)stringCF; // 方式二

    NSLog(@"%@", stringOC1);
    NSLog(@"%@", stringOC2);
}
```









