## Objective-C，runtime消息发送、动态方法解析、消息转发

-------------

在 Objective-C 语言中，对象/类(其实类也是一个对象) 执行方法最后会转化成给对象发送消息:

`objc_msgSend(receiver, @selector(message))`

如果 `reveiver` 中没有找到对应方法 `message`, 则会开始消息转发的过程，整体来说，大概有四个流程

- 消息发送，objc-msgSend
- 方法查找，如果找的到执行，找不到执行下一步
- 动态方法解析，Method Resolution
- 消息转发，消息转发又分为两个过程
  - 快速转发
  - 方法签名转发
- 如果上面的流程都走完了，仍然没有办法转发此消息，那么会调用doesNotRecognizeSelector，抛出异常

接下来，我们详细看一下每个过程中所作的工作



#### 消息发送，objc_msgSend

----

当我们调用`[object method]`的时候，实际上执行的是`objc_msgSend(object, @selector(method))`方法，因为objective-c不像C++那样能进行函数重载，所以`objective-c`一个方法名只能对应一个函数，不能实现同一个方法名，而参数类型不同的情况

```markdown
[object method]会被翻译成下面函数中的一个：
1.objc_msgSend：函数实现在当前类，并且函数没有返回值
2.objc_msgSendSuper：函数的实现在父类，并且函数没有返回值
3.objc_msgSend_stret：函数的实现在当前类，并且返回值是数据结构
4.objc_msgSendSuper_stret：函数的实现在父类，并且返回值是数据结构
5.objc_msgSend_fpret：函数实现在当前类，并且返回值是浮点数
```



#### 方法查找，如果找的到执行，找不到执行下一步

------

`objc_msgSend`的四个函数在执行的过程中，会查找方法的IMP指针，具体分为以下两步：

- 查找object的method_cache里面有没有对应方法的IMP缓存，如果有，直接调用，没有执行下一步

- 通过C语言函数，查找方法IMP

  ```c++
  IMP _class_lookupMethodAndLoadCache3(id obj, SEL sel, Class cls)
  {
      return lookUpImpOrForward(cls, sel, obj, 
                                YES/*initialize*/, NO/*cache*/, YES/*resolver*/);
  }
  ```

  这个方法只允许被 `_objc_msgSend` 内部调度，其他方式应该使用 `lookUpImp` 此函数将忽略缓存查询，因为执行此函数之前能确保已经查询过对应的内存

  ```c++
  IMP lookUpImpOrForward(Class cls, SEL sel, id inst, 
                         bool initialize, bool cache, bool resolver)
  {
      Class curClass;
      IMP imp = nil;
      Method meth;
      bool triedResolver = NO;
  
      runtimeLock.assertUnlocked();
  
      // 这里传入cache==false，因为objc_msgSend汇编阶段已经查找过缓存，故直接跳过
      if (cache) { // 如果有缓存的话，从缓存里查找
          imp = cache_getImp(cls, sel);
          if (imp) return imp;
      }
  
      // 实现对应的类，设置父类、元类等等相关信息，分配可读写结构体 class_rw_t 的空间
      if (!cls->isRealized()) {
          rwlock_writer_t lock(runtimeLock);
          realizeClass(cls);
      }
  
       // 判断类别是否已经初始化过，初始化过程会触发+initialize
      if (initialize  &&  !cls->isInitialized()) {
          _class_initialize (_class_getNonMetaClass(cls, inst));
      }
  
      // 这里加锁是因为OC在运行时能动态添加方法，
    	// 比方说分类 category 添加方法是在运行时期添加
    	// 如果此时不添加锁进行原子读操作，很可能因为新方法添加导致缓存被冲洗（flush）
   retry:
      runtimeLock.read();//加读锁
  
      // 支持GC的环境需要对一些方法进行忽略，比如retain、release...等等
      if (ignoreSelector(sel)) {
          imp = _objc_ignored_method;
          cache_fill(cls, sel, imp, inst);
          goto done;
      }
  
      // Try this class's cache.
      // 再次查询缓存
      // TODO: 这里为什么会再次查询缓存列表？一开始cache==NO直接忽略了缓存查询，为什么加锁之后却要重新从缓存查询
      // 结合加锁的逻辑，是否因为调度的时候是并列的，但是读的时候是原子，很可能加锁之后因为上一次查找过程中重新更新了方法列表缓存？
      imp = cache_getImp(cls, sel);
      if (imp) goto done;
  
      // Try this class's method lists.
      // 缓存没有查到，到方法列表中查询
      meth = getMethodNoSuper_nolock(cls, sel);
      if (meth) {
          // 查到就更新缓存列表
          log_and_fill_cache(cls, meth->imp, sel, inst, cls);
          imp = meth->imp;
          goto done;
      }
  
  		// 开始在父类中进行查找
      curClass = cls;
      while ((curClass = curClass->superclass)) {
          // 从父类缓存中查询
          imp = cache_getImp(curClass, sel);
          if (imp) {
              // 如果是 _objc_msgForward_impcache 则不进行缓存
              if (imp != (IMP)_objc_msgForward_impcache) {
                  // 在父类查询到也存在本类的缓存中
                  log_and_fill_cache(cls, imp, sel, inst, curClass);
                  goto done;
              }
              else {
                	// 如果查找到的 IMP 为 _objc_msgForward_impcache 直接结束查找
                	// 并执行 -resolveInstanceMethod: / +resolveClassMethod:
                  break;
              }
          }
  
          // 从父类方法列表中查
          meth = getMethodNoSuper_nolock(curClass, sel);
          if (meth) {
              log_and_fill_cache(cls, meth->imp, sel, inst, curClass);
              imp = meth->imp;
              goto done;
          }
      }
  
    	// 父类中也没有找到方法
      if (resolver  &&  !triedResolver) {
          runtimeLock.unlockRead();
          // 进行 -resolveInstanceMethod: / +resolveClassMethod: 动态添加方法 
          _class_resolveMethod(cls, sel, inst);
          // 动态实现完了之后，因为之前锁已经解锁，方法列表可能已经更新了，所以会从新进行一轮方法查找
          triedResolver = YES;
          goto retry;
      }
  
      // No implementation found, and method resolver didn't help. 
      // Use forwarding.
  
      // 进行方法转发并对其结果进行缓存
      imp = (IMP)_objc_msgForward_impcache;
      cache_fill(cls, sel, imp, inst);
  
   done:
      runtimeLock.unlockRead();
  
      assert(!(ignoreSelector(sel)  &&  imp != (IMP)&_objc_ignored_method));
  
      assert(imp != _objc_msgSend_uncached_impcache);
  
      return imp;
  }
  ```
  
