## isa_t解析

----

源码在isa.h和objc-private.h文件中，链接：

- isa.h：https://opensource.apple.com/source/objc4/objc4-781/runtime/isa.h.auto.html
- objc-private.h：https://opensource.apple.com/source/objc4/objc4-781/runtime/objc-private.h.auto.html

源码：

```c++
// objc4-750 之后
union isa_t {
    isa_t() { }
    isa_t(uintptr_t value) : bits(value) { }
    // Class的的具体类型是objc_class，见文章objc_object和objc_class
    Class cls;
    uintptr_t bits;
#if defined(ISA_BITFIELD)
    struct {
        ISA_BITFIELD;  // defined in isa.h
    };
#endif
};

// objc4-723及之前的版本
union isa_t 
{
    isa_t() { }
    isa_t(uintptr_t value) : bits(value) { }

    Class cls;
    uintptr_t bits;

#if SUPPORT_PACKED_ISA

    // extra_rc must be the MSB-most field (so it matches carry/overflow flags)
    // nonpointer must be the LSB (fixme or get rid of it)
    // shiftcls must occupy the same bits that a real class pointer would
    // bits + RC_ONE is equivalent to extra_rc + 1
    // RC_HALF is the high bit of extra_rc (i.e. half of its range)

    // future expansion:
    // uintptr_t fast_rr : 1;     // no r/r overrides
    // uintptr_t lock : 2;        // lock for atomic property, @synch
    // uintptr_t extraBytes : 1;  // allocated with extra bytes

// 如果是arm64架构
# if __arm64__
#   define ISA_MASK        0x0000000ffffffff8ULL // 掩码
#   define ISA_MAGIC_MASK  0x000003f000000001ULL
#   define ISA_MAGIC_VALUE 0x000001a000000001ULL
    struct {
        uintptr_t nonpointer        : 1;
        uintptr_t has_assoc         : 1;
        uintptr_t has_cxx_dtor      : 1;
        uintptr_t shiftcls          : 33; // MACH_VM_MAX_ADDRESS 0x1000000000
        uintptr_t magic             : 6;
        uintptr_t weakly_referenced : 1;
        uintptr_t deallocating      : 1;
        uintptr_t has_sidetable_rc  : 1;
        uintptr_t extra_rc          : 19;
#       define RC_ONE   (1ULL<<45)
#       define RC_HALF  (1ULL<<18)
    };

// 如果是x86_64架构
# elif __x86_64__
#   define ISA_MASK        0x00007ffffffffff8ULL
#   define ISA_MAGIC_MASK  0x001f800000000001ULL
#   define ISA_MAGIC_VALUE 0x001d800000000001ULL
    struct {
        // 0 代表普通的指针，存储着类对象、元类对象的内存地址
        // 1 代表优化后的使用位域存储更多的信息
        uintptr_t nonpointer        : 1;
        // 是否有设置过关联对象，如果没有释放时会更快
        uintptr_t has_assoc         : 1;
        // 是否有C++析构函数，如果没有，释放时更快
        uintptr_t has_cxx_dtor      : 1;
        // 存储着类对象、元类对象的内存地址信息
        uintptr_t shiftcls          : 44; // MACH_VM_MAX_ADDRESS 0x7fffffe00000
        // 用于在调试时，分辨对象是否未完成初始化
        uintptr_t magic             : 6;
        // 是否有被弱引用引用过
        uintptr_t weakly_referenced : 1;
        // 对象是否正在释放
        uintptr_t deallocating      : 1;
        // 引用计数是否过大无法存储在isa中
        // 如果为1，那么引用计数会存储在一个叫SideTable的类的属性中
        uintptr_t has_sidetable_rc  : 1;
        // 里面存储的值是引用计数器减1
        uintptr_t extra_rc          : 8;
#       define RC_ONE   (1ULL<<56)
#       define RC_HALF  (1ULL<<7)
    };

# else
#   error unknown architecture for packed isa
# endif

// 至此我们已经对isa指针有了新的认识，__arm64__架构之后，isa指针不单单只存储了类对象和元类对象的内存地址，而是使用共用体的方式存储了更多信息，其中shiftcls存储了类对象和元类对象的内存地址，需要同ISA_MASK进行按位与 &运算才可以取出其内存地址值。
  
// SUPPORT_PACKED_ISA
#endif


#if SUPPORT_INDEXED_ISA

# if  __ARM_ARCH_7K__ >= 2

#   define ISA_INDEX_IS_NPI      1
#   define ISA_INDEX_MASK        0x0001FFFC
#   define ISA_INDEX_SHIFT       2
#   define ISA_INDEX_BITS        15
#   define ISA_INDEX_COUNT       (1 << ISA_INDEX_BITS)
#   define ISA_INDEX_MAGIC_MASK  0x001E0001
#   define ISA_INDEX_MAGIC_VALUE 0x001C0001
    struct {
        uintptr_t nonpointer        : 1;
        uintptr_t has_assoc         : 1;
        uintptr_t indexcls          : 15;
        uintptr_t magic             : 4;
        uintptr_t has_cxx_dtor      : 1;
        uintptr_t weakly_referenced : 1;
        uintptr_t deallocating      : 1;
        uintptr_t has_sidetable_rc  : 1;
        uintptr_t extra_rc          : 7;
#       define RC_ONE   (1ULL<<25)
#       define RC_HALF  (1ULL<<6)
    };

# else
#   error unknown architecture for indexed isa
# endif

// SUPPORT_INDEXED_ISA
#endif

};
```

什么是union:

union 是c++中的关键字，翻译为共用体或者联合体。当多个基本数据类型或者复合同一片内存时，我们使用联合体；当多种类型，多个对象，多个事物只取其一时（我们姑且通俗地称其为“n 选1”），我们也可以使用联合体来发挥其长处  

为了方便下面知识的理解，我们下来了解一个概念：位域

位域是指在信息存储的时候，并不需要占用一个完整的字节，而只需占一个或几个二进制位

