## Block内存详解

这一个专题，主要研究一下几个问题：

- NSGlobalBlock、NSStackBlock和NSMallocBlock的C++源码研究，既说明为什么Block是一个对象
- Block中引用局部变量的时候，C++源码研究
- Block中修改外部变量的时候，C++源码研究
- __block和问题和auto变量的问题
- 循环引用和接触循环引用的问题



#### 为什么说Block是一个对象

-----

<font color=orange>**NSGlobalBlock(没有引用非全局变量的block)-C++**</font>

原始代码：

```objective-c
int main(int argc, char * argv[]) {
    @autoreleasepool {
        ^(int a) {
					  int b = a;
            int c = b;
        };
    }
    return 0;
}
```

转换`xcrun -sdk iphoneos clang -rewrite-objc main.m -o main_cpp.cpp`：

```objective-c
int main(int argc, char * argv[]) {
    /* @autoreleasepool */ { __AtAutoreleasePool __autoreleasepool; 
        ((void (*)(int))&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA));
    }
    return 0;
}

struct __main_block_impl_0 {
    struct __block_impl impl;
    struct __main_block_desc_0* Desc;
    // C++构造方法，第一个参数是函数指针
    __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, int flags=0) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself, int a) {
    int b = a;
    int c = b;
}

static struct __main_block_desc_0 {
    size_t reserved;
    size_t Block_size; 
} __main_block_desc_0_DATA = { 0, sizeof(struct __main_block_impl_0)};

struct __block_impl {
    void *isa;
    int Flags;
    int Reserved;
    void *FuncPtr;
};
```

<font color=orange>**NSStaticBlock(引用了外部变量(非全局变量)的block)-C++**</font>

源代码:

```objective-c
int main(int argc, char * argv[]) {
    @autoreleasepool {
				int d = 10;
        ^(int a) {
            int b = a;
            int c = d
        };
        
    }
    return 0;
}
```

C++代码：

```c++
int main(int argc, char * argv[]) {
    /* @autoreleasepool */ { __AtAutoreleasePool __autoreleasepool; 
        int d = 10;
        ((void (*)(int))&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, d));
    }
    return 0;
}

struct __main_block_impl_0 {
    struct __block_impl impl;
    struct __main_block_desc_0* Desc;
    int d;
    __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, int _d, int flags=0) : d(_d) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};

struct __block_impl {
    void *isa;
    int Flags;
    int Reserved;
    void *FuncPtr;
};

static struct __main_block_desc_0 {
    size_t reserved;
    size_t Block_size;
} __main_block_desc_0_DATA = { 0, sizeof(struct __main_block_impl_0)};

static void __main_block_func_0(struct __main_block_impl_0 *__cself, int a) {
		int d = __cself->d; // bound by copy
    int b = a;
    int c = d;
}
```



<font color=orange>**NSStaticBlock(引用了外部变量(非全局变量)的block)-C++**</font>

原始代码:

```objective-c
int main(int argc, char * argv[]) {
    @autoreleasepool {
        __block int d = 10;
        ^(int a) {
            int b = a;
            d = 20;
        };
    }
    return 0;
}
```

C++代码：

```c++
int main(int argc, char * argv[]) {
    /* @autoreleasepool */ { __AtAutoreleasePool __autoreleasepool; 
        __attribute__((__blocks__(byref))) __Block_byref_d_0 d = {
        		(void*)0,
          	(__Block_byref_d_0 *)&d, 
          	0, 
          	sizeof(__Block_byref_d_0), 
          	10
        };
        ((void (*)(int))&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, (__Block_byref_d_0 *)&d, 570425344));

    }
    return 0;
}

struct __main_block_impl_0 {
    struct __block_impl impl;
    struct __main_block_desc_0* Desc;
    __Block_byref_d_0 *d; // by ref
    __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, __Block_byref_d_0 *_d, int flags=0) : d(_d->__forwarding) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};

struct __block_impl {
    void *isa;
    int Flags;
    int Reserved;
    void *FuncPtr;
};

static struct __main_block_desc_0 {
    size_t reserved;
    size_t Block_size;
    void (*copy)(struct __main_block_impl_0*, struct __main_block_impl_0*);
    void (*dispose)(struct __main_block_impl_0*);
} __main_block_desc_0_DATA = { 0, sizeof(struct __main_block_impl_0), __main_block_copy_0, __main_block_dispose_0};

struct __Block_byref_d_0 {
    void *__isa;
    __Block_byref_d_0 *__forwarding;
    int __flags;
    int __size;
    int d;
};

static void __main_block_copy_0(struct __main_block_impl_0*dst, struct __main_block_impl_0*src) {
  	_Block_object_assign((void*)&dst->d, (void*)src->d, 8/*BLOCK_FIELD_IS_BYREF*/);
}

static void __main_block_dispose_0(struct __main_block_impl_0*src) {
  	_Block_object_dispose((void*)src->d, 8/*BLOCK_FIELD_IS_BYREF*/);
}

static void __main_block_func_0(struct __main_block_impl_0 *__cself, int a) {
  	__Block_byref_d_0 *d = __cself->d; // bound by ref
  	int b = a;
  	(d->__forwarding->d) = 20;
}
```

我们看到，有两个内存相关的函数`__main_block_copy_0`和`__main_block_dispose_0`，生成的C++代码文件里，并没有。其实，它属于runtime。我们会有其他文章详细讲解着两个函数

#### 相关文章

-----

https://www.jianshu.com/p/4a6dca34d980

https://github.com/nst/iOS-Runtime-Headers/tree/master/Frameworks/CoreFoundation.framework

block源码：https://opensource.apple.com/source/libclosure/libclosure-67/



#### 目录

----

- 内存类型的角度划分block类型

- block内存管理

- block防止内存引用

  