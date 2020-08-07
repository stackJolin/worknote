## Block源码详解之内存管理

我们在'Block源码详解'中，了解了block在C++层面的工作机制。这篇文章，我们主要来看，‘Block源码详解’文章中遗留的历史问题：

`_Block_object_dispose`和`_Block_object_assign`的工作原理

上面两个函数，在我们用clang生成的C++代码里面是没有实现的，因为他们是在runtime中

源码链接：https://opensource.apple.com/source/libclosure/libclosure-67/

上面两个具体的方法所在文件链接：https://opensource.apple.com/source/libclosure/libclosure-67/runtime.c.auto.html

嗯。。。。。。。。。

在具体解析上面两个方法之前，我们先来熟悉下面几个函数，因为，上面两个函数的工作过程，都是依赖于此

<font color=orange>**_Block_copy和 _Block_release：**</font>

源码链接：https://opensource.apple.com/source/libclosure/libclosure-67/runtime.c.auto.html

```c++
static int32_t latching_incr_int(volatile int32_t *where) {
    // TODO：zhuhoulin，这里为什么使用循环？个人感觉和自旋锁一样，提升效率
    while (1) {
        // 获取当前的Block_layout的flag
        int32_t old_value = *where;
        // 如果引用计数达到最大，直接返回，30000多个指针指向，一般不会出现
        if ((old_value & BLOCK_REFCOUNT_MASK) == BLOCK_REFCOUNT_MASK) { 
            return BLOCK_REFCOUNT_MASK;
        }
        // 随后做一次原子性判断其值当前是否被其他线程改动，如果被改动就进入下一次循环直到改动结束后赋值。
        // OSAtomicCompareAndSwapInt的作用就是在where取值与old_value相同时，将old_value+2赋给where。
        // 需要知道的是，Block的引用计数以flags的后16位代表，以 2为单位，每次递增2，1被BLOCK_DEALLOCATING正在释放占用。
        if (OSAtomicCompareAndSwapInt(old_value, old_value+2, where)) {
            return old_value+2;
        }
    }
}

#if 0
static struct Block_descriptor_1 * _Block_descriptor_1(struct Block_layout *aBlock)
{
    return aBlock->descriptor;
}
#endif

static struct Block_descriptor_2 * _Block_descriptor_2(struct Block_layout *aBlock)
{
    // 如果flag没有标记为支持copy和dispose助手，return
    if (! (aBlock->flags & BLOCK_HAS_COPY_DISPOSE)) return NULL;
    // 获取aBlock的descriptor地址，这里能够知道，这个aBlock->descriptor是Block_descriptor_1类型的
    uint8_t *desc = (uint8_t *)aBlock->descriptor;
    // 移动指针，指向Block_descriprot_2类型的指针,并返回
    desc += sizeof(struct Block_descriptor_1);
    return (struct Block_descriptor_2 *)desc;
}

static struct Block_descriptor_3 * _Block_descriptor_3(struct Block_layout *aBlock)
{
    // 如果flag没有标记为支持copy和dispose助手，return
    if (! (aBlock->flags & BLOCK_HAS_SIGNATURE)) return NULL;
    // 获取aBlock的descriptor地址，这里能够知道，这个aBlock->descriptor是Block_descriptor_1类型的
    uint8_t *desc = (uint8_t *)aBlock->descriptor;
    // 移动指针，指向Block_descriprot_2类型的指针,并返回
    desc += sizeof(struct Block_descriptor_1);
    // 如果有dispose和copy助手，继续移动指针
    if (aBlock->flags & BLOCK_HAS_COPY_DISPOSE) {
        desc += sizeof(struct Block_descriptor_2);
    }
    return (struct Block_descriptor_3 *)desc;
}

static void _Block_call_copy_helper(void *result, struct Block_layout *aBlock)
{
    struct Block_descriptor_2 *desc = _Block_descriptor_2(aBlock);
    if (!desc) return;
    // 这个实现方法是在编译的时候生成的，详见block源码解析。方法内部调用的是_Block_object_assign方法
    (*desc->copy)(result, aBlock); // do fixup
}

// 这个是编译的时候生成的copy方法,对应的还有dispose方法
static void __main_block_copy_0(struct __main_block_impl_0*dst, struct __main_block_impl_0*src) {
    // 这个d是，外部的变量。编译的时候自动生成的
  	_Block_object_assign((void*)&dst->d, (void*)src->d, 8/*BLOCK_FIELD_IS_BYREF*/);
}
static void __main_block_dispose_0(struct __main_block_impl_0*src) {
  	_Block_object_dispose((void*)src->d, 8/*BLOCK_FIELD_IS_BYREF*/);
}

static void _Block_call_dispose_helper(struct Block_layout *aBlock)
{
    struct Block_descriptor_2 *desc = _Block_descriptor_2(aBlock);
    if (!desc) return;

    (*desc->dispose)(aBlock);
}


void *_Block_copy(const void *arg) {
    // 声明一个Block_layout变量
    struct Block_layout *aBlock;
    // 如果传入的block不存在的话，直接返回
    if (!arg) return NULL;
    
    // The following would be better done as a switch statement
    aBlock = (struct Block_layout *)arg;
    if (aBlock->flags & BLOCK_NEEDS_FREE) { // 如果是一个 堆区 的block，就对其引用计数递增，然后返回Block。
        // latches on high
        latching_incr_int(&aBlock->flags);
        return aBlock;
    }
    else if (aBlock->flags & BLOCK_IS_GLOBAL) { // 如果是一个 全局区 的block,直接返回
        return aBlock;
    }
    else { // 如果是一个栈区的block
        // Its a stack block.  Make a copy.
        // 新分配一块堆区内存，如果分配失败，直接返回
        struct Block_layout *result = malloc(aBlock->descriptor->size);
        if (!result) return NULL;
        
        // 如果分配成功，将栈区的block copy一份到上面初始化的堆区内存
        memmove(result, aBlock, aBlock->descriptor->size); // bitcopy first
        
        // reset refcount
        //            Ox0001                0xFFFE
        // 重置引用计数 BLOCK_REFCOUNT_MASK | BLOCK_DEALLOCATING = oxFFFF,~(BLOCK_REFCOUNT_MASK | BLOCK_DEALLOCATING) = ox0000
        // result->flags与0x0000与等就将result->flags的后16位置零。然后将新Block标识为堆Block并将其引用计数置为2
        result->flags &= ~(BLOCK_REFCOUNT_MASK|BLOCK_DEALLOCATING);    // XXX not needed
        // BLOCK_NEEDS_FREE = Ox0100 0000, result->flags = 0x0100 0002，低16位表示引用计数
        // TODO: 为什么这里的引用计数是+2
        result->flags |= BLOCK_NEEDS_FREE | 2;  // logical refcount 1
        _Block_call_copy_helper(result, aBlock);
        // 修改block类型
        result->isa = _NSConcreteMallocBlock;
        return result;
    }
}

static bool latching_decr_int_should_deallocate(volatile int32_t *where) {
    while (1) {
        int32_t old_value = *where;
        // 引用计数太大，直接返回false
        if ((old_value & BLOCK_REFCOUNT_MASK) == BLOCK_REFCOUNT_MASK) {
            return false; // latched high
        }
        // 引用计数为0，直接返回
        if ((old_value & BLOCK_REFCOUNT_MASK) == 0) {
            return false;   // underflow, latch low
        }
        
        int32_t new_value = old_value - 2;
        bool result = false;
        if ((old_value & (BLOCK_REFCOUNT_MASK|BLOCK_DEALLOCATING)) == 2) {
            new_value = old_value - 1;
            result = true;
        }
        if (OSAtomicCompareAndSwapInt(old_value, new_value, where)) {
            return result;
        }
    }
}

void _Block_release(const void *arg) {
    // block不存在 || block是全局block || block是栈区block ,直接return
    struct Block_layout *aBlock = (struct Block_layout *)arg;
    if (!aBlock) return;
    if (aBlock->flags & BLOCK_IS_GLOBAL) return;
    if (! (aBlock->flags & BLOCK_NEEDS_FREE)) return;

    if (latching_decr_int_should_deallocate(&aBlock->flags)) {
        _Block_call_dispose_helper(aBlock);
        _Block_destructInstance(aBlock);
        free(aBlock);
    }
}
```

