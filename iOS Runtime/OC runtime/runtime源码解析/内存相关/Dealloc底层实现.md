## Dealloc低层实现

------

`NSObject`调用`dealloc`后，产生的函数调用链如下：

- dealloc
- objc_rootDealloc
- objc_dispose
- objc_destructInstance

`objc_descructInstance`的源码如下：

```c++
void _objc_rootDealloc(id obj)
{
    ASSERT(obj);

    obj->rootDealloc();
}

inline void objc_object::rootDealloc()
{
    if (isTaggedPointer()) return;  // fixme necessary?

    if (fastpath(isa.nonpointer  &&  
                 !isa.weakly_referenced  &&  
                 !isa.has_assoc  &&  
                 !isa.has_cxx_dtor  &&  
                 !isa.has_sidetable_rc))
    {
        assert(!sidetable_present());
        free(this);
    } 
    else {
        object_dispose((id)this);
    }
}

id object_dispose(id obj)
{
    if (!obj) return nil;

    objc_destructInstance(obj);
    // 做完各种析构后，释放obj的内存
    free(obj);

    return nil;
}

void *objc_destructInstance(id obj) 
{
    if (obj) {
        // Read all of the flags at once for performance.
        bool cxx = obj->hasCxxDtor();
        bool assoc = obj->hasAssociatedObjects();

        // 下面这三个的执行顺序非常重要
        // 1.释放实例变量等
        if (cxx) object_cxxDestruct(obj);
        // 2.移除关联对象
        if (assoc) _object_remove_assocations(obj);
        // 3.清空引用计数表 + 清空弱引用表
        obj->clearDeallocating();
    }

    return obj;
}
```

基本上一个对象的释放有以下几个步骤：

- 释放自身的成员变量
- 移除自身的关联对象
- 清空`refCountMap`和`weak_table_t`
- 释放自己



#### dealloc中的常见错误

--------







#### dealloc在哪个线程被调用

---------

`dealloc`并不总是在主线程中被调用，从上面的`sidetable_release`方法来看，`dealloc`的调用线程是最后一个调用`release`方法的线程。当需要释放对象时，向对象实例发送`dealloc`消息。所以尽量避免，在`dealloc`中执行UI相关的操作