`lookUpImp`函数的整个查找过程分为以下几步：
  
- 实现、初始化对应的类
  - 根据是否支持垃圾回收机制(GC)判断是否忽略当前的方法调用
  - 从cache中查找方法
  - cache中没有找到对应的方法，则到方法列表中查，查到则缓存
  - 如果本类中查询到没有结果，则遍历所有父类重复上面的查找过程
  - 最后都没有找到的方法的话，则执行 `_class_resolveMethod` 让调用者动态添加方法，并重复一轮查询方法的过程
  - 若第六步没有完成动态添加方法，则把 _objc_msgForward_impcache 作为对应 SEL 的方法进行缓存，然后调用 _objc_msgForward_impcache 方法



#### 动态方法解析，Method Resolution

-------

消息发送的过程中，如果没有找到方法，先进行 `_class_resolveMethod` 允许开发者动态的根据 SEL 实现对应的 IMP，实现前先执行 `runtimeLock.unlockRead()` 打开了读锁，所以开发者在此动态实现的过程添加了方法实现，故不需要缓存方法；

`_class_resolveMethod` 调用过程又是非原子性的，执行完的时候方法列表可能已经更新了，所以执行完了之后需要重复一轮查询方法的过程

```objective-c
void _class_resolveMethod(Class cls, SEL sel, id inst)
{
    if (! cls->isMetaClass()) {
        // try [cls resolveInstanceMethod:sel]
        _class_resolveInstanceMethod(cls, sel, inst);
    } 
    else {
        // try [nonMetaClass resolveClassMethod:sel]
        // and [cls resolveInstanceMethod:sel]
        _class_resolveClassMethod(cls, sel, inst);
        if (!lookUpImpOrNil(cls, sel, inst, 
                            NO/*initialize*/, YES/*cache*/, NO/*resolver*/)) 
        {
            _class_resolveInstanceMethod(cls, sel, inst);
        }
    }
}
```

如果 cls 不是元类，则执行 `_class_resolveInstanceMethod` 函数；否则 cls 属于元类则会调用 `_class_resolveClassMethod` ，然后执行 `lookUpImpOrNil`

```objective-c
IMP lookUpImpOrNil(Class cls, SEL sel, id inst, 
                   bool initialize, bool cache, bool resolver)
{
    IMP imp = lookUpImpOrForward(cls, sel, inst, initialize, cache, resolver);
    if (imp == _objc_msgForward_impcache) return nil;
    else return imp;
}
```

`lookUpImpOrNil` 和 `lookUpImpOrForward` 类似，前者内部是先调用后者函数，判断返回 imp 结果是否和 _objc_msgForward_impcache 相同，如果相同返回 nil，反之返回 imp。

需要注意的是在 `lookUpImpOrNil` 中并不会对 cls 进行初始化(initialize)或者是方法动态实现过程(resolver)，若 `lookUpImpOrNil` 返回了nil，则会调用 `_class_resolveInstanceMethod`

这里以非元类来分析

```objective-c
static void _class_resolveInstanceMethod(Class cls, SEL sel, id inst)
{
    // 如果类没有实现 `+resolveInstanceMethod`  方法则返回nil
    if (! lookUpImpOrNil(cls->ISA(), SEL_resolveInstanceMethod, cls, 
                         NO/*initialize*/, YES/*cache*/, NO/*resolver*/)) 
    {
        return;
    }

    // 通过 objc_msgSend 来执行  resolveInstanceMethod 方法
    BOOL (*msg)(Class, SEL, SEL) = (typeof(msg))objc_msgSend;
    bool resolved = msg(cls, SEL_resolveInstanceMethod, sel);

    // resolveInstanceMethod 执行过程中肯能会动态添加方法， lookUpImpOrNil 会缓存最新的imp(不管是否是开发者动态实现)，
  	// 这样做可以下次方法调用的时候，不会再次执行动态方法解析的过程
    IMP imp = lookUpImpOrNil(cls, sel, inst, 
                             NO/*initialize*/, YES/*cache*/, NO/*resolver*/);

    // ......忽略相关日志代码
}
```