<font color=orange>**_Block_retain_object和 _Block_release_object：**</font>

```c++
static void _Block_retain_object_default(const void *ptr __unused) { }

static void _Block_release_object_default(const void *ptr __unused) { }

static void _Block_destructInstance_default(const void *aBlock __unused) {}
// 默认的_Block_retain_object被赋值为_Block_retain_object_default，即什么都不做
static void (*_Block_retain_object)(const void *ptr) = _Block_retain_object_default;
static void (*_Block_release_object)(const void *ptr) = _Block_release_object_default;
static void (*_Block_destructInstance) (const void *aBlock) = _Block_destructInstance_default;

void _Block_use_RR2(const Block_callbacks_RR *callbacks) {
    _Block_retain_object = callbacks->retain;
    _Block_release_object = callbacks->release;
    _Block_destructInstance = callbacks->destructInstance;
}

```

<font color=orange>**_Block_byref_copy和 _Block_byref_release：**</font>

```c++
static struct Block_byref *_Block_byref_copy(const void *arg) {
    struct Block_byref *src = (struct Block_byref *)arg;

    if ((src->forwarding->flags & BLOCK_REFCOUNT_MASK) == 0) {
        // src points to stack
        struct Block_byref *copy = (struct Block_byref *)malloc(src->size);
        copy->isa = NULL;
        // byref value 4 is logical refcount of 2: one for caller, one for stack
        copy->flags = src->flags | BLOCK_BYREF_NEEDS_FREE | 4;
        copy->forwarding = copy; // patch heap copy to point to itself
        src->forwarding = copy;  // patch stack to point to heap copy
        copy->size = src->size;

        if (src->flags & BLOCK_BYREF_HAS_COPY_DISPOSE) {
            // Trust copy helper to copy everything of interest
            // If more than one field shows up in a byref block this is wrong XXX
            struct Block_byref_2 *src2 = (struct Block_byref_2 *)(src+1);
            struct Block_byref_2 *copy2 = (struct Block_byref_2 *)(copy+1);
            copy2->byref_keep = src2->byref_keep;
            copy2->byref_destroy = src2->byref_destroy;

            if (src->flags & BLOCK_BYREF_LAYOUT_EXTENDED) {
                struct Block_byref_3 *src3 = (struct Block_byref_3 *)(src2+1);
                struct Block_byref_3 *copy3 = (struct Block_byref_3*)(copy2+1);
                copy3->layout = src3->layout;
            }

            (*src2->byref_keep)(copy, src);
        }
        else {
            // Bitwise copy.
            // This copy includes Block_byref_3, if any.
            memmove(copy+1, src+1, src->size - sizeof(*src));
        }
    }
    // already copied to heap
    else if ((src->forwarding->flags & BLOCK_BYREF_NEEDS_FREE) == BLOCK_BYREF_NEEDS_FREE) {
        latching_incr_int(&src->forwarding->flags);
    }
    
    return src->forwarding;
}



static void _Block_byref_release(const void *arg) {
    struct Block_byref *byref = (struct Block_byref *)arg;

    // dereference the forwarding pointer since the compiler isn't doing this anymore (ever?)
    byref = byref->forwarding;
    
    if (byref->flags & BLOCK_BYREF_NEEDS_FREE) {
        int32_t refcount = byref->flags & BLOCK_REFCOUNT_MASK;
        os_assert(refcount);
        if (latching_decr_int_should_deallocate(&byref->flags)) {
            if (byref->flags & BLOCK_BYREF_HAS_COPY_DISPOSE) {
                struct Block_byref_2 *byref2 = (struct Block_byref_2 *)(byref+1);
                (*byref2->byref_destroy)(byref);
            }
            free(byref);
        }
    }
}
```



