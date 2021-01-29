## GCD vs NSOperation

--------------------

 

#### `NSOperation`相对于`GCD`的优势

-------------

- `Operation`支持`KVO`,可以监听`Operation`的状态
- `Operation`可以取消操作。而`GCD`不可以。需要注意`iOS 8`之后，`GCD`添加了一个取消任务的函数，但是使用有限制
- `Operation`可以实现依赖，而且可以实现跨队列的依赖。而`GCD`不支持，但是`GCD`可以通过其他方式去实现。`GCD`实现依赖有两种方式：`dispatch_group`和`dispatch_semaphore`
- `GCD`只支持`FIFO`的队列，而`NSOperation`可以调整队列的执行顺序



#### `GCD`相对于`NSOperation`的优势

--------









#### 什么时候使用`GCD`，什么时候使用`NSOperation`

-------------

###### 使用`NSOperation`的情况

- 各个操作之间有依赖关系
- 对操作有需求，比如需要暂停、取消
- 需要并发管理



###### 使用`GCD`的情况

- 单例
- 简单的多线程操作，例如回调主线程