到此，消息转发前的逻辑已经全部走完，简单总结一下各个函数调用的顺序作用：

- 汇编入口 `_objc_msgSend` 为消息发送的入口
- 找不到方法则跳转到 `__objc_msgSend_uncached_impcache` ，对栈进行相关操作
- 跳转 `_class_lookupMethodAndLoadCache3` (objc-runtime-new.mm)
- 第一次执行 `lookUpImpOrForward`, 对相关类进行 initialize 相关操作，忽略缓存列表去查找方法，如果找不到会进行 reslover 动态方法解析
- 步骤4会一直从本类到父类进行重复查找，如果都没有找到方法则调用 `_class_resolveMethod` 进行方法动态解析
- 如果是非元类，则直接跳转到 `_class_resolveInstanceMethod` ，函数内部会先调用 `lookUpImpOrNil` 来判断类有没有实现 `+resolveInstanceMethod` 方法，这里的查找结果也会缓存到 cache 中，内部查找也是通过 `lookUpImpOrForward` 来实现，根据返回的imp是否为 `_objc_msgForward_impcache` ，若是则返回 nil，然后 `_class_resolveClassMethod` 会直接return，结束动态解析过程
- 若 `+resolveClassMethod` 被实现，则同过 `objc_msgSend` 来执行 `+resolveClassMethod` 方法；缓存结果，减少 `_class_resolveClassMethod` 过程调用



#### 消息转发

--------

消息转发分为两个流程，顺序是：

- Fast Forwarding：快速转发
- Normal Forwarding：正常转发

###### Fast Forwarding:

```objective-c
- (id)forwardingTargetForSelector:(SEL)aSelector {
    if(aSelector == @selector(foo:)){
        return [[BackupClass alloc] init];
    }
    return [super forwardingTargetForSelector:aSelector];
}
// Fast Rorwarding这是一种快速消息转发：只需要在指定API方法里面返回一个新对象即可
// 消息转发机制执行前，runtime系统允许我们替换消息的接收者为其他对象。通过`- (id)forwardingTargetForSelector:(SEL)aSelector`方法。如果此方法返回的是nil 或者self,则会进入消息转发机制`（- (void)forwardInvocation:(NSInvocation *)invocation`），否则将会向返回的对象重新发送消息。

```



###### Normal Forwarding:

```objective-c
- (void)forwardInvocation:(NSInvocation *)invocation {
    SEL sel = invocation.selector;
    if([alternateObject respondsToSelector:sel]) {
        [invocation invokeWithTarget:alternateObject];
    } else {
        [self doesNotRecognizeSelector:sel];
    }
}

- (NSMethodSignature *)methodSignatureForSelector:(SEL)aSelector {
    NSMethodSignature *methodSignature = [super methodSignatureForSelector:aSelector];
    if (!methodSignature) {
        methodSignature = [NSMethodSignature signatureWithObjCTypes:"v@:*"];
    }
    return methodSignature;
}

//forwardInvocation: 方法就是一个不能识别消息的分发中心，将这些不能识别的消息转发给不同的消息对象，或者转发给同一个对象，再或者将消息翻译成另外的消息，亦或者简单的“吃掉”某些消息，因此没有响应也不会报错。例如：我们可以为了避免直接闪退，可以当消息没法处理时在这个方法中给用户一个提示，也不失为一种友好的用户体验。
// 其中，参数invocation是从哪来的？在forwardInvocation:消息发送前，runtime系统会向对象发送methodSignatureForSelector:消息，并取到返回的方法签名用于生成NSInvocation对象。所以重写forwardInvocation:的同时也要重写methodSignatureForSelector:方法，否则会抛出异常。当一个对象由于没有相应的方法实现而无法响应某个消息时，运行时系统将通过forwardInvocation:消息通知该对象。每个对象都继承了forwardInvocation:方法，我们可以将消息转发给其它的对象。
```



###### Fast Forwarding 和 Normal Forwarding对比

```markdown
需要重载的API方法的用法不同:
-	前者只需要重载一个API即可，后者需要重载两个API。
-	前者只需在API方法里面返回一个新对象即可，后者需要对被转发的消息进行重签并手动转发给新对象（利用 invokeWithTarget:）。

转发给新对象的个数不同:
-	前者只能转发一个对象，后者可以连续转发给多个对象。例如下面是完整转发：
```







#### 相关文章

----

https://chipengliu.github.io/2019/06/02/objc-msgSend-forward/

https://juejin.im/post/5ae96e8c6fb9a07ac85a3860

Objc-msg-arm64源码深入分析：https://chipengliu.github.io/2019/04/07/objc-msg-armd64/