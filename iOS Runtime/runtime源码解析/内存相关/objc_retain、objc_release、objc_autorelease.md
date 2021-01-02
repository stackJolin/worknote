## objc_retain、objc_release、objc_autorelease

-----

上面三个函数在NSObject.mm文件中，链接：https://opensource.apple.com/source/objc4/objc4-781/runtime/NSObject.mm.auto.html

源码如下：

```c++
// 表示的是objective-c 2.0版本，2.0版本是稳定版本，也就是我们现在使用的版本
#if __OBJC2__

__attribute__((aligned(16), flatten, noinline))
id 
objc_retain(id obj)
{
    if (!obj) return obj;
    if (obj->isTaggedPointer()) return obj;
    return obj->retain();
}


__attribute__((aligned(16), flatten, noinline))
void 
objc_release(id obj)
{
    if (!obj) return;
    if (obj->isTaggedPointer()) return;
    return obj->release();
}


__attribute__((aligned(16), flatten, noinline))
id
objc_autorelease(id obj)
{
    if (!obj) return obj;
    if (obj->isTaggedPointer()) return obj;
    return obj->autorelease();
}


// OBJC2
#else
// not OBJC2


id objc_retain(id obj) { return [obj retain]; }
void objc_release(id obj) { [obj release]; }
id objc_autorelease(id obj) { return [obj autorelease]; }


#endif
```



#### objc_retain

----

```c++
id 
objc_retain(id obj)
{
    if (!obj) return obj;
    if (obj->isTaggedPointer()) return obj;
    return obj->retain();
}

- (id)retain {
    return _objc_rootRetain(self);
}

NEVER_INLINE id
_objc_rootRetain(id obj)
{
    ASSERT(obj);

    return obj->rootRetain();
}
```

obj->rootRetain()的实现在objc-objct.h文件中，链接:https://opensource.apple.com/source/objc4/objc4-781/runtime/objc-object.h.auto.html

```c++
ALWAYS_INLINE id 
objc_object::rootRetain()
{
    return rootRetain(false, false);
}

ALWAYS_INLINE id 
objc_object::rootRetain(bool tryRetain, bool handleOverflow)
{
    // 如果是taggedPointer 直接返回
    if (isTaggedPointer()) return (id)this;

    bool sideTableLocked = false;
    bool transcribeToSideTable = false;

    isa_t oldisa;
    isa_t newisa;

    do {
        transcribeToSideTable = false;
        oldisa = LoadExclusive(&isa.bits);
        newisa = oldisa;
        if (slowpath(!newisa.nonpointer)) {
            ClearExclusive(&isa.bits);
            if (rawISA()->isMetaClass()) return (id)this;
            if (!tryRetain && sideTableLocked) sidetable_unlock();
            if (tryRetain) return sidetable_tryRetain() ? (id)this : nil;
            else return sidetable_retain();
        }
        // don't check newisa.fast_rr; we already called any RR overrides
        if (slowpath(tryRetain && newisa.deallocating)) {
            ClearExclusive(&isa.bits);
            if (!tryRetain && sideTableLocked) sidetable_unlock();
            return nil;
        }
        uintptr_t carry;
        newisa.bits = addc(newisa.bits, RC_ONE, 0, &carry);  // extra_rc++

        if (slowpath(carry)) {
            // newisa.extra_rc++ overflowed
            if (!handleOverflow) {
                ClearExclusive(&isa.bits);
                return rootRetain_overflow(tryRetain);
            }
            // Leave half of the retain counts inline and 
            // prepare to copy the other half to the side table.
            if (!tryRetain && !sideTableLocked) sidetable_lock();
            sideTableLocked = true;
            transcribeToSideTable = true;
            newisa.extra_rc = RC_HALF;
            newisa.has_sidetable_rc = true;
        }
    } while (slowpath(!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits)));

    if (slowpath(transcribeToSideTable)) {
        // Copy the other half of the retain counts to the side table.
        sidetable_addExtraRC_nolock(RC_HALF);
    }

    if (slowpath(!tryRetain && sideTableLocked)) sidetable_unlock();
    return (id)this;
}
```

总体分为三个流程：

- 如果是taggedPointer，也就是值存在指针内部，直接返回

- 如果是未优化的isa，使用sidetable_retain

- 如果是已经优化的isa，这里分为两种情况

  - 如果extra_rc溢出

    将isa.extra_rc中的一半值转移至sidetable中，然后将isa.has_sidetable_rc设置为true，表示使用了sidetable来计算引用计数

  - 如果extra_rc未溢出

    执行isa.extra_rc + 1





#### objc_release

------





#### objc_autorelease

-----





