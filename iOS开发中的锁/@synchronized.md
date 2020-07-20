## @syncronized源码分析



#### 使用

------





#### 源码解析

----

```swift
// Allocates recursive mutex associated with 'obj' if needed.
int objc_sync_enter(id obj)
{
    int result = OBJC_SYNC_SUCCESS;

    if (obj) {
        SyncData* data = id2data(obj, ACQUIRE);
        assert(data);
        data->mutex.lock();
    } else {
        // @synchronized(nil) does nothing
        if (DebugNilSync) {
            _objc_inform("NIL SYNC DEBUG: @synchronized(nil); set a breakpoint on objc_sync_nil to debug");
        }
        objc_sync_nil();
    }

    return result;
}

BREAKPOINT_FUNCTION(
    void objc_sync_nil(void)
);
```



#### 相关文章

-----

- https://juejin.im/post/5e66eea4f265da57375c5181