## Block release详解

------

runtime源码如下：

```c++
void _Block_release(const void *arg) {
    //1、将传入的参数强制转换为Block_layout类型。如果传入的参数不存在，直接return
    struct Block_layout *aBlock = (struct Block_layout *)arg;
    if (!aBlock) return;
  
    //2、如果是全局block，直接返回
    if (aBlock->flags & BLOCK_IS_GLOBAL) return;
  
    //3、如果不是堆区的block，直接返回(不是堆区的block，那就是栈区的block)
    if (! (aBlock->flags & BLOCK_NEEDS_FREE)) return;
    //4、判断block的引用计数，是否需要释放内存
    if (latching_decr_int_should_deallocate(&aBlock->flags)) {
        //5、调用dispose助手，清理
        _Block_call_dispose_helper(aBlock);
        //6、没有做任何操作
        _Block_destructInstance(aBlock);
        //7、释放掉block
        free(aBlock);
    }
}

static bool latching_decr_int_should_deallocate(volatile int32_t *where) {
    while (1) {
        int32_t old_value = *where;
        if ((old_value & BLOCK_REFCOUNT_MASK) == BLOCK_REFCOUNT_MASK) {
            return false; // latched high
        }
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

static void _Block_call_dispose_helper(struct Block_layout *aBlock)
{
    struct Block_descriptor_2 *desc = _Block_descriptor_2(aBlock);
    if (!desc) return;

    (*desc->dispose)(aBlock);
}

static void _Block_destructInstance_default(const void *aBlock __unused) {}
static void (*_Block_destructInstance) (const void *aBlock) = _Block_destructInstance_default;

```

具体流程如下：

- 1.强制转换参数类型，如果参数不存在直接返回
- 2.如果是全局区block，直接返回
- 3.如果是栈区block，直接返回
- 4判断block引用计数
  - 如果引用计数为0，调用dispose助手，清理block
  - 如果不为0，返回