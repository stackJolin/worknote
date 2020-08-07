## objc_object、objc_class、Class



#### objc_object

-----

源码：

```c++
struct objc_object {
private:
    isa_t isa;

public:

    // ISA() assumes this is NOT a tagged pointer object
    Class ISA();

    // rawISA() assumes this is NOT a tagged pointer object or a non pointer ISA
    Class rawISA();

    // getIsa() allows this to be a tagged pointer object
    Class getIsa();
    
    uintptr_t isaBits() const;

    // initIsa() should be used to init the isa of new objects only.
    // If this object already has an isa, use changeIsa() for correctness.
    // initInstanceIsa(): objects with no custom RR/AWZ
    // initClassIsa(): class objects
    // initProtocolIsa(): protocol objects
    // initIsa(): other objects
    void initIsa(Class cls /*nonpointer=false*/);
    void initClassIsa(Class cls /*nonpointer=maybe*/);
    void initProtocolIsa(Class cls /*nonpointer=maybe*/);
    void initInstanceIsa(Class cls, bool hasCxxDtor);

    // changeIsa() should be used to change the isa of existing objects.
    // If this is a new object, use initIsa() for performance.
    Class changeIsa(Class newCls);

    bool hasNonpointerIsa();
    bool isTaggedPointer();
    bool isBasicTaggedPointer();
    bool isExtTaggedPointer();
    bool isClass();

    // object may have associated objects?
    bool hasAssociatedObjects();
    void setHasAssociatedObjects();

    // object may be weakly referenced?
    bool isWeaklyReferenced();
    void setWeaklyReferenced_nolock();

    // object may have -.cxx_destruct implementation?
    bool hasCxxDtor();

    // Optimized calls to retain/release methods
    id retain();
    void release();
    id autorelease();

    // Implementations of retain/release methods
    id rootRetain();
    bool rootRelease();
    id rootAutorelease();
    bool rootTryRetain();
    bool rootReleaseShouldDealloc();
    uintptr_t rootRetainCount();

    // Implementation of dealloc methods
    bool rootIsDeallocating();
    void clearDeallocating();
    void rootDealloc();

private:
    void initIsa(Class newCls, bool nonpointer, bool hasCxxDtor);

    // Slow paths for inline control
    id rootAutorelease2();
    uintptr_t overrelease_error();

#if SUPPORT_NONPOINTER_ISA
    // Unified retain count manipulation for nonpointer isa
    id rootRetain(bool tryRetain, bool handleOverflow);
    bool rootRelease(bool performDealloc, bool handleUnderflow);
    id rootRetain_overflow(bool tryRetain);
    uintptr_t rootRelease_underflow(bool performDealloc);

    void clearDeallocating_slow();

    // Side table retain count overflow for nonpointer isa
    void sidetable_lock();
    void sidetable_unlock();

    void sidetable_moveExtraRC_nolock(size_t extra_rc, bool isDeallocating, bool weaklyReferenced);
    bool sidetable_addExtraRC_nolock(size_t delta_rc);
    size_t sidetable_subExtraRC_nolock(size_t delta_rc);
    size_t sidetable_getExtraRC_nolock();
#endif

    // Side-table-only retain count
    bool sidetable_isDeallocating();
    void sidetable_clearDeallocating();

    bool sidetable_isWeaklyReferenced();
    void sidetable_setWeaklyReferenced_nolock();

    id sidetable_retain();
    id sidetable_retain_slow(SideTable& table);

    uintptr_t sidetable_release(bool performDealloc = true);
    uintptr_t sidetable_release_slow(SideTable& table, bool performDealloc = true);

    bool sidetable_tryRetain();

    uintptr_t sidetable_retainCount();
#if DEBUG
    bool sidetable_present();
#endif
};
```



#### objc_class

----

源码：

