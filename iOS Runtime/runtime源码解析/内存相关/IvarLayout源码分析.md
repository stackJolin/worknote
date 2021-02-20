## IvarLayout源码分析

------

Runtime中关于`IvarLayout`的几个方法：

```objective-c
const uint8_t * _Nullable class_getIvarLayout(Class _Nullable cls)

const uint8_t * _Nullable class_getWeakIvarLayout(Class _Nullable cls)

void class_setIvarLayout(Class _Nullable cls, const uint8_t * _Nullable layout)

void class_setWeakIvarLayout(Class _Nullable cls, const uint8_t * _Nullable layout)
```

