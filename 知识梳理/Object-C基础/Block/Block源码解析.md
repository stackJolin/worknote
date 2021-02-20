## Block内存详解

这一个专题，主要研究一下几个问题：

- `__NSConcreteGlobalBlock__`、`__NSConcreteStackBlock__`和`__NSConcreteMallocBlock__`的C++源码研究，既说明为什么Block是一个对象
- Block中引用局部变量的时候，C++源码研究
- Block中修改外部变量的时候，C++源码研究
- __block和问题和auto变量(自动变量即是局部变量)的问题
- 循环引用和接触循环引用的问题



#### 先来介绍一下runtime中和Block相关的数据结构

----

Block相关的数据结构都在runtime下，Block_private.h文件中，该文件链接：https://opensource.apple.com/source/libclosure/libclosure-67/Block_private.h.auto.html

先介绍最重要的四个结构体，也就是Block的真实面目：

```c++
struct Block_layout {
    void *isa;
    volatile int32_t flags; // contains ref count
    int32_t reserved; 
    void (*invoke)(void *, ...);
    // block附加描述信息，主要保存了内存size以及copy和dispose函数的指针及签名和layout等信息，通过源码可发现，layout中只包含了Block_descriptor_1，并未包含Block_descriptor_2和Block_descriptor_3，这是因为在捕获不同类型变量或者没用到外部变量时，编译器会改变结构体的结构，按需添加Block_descriptor_2和Block_descriptor_3，所以才需要BLOCK_HAS_COPY_DISPOSE和BLOCK_HAS_SIGNATURE等枚举来判断
    struct Block_descriptor_1 *descriptor;
    // imported variables
};

struct Block_descriptor_1 {
    uintptr_t reserved; // 保留字
    uintptr_t size; // Block的size
};

struct Block_descriptor_2 {
    // requires BLOCK_HAS_COPY_DISPOSE
    void (*copy)(void *dst, const void *src);
    void (*dispose)(const void *);
};

struct Block_descriptor_3 {
    // requires BLOCK_HAS_SIGNATURE
    const char *signature;
    const char *layout;     // contents depend on BLOCK_HAS_EXTENDED_LAYOUT
};
```

我们挨个介绍：

<font color=orange>_Block_layout:</font>

1. isa 指针，所有对象都有该指针，用于实现对象相关的功能。
2. flags，用于按 bit 位表示一些 block 的附加信息，本文后面介绍 block copy 的实现代码可以看到对该变量的使用。
3. reserved，保留变量。
4. invoke，函数指针，指向具体的 block 实现的函数调用地址。
5. descriptor， 表示该 block 的附加描述信息，主要是 size 大小，以及 copy 和 dispose 函数的指针。
6. variables，捕获过来的变量，block 能够访问它外部的局部变量，就是因为将这些变量（或变量的地址）复制到了结构体中。

<font color=orange>Block_descriptor_1、Block_descriptor_2、Block_descriptor_3：</font>

正如我们看到的，该文件中，有三种Block_descriptor_x的结构体，然后Block_Layout的定义中，只有Block_descritor_1类型的，那么其他两种有什么用处呢？

其实，到底Block_layout该使用上面三个结构体中的哪一种，是根据flags字段来确定的。那么，编译器是怎么判断的呢？

我们先来看一下定义在Block_private.h中的一个枚举结构：

```c++
// Values for Block_layout->flags to describe block objects
// 通过注释可以看到，下面的这几个枚举，有些是运行时用到的，有些是编译期间用到的
enum {
    BLOCK_DEALLOCATING =      (0x0001),  // runtime 
    BLOCK_REFCOUNT_MASK =     (0xfffe),  // runtime 用于标识 栈区 block
    BLOCK_NEEDS_FREE =        (1 << 24), // runtime 用于标识 堆区 block
    BLOCK_HAS_COPY_DISPOSE =  (1 << 25), // compiler block的discriptor含有copy和dispose助手/方法
    BLOCK_HAS_CTOR =          (1 << 26), // compiler: helpers have C++ code block中有C++代码
    BLOCK_IS_GC =             (1 << 27), // runtime
    BLOCK_IS_GLOBAL =         (1 << 28), // compiler 用于标识 全局 block 
    BLOCK_USE_STRET =         (1 << 29), // compiler: undefined if !BLOCK_HAS_SIGNATURE
    BLOCK_HAS_SIGNATURE  =    (1 << 30), // compiler block的函数有签名信息
    BLOCK_HAS_EXTENDED_LAYOUT=(1 << 31)  // compiler block中有访问外部的变量和对象
};

// 扩展布局信息编码
// Extended layout encoding.

// Values for Block_descriptor_3->layout with BLOCK_HAS_EXTENDED_LAYOUT
// and for Block_byref_3->layout with BLOCK_BYREF_LAYOUT_EXTENDED

// If the layout field is less than 0x1000, then it is a compact encoding 
// of the form 0xXYZ: X strong pointers, then Y byref pointers, 
// then Z weak pointers.

// If the layout field is 0x1000 or greater, it points to a 
// string of layout bytes. Each byte is of the form 0xPN.
// Operator P is from the list below. Value N is a parameter for the operator.
// Byte 0x00 terminates the layout; remaining block data is non-pointer bytes.

// 当layout的值小于0x1000时，则是一个压缩的扩展布局描述，其格式是0xXYZ，其中的X的值表示的是block中应用的外部被声明为strong类型的对象数量，Y值则是block中引用的外部被声明为__block类型的变量数量，而Z则是block中引用的外部被声明为__weak的对象数量
// 而当layout的值大于0x1000时，则是一个以0结束的字符串指针，字符串的每个字节的格式是0xPN,也就是每个字节中的高4位bit表示的是引用外部对象的类型，而低4位bit则是这个对象类型的数量
// layout的信息只是记录了一个Block对象，引用了外部对象的布局信息描述，对于普通类型的则不会记录。需要注意的是，系统会把引用对象排列在前面，而引用的普通数据类型则排列在后面

enum {
    BLOCK_LAYOUT_ESCAPE = 0, // N=0 halt, rest is non-pointer. N!=0 reserved.
    BLOCK_LAYOUT_NON_OBJECT_BYTES = 1,    // N bytes non-objects
    BLOCK_LAYOUT_NON_OBJECT_WORDS = 2,    // N words non-objects
    BLOCK_LAYOUT_STRONG           = 3,    // N words strong pointers
    BLOCK_LAYOUT_BYREF            = 4,    // N words byref pointers
    BLOCK_LAYOUT_WEAK             = 5,    // N words weak pointers
    BLOCK_LAYOUT_UNRETAINED       = 6,    // N words unretained pointers
    BLOCK_LAYOUT_UNKNOWN_WORDS_7  = 7,    // N words, reserved
    BLOCK_LAYOUT_UNKNOWN_WORDS_8  = 8,    // N words, reserved
    BLOCK_LAYOUT_UNKNOWN_WORDS_9  = 9,    // N words, reserved
    BLOCK_LAYOUT_UNKNOWN_WORDS_A  = 0xA,  // N words, reserved
    BLOCK_LAYOUT_UNUSED_B         = 0xB,  // unspecified, reserved
    BLOCK_LAYOUT_UNUSED_C         = 0xC,  // unspecified, reserved
    BLOCK_LAYOUT_UNUSED_D         = 0xD,  // unspecified, reserved
    BLOCK_LAYOUT_UNUSED_E         = 0xE,  // unspecified, reserved
    BLOCK_LAYOUT_UNUSED_F         = 0xF,  // unspecified, reserved
};
```