```c++
struct objc_class : objc_object {
    Class superclass;
    const char *name;
    uint32_t version;
    uint32_t info;
    uint32_t instance_size;
    struct old_ivar_list *ivars;
    struct old_method_list **methodLists;
    Cache cache;
    struct old_protocol_list *protocols;
    // CLS_EXT only
    const uint8_t *ivar_layout;
    struct old_class_ext *ext;

    void setInfo(uint32_t set) {
        OSAtomicOr32Barrier(set, (volatile uint32_t *)&info);
    }

    void clearInfo(uint32_t clear) {
        OSAtomicXor32Barrier(clear, (volatile uint32_t *)&info);
    }


    // set and clear must not overlap
    void changeInfo(uint32_t set, uint32_t clear) {
        assert((set & clear) == 0);

        uint32_t oldf, newf;
        do {
            oldf = this->info;
            newf = (oldf | set) & ~clear;
        } while (!OSAtomicCompareAndSwap32Barrier(oldf, newf, (volatile int32_t *)&info));
    }

    bool hasCxxCtor() {
        // set_superclass propagates the flag from the superclass.
        return info & CLS_HAS_CXX_STRUCTORS;
    }

    bool hasCxxDtor() {
        return hasCxxCtor();  // one bit for both ctor and dtor
    }

    // Return YES if the class's ivars are managed by ARC, 
    // or the class is MRC but has ARC-style weak ivars.
    bool hasAutomaticIvars() {
        return info & (CLS_IS_ARC | CLS_HAS_WEAK_WITHOUT_ARC);
    }

    // Return YES if the class's ivars are managed by ARC.
    bool isARC() {
        return info & CLS_IS_ARC;
    }

    bool hasCustomRR() { 
        return true;
    }
    void setHasCustomRR(bool = false) { }
    void setHasDefaultRR() { }
    void printCustomRR(bool) { }

    bool hasCustomAWZ() { 
        return true;
    }
    void setHasCustomAWZ(bool = false) { }
    void setHasDefaultAWZ() { }
    void printCustomAWZ(bool) { }

    bool instancesHaveAssociatedObjects() {
        return info & CLS_INSTANCES_HAVE_ASSOCIATED_OBJECTS;
    }

    void setInstancesHaveAssociatedObjects() {
        setInfo(CLS_INSTANCES_HAVE_ASSOCIATED_OBJECTS);
    }

    bool shouldGrowCache() {
        return info & CLS_GROW_CACHE;
    }

    void setShouldGrowCache(bool grow) {
        if (grow) setInfo(CLS_GROW_CACHE);
        else clearInfo(CLS_GROW_CACHE);
    }

    // +initialize bits are stored on the metaclass only
    bool isInitializing() {
        return getMeta()->info & CLS_INITIALIZING;
    }

    // +initialize bits are stored on the metaclass only
    void setInitializing() {
        getMeta()->setInfo(CLS_INITIALIZING);
    }

    // +initialize bits are stored on the metaclass only
    bool isInitialized() {
        return getMeta()->info & CLS_INITIALIZED;
    }

    // +initialize bits are stored on the metaclass only
    void setInitialized() {
        getMeta()->changeInfo(CLS_INITIALIZED, CLS_INITIALIZING);
    }

    bool isLoadable() {
        // A class registered for +load is ready for +load to be called
        // if it is connected.
        return isConnected();
    }

    IMP getLoadMethod();

    bool isFuture();

    bool isConnected();

    const char *mangledName() { return name; }
    const char *demangledName() { return name; }
    const char *nameForLogging() { return name; }
    
    bool isRootClass() {
        return superclass == nil;
    }

    bool isRootMetaclass() {
        return ISA() == (Class)this;
    }

    bool isMetaClass() {
        return info & CLS_META;
    }

    // NOT identical to this->ISA() when this is a metaclass
    Class getMeta() {
        if (isMetaClass()) return (Class)this;
        else return this->ISA();
    }

    // May be unaligned depending on class's ivars.
    uint32_t unalignedInstanceStart() {
        // This is not simply superclass->instance_size.
        // superclass->instance_size is padded to its sizeof() boundary, 
        // which may envelop one of this class's ivars. 
        // That in turn would break ARC-style ivar layouts.
        // Instead, we use the address of this class's first ivar when possible.
        if (!superclass) return 0;
        if (!ivars || ivars->ivar_count == 0) return superclass->instance_size;
        return ivars->ivar_list[0].ivar_offset;
    }

    // Class's instance start rounded up to a pointer-size boundary.
    // This is used for ARC layout bitmaps.
    uint32_t alignedInstanceStart() {
        return word_align(unalignedInstanceStart());
    }


    // May be unaligned depending on class's ivars.
    uint32_t unalignedInstanceSize() {
        return instance_size;
    }

    // Class's ivar size rounded up to a pointer-size boundary.
    uint32_t alignedInstanceSize() {
        return word_align(unalignedInstanceSize());
    }

    size_t instanceSize(size_t extraBytes) {
        size_t size = alignedInstanceSize() + extraBytes;
        // CF requires all objects be at least 16 bytes.
        if (size < 16) size = 16;
        return size;
    }

};

```



#### Class

```c++
// Class 是只下过C的结构体objc_class的指针
typedef struct objc_class *Class;
```