#### `_Block_object_assign` 源码解析:

---

先了解一下，下面几个枚举

```c++
// Values for _Block_object_assign() and _Block_object_dispose() parameters
enum {
    // see function implementation for a more complete description of these fields and combinations
    // OC对象类型
    BLOCK_FIELD_IS_OBJECT   =  3,  // id, NSObject, __attribute__((NSObject)), block, ...
    // 另一个block
    BLOCK_FIELD_IS_BLOCK    =  7,  // a block variable
    // 一个背block修饰后，生成的结构体
    BLOCK_FIELD_IS_BYREF    =  8,  // the on stack structure holding the __block variable
    // 被__weak修饰后的弱引用，只在Block_byref管理内存对象时使用，也就是__block __weak id
    BLOCK_FIELD_IS_WEAK     = 16,  // declared __weak, only used in byref copy helpers
    // 在处理Block_byref内部对象内存的时候会加的一个额外标记，配合上面的枚举一起使用；
    BLOCK_BYREF_CALLER      = 128, // called from __block (byref) copy/dispose support routines.
};

enum {
    BLOCK_ALL_COPY_DISPOSE_FLAGS = 
        BLOCK_FIELD_IS_OBJECT | BLOCK_FIELD_IS_BLOCK | BLOCK_FIELD_IS_BYREF |
        BLOCK_FIELD_IS_WEAK | BLOCK_BYREF_CALLER
};
```





