## 关于NSOperation级别的操作依赖和NSOperationQueue的栅栏函数

------------

讨论一个问题，应该先从场景出发，一定是场景出现了不同，导致了不同的解决方案。

###### NSOperation的操作依赖

```markdown
这个依赖指的是`操作`级别的，和`NSOperationQueue`没有关系
也就是说，`QueueA`中的某个`Operation`可以是依赖`QueueA`当中的某个`Operation`，也可以是依赖`QueueB`当中的某个`Operation`
```



###### NSOperationQueue中的栅栏任务

```markdown
`- (void)addBarrierBlock:(void (^)(void))barrier;`
这个方法和`dispatch_barrier_async`一样的效果，作用于当前队列。
在`addBarrierBlock`之前的任务执行完，再去执行`addBarrierBlock`中的任务。`addBarrierBlock`执行完之后，再去执行`addBarrierBlock`之后添加的`Operation`

```



