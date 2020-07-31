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
void *_Block_copy(const void *arg) {
    struct Block_layout *aBlock;

    if (!arg) return NULL;
    
    // The following would be better done as a switch statement
    aBlock = (struct Block_layout *)arg;
    if (aBlock->flags & BLOCK_NEEDS_FREE) {
        // latches on high
        latching_incr_int(&aBlock->flags);
        return aBlock;
    }
    else if (aBlock->flags & BLOCK_IS_GLOBAL) {
        return aBlock;
    }
    else {
        // Its a stack block.  Make a copy.
        struct Block_layout *result = malloc(aBlock->descriptor->size);
        if (!result) return NULL;
        memmove(result, aBlock, aBlock->descriptor->size); // bitcopy first
        // reset refcount
        result->flags &= ~(BLOCK_REFCOUNT_MASK|BLOCK_DEALLOCATING);    // XXX not needed
        result->flags |= BLOCK_NEEDS_FREE | 2;  // logical refcount 1
        _Block_call_copy_helper(result, aBlock);
        // Set isa last so memory analysis tools see a fully-initialized object.
        result->isa = _NSConcreteMallocBlock;
        return result;
    }
}

void _Block_release(const void *arg) {
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

```c++
void _Block_object_assign(void *destArg, const void *object, const int flags) {
    const void **dest = (const void **)destArg;
    switch (os_assumes(flags & BLOCK_ALL_COPY_DISPOSE_FLAGS)) {
        case BLOCK_FIELD_IS_OBJECT:
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