```c++
void _Block_object_assign(void *destArg, const void *object, const int flags) {
    // destArg为执行Block_copy()后的block中的对象、block、或者BYREF指针的指针，obj为copy之前的变量指针
    const void **dest = (const void **)destArg;
    switch (os_assumes(flags & BLOCK_ALL_COPY_DISPOSE_FLAGS)) {
        case BLOCK_FIELD_IS_OBJECT: // 如果外部变量是对象、block或者tyref类型，增加object变量的引用计数
            /*******
            id object = ...;
            [^{ object; } copy];
            ********/

            _Block_retain_object(object);
            *dest = object;
            break;

        case BLOCK_FIELD_IS_BLOCK:
            /*******
            void (^object)(void) = ...;
            [^{ object; } copy];
            ********/

            *dest = _Block_copy(object);
            break;

        case BLOCK_FIELD_IS_BYREF | BLOCK_FIELD_IS_WEAK:
        case BLOCK_FIELD_IS_BYREF:
            /*******
             // copy the onstack __block container to the heap
             // Note this __weak is old GC-weak/MRC-unretained.
             // ARC-style __weak is handled by the copy helper directly.
             __block ... x;
             __weak __block ... x;
             [^{ x; } copy];
             ********/

            *dest = _Block_byref_copy(object);
            break;

        case BLOCK_BYREF_CALLER | BLOCK_FIELD_IS_OBJECT:
        case BLOCK_BYREF_CALLER | BLOCK_FIELD_IS_BLOCK:
            /*******
             // copy the actual field held in the __block container
             // Note this is MRC unretained __block only. 
             // ARC retained __block is handled by the copy helper directly.
             __block id object;
             __block void (^object)(void);
             [^{ object; } copy];
             ********/

            *dest = object;
            break;

        case BLOCK_BYREF_CALLER | BLOCK_FIELD_IS_OBJECT | BLOCK_FIELD_IS_WEAK:
        case BLOCK_BYREF_CALLER | BLOCK_FIELD_IS_BLOCK  | BLOCK_FIELD_IS_WEAK:
            /*******
             // copy the actual field held in the __block container
             // Note this __weak is old GC-weak/MRC-unretained.
             // ARC-style __weak is handled by the copy helper directly.
             __weak __block id object;
             __weak __block void (^object)(void);
             [^{ object; } copy];
             ********/

            *dest = object;
            break;

        default:
          break;
    }
}
```



#### `_Block_object_dispose` 源码解析:

----

```c++
void _Block_object_dispose(const void *object, const int flags) {
    switch (os_assumes(flags & BLOCK_ALL_COPY_DISPOSE_FLAGS)) {
        case BLOCK_FIELD_IS_BYREF | BLOCK_FIELD_IS_WEAK:
        case BLOCK_FIELD_IS_BYREF:
          	// get rid of the __block data structure held in a Block
          	_Block_byref_release(object);
          	break;
        case BLOCK_FIELD_IS_BLOCK:
        	  _Block_release(object);
      	    break;
        case BLOCK_FIELD_IS_OBJECT:
    	      _Block_release_object(object);
  	        break;
        case BLOCK_BYREF_CALLER | BLOCK_FIELD_IS_OBJECT:
        case BLOCK_BYREF_CALLER | BLOCK_FIELD_IS_BLOCK:
        case BLOCK_BYREF_CALLER | BLOCK_FIELD_IS_OBJECT | BLOCK_FIELD_IS_WEAK:
        case BLOCK_BYREF_CALLER | BLOCK_FIELD_IS_BLOCK  | BLOCK_FIELD_IS_WEAK:
	          break;
        default:
            break;
    }
}
```



#### 相关文章

-----

- https://www.jianshu.com/p/d96d27819679