#### _Block_byref

-------

```c++

struct Block_byref {
    void *isa; // 指向父类，一般执行0
    struct Block_byref *forwarding; // 在占中指向自己，block copy后指向堆中的byref
    volatile int32_t flags; // contains ref count
    // block byref所占内存的大小
    uint32_t size;
};

// 用来保存附加信息
struct Block_byref_2 {
    // requires BLOCK_BYREF_HAS_COPY_DISPOSE
    BlockByrefKeepFunction byref_keep;
    BlockByrefDestroyFunction byref_destroy;
};
// 用来保存附加信息
struct Block_byref_3 {
    // requires BLOCK_BYREF_LAYOUT_EXTENDED
    const char *layout;
};

// Values for Block_byref->flags to describe __block variables
enum {
    // Byref refcount must use the same bits as Block_layout's refcount.
    // BLOCK_DEALLOCATING =      (0x0001),  // runtime
    // BLOCK_REFCOUNT_MASK =     (0xfffe),  // runtime

    BLOCK_BYREF_LAYOUT_MASK =       (0xf << 28), // compiler
    // 表示含有layout
    BLOCK_BYREF_LAYOUT_EXTENDED =   (  1 << 28), // compiler
    BLOCK_BYREF_LAYOUT_NON_OBJECT = (  2 << 28), // compiler
    BLOCK_BYREF_LAYOUT_STRONG =     (  3 << 28), // compiler
    BLOCK_BYREF_LAYOUT_WEAK =       (  4 << 28), // compiler
    BLOCK_BYREF_LAYOUT_UNRETAINED = (  5 << 28), // compiler

    BLOCK_BYREF_IS_GC =             (  1 << 27), // runtime
    // 表示byref中含有copy_dispose函数，在__block捕获的变量为对象时，就会生成copy_dispose函数，用来管理对象内存
    BLOCK_BYREF_HAS_COPY_DISPOSE =  (  1 << 25), // compiler
    // 判断是否要释放
    BLOCK_BYREF_NEEDS_FREE =        (  1 << 24), // runtime
};
```

变量在被`__block`修饰的时候，由编译器生成的



### _NSConcreteGlobalBlock

-----

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
        // 这里是_NSConcreteStackBlock而不是_NSConcreteGlobalBlock，原因是这里使用的是clang编译，而不是llvm。如果是llvm的话，就会是_NSConcreteGlobalBlock
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



#### _NSConcreteStaticBlock

----------

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



#### __block修饰过的局部变量的block

---------

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



总结：

- `_NSConcreteGlobalBlock__`：没有捕获局部变量的block(可以捕获全局变量、局部静态变量、全局静态变量)
- `_NSConcreteStackBlock`：捕获了局部变量的block
- `_NSConcreteMallocBlock`：上面两种`block`执行copy操作，就会生成一个`__NSMallocBlock`



#### 相关文章

-----

https://www.jianshu.com/p/4a6dca34d980

https://github.com/nst/iOS-Runtime-Headers/tree/master/Frameworks/CoreFoundation.framework

block源码：https://opensource.apple.com/source/libclosure/libclosure-67/

https://www.programmersought.com/article/34684052828/

https://www.jianshu.com/p/d96d27819679

https://www.stephenw.cc/2017/06/11/objc-block/

Clang下(ABI)对于block的描述：https://clang.llvm.org/docs/Block-ABI-Apple.html

https://cloud.tencent.com/developer/article/1508380

#### 目录

----

- 内存类型的角度划分block类型

- block内存管理

- block防止内存引用

  