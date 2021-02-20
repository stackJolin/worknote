## Block copy详解

-------

`__NSConcreteStaticBlock`执行一次`copy`操作，就会将block复制一份到堆区，变成了`_NSConcreteMallocBlock`。而这里的`copy`其实是指的runtime中的`_Block_copy`方法，具体如下

```c++
void *_Block_copy(const void *arg) {
    //1、声明一个Block_layout变量，如果传递进来的参数为nil，直接返回
    struct Block_layout *aBlock;
    if (!arg) return NULL;
    
    //2、将传递进来的block，强制转换为`Block_layout`类型
    aBlock = (struct Block_layout *)arg;
  
    //3、如果是堆区Block，增加引用计数后，然后返回
    if (aBlock->flags & BLOCK_NEEDS_FREE) {
        // latches on high
        latching_incr_int(&aBlock->flags);
        return aBlock;
    }
    //4、如果是全局block，直接返回
    else if (aBlock->flags & BLOCK_IS_GLOBAL) {
        return aBlock;
    }
    else {
        //5、如果是栈区Block
        // 堆区分配一块和该block大小一致的控件
        struct Block_layout *result = malloc(aBlock->descriptor->size);
        // 如果分配失败，直接返回
        if (!result) return NULL;
        //6、将栈区的block copy到堆区
        memmove(result, aBlock, aBlock->descriptor->size); // bitcopy first
        // 7、将新的block的引用计数设置为0
        result->flags &= ~(BLOCK_REFCOUNT_MASK|BLOCK_DEALLOCATING);    // XXX not needed
        result->flags |= BLOCK_NEEDS_FREE | 2;  // logical refcount 1
        //8、复制copy和dispose助手，如果有的话
        _Block_call_copy_helper(result, aBlock);
        //9、修改ISA指针类型
        result->isa = _NSConcreteMallocBlock;
        return result;
    }
}

static int32_t latching_incr_int(volatile int32_t *where) {
    while (1) {
        int32_t old_value = *where;
        if ((old_value & BLOCK_REFCOUNT_MASK) == BLOCK_REFCOUNT_MASK) {
            return BLOCK_REFCOUNT_MASK;
        }
        if (OSAtomicCompareAndSwapInt(old_value, old_value+2, where)) {
            return old_value+2;
        }
    }
}

static void _Block_call_copy_helper(void *result, struct Block_layout *aBlock)
{
    struct Block_descriptor_2 *desc = _Block_descriptor_2(aBlock);
    if (!desc) return;

    (*desc->copy)(result, aBlock); // do fixup
}

static struct Block_descriptor_2 * _Block_descriptor_2(struct Block_layout *aBlock)
{
    if (! (aBlock->flags & BLOCK_HAS_COPY_DISPOSE)) return NULL;
    uint8_t *desc = (uint8_t *)aBlock->descriptor;
    desc += sizeof(struct Block_descriptor_1);
    return (struct Block_descriptor_2 *)desc;
}
```

上面的就是`_NSConcreteStaticBlock`转换为`_NSConcreteMallocBlock`的流程，具体如下：

- 1.声明一个`Block_layout`，如果被copy的block是Null，直接返回
- 2.将传进来的block，强制转换成`Block_layout`类型
- 3.如果传递进来的Block是一个堆block，就增加引用计数，然后返回源block
- 4.如果传递进来的Block是一个全局block，直接返回，不做任何处理
- 5.如果传递进来的Block是一个栈Block，按照栈Block大小，在堆区分配一块内存。如果分配失败，直接返回
- 6.利用`memmove`函数，将栈区block复制到堆区
- 7.将新的block的引用计数设置为2，并将Block标记为堆区
- 8.复制copy和dispose助手
- 9.修改isa指针