## 自定义NSOperation

--------

先了解一下为什么要自定义`NSOperation`：

```markdown
https://www.jianshu.com/p/a87c2d520a72
http://zxfcumtcs.github.io/2016/05/17/NSOperation/
```



#### 关于NSOperation的状态

-------

一共有四种状态：

- isReady

  ```markdown
  表示`operation`是否准备就绪，可以执行
  默认`isReady`的值取决于是否有依赖的`operations`没有执行完成
  如果`operations`变成了空，那么`isReady`会被置为`true`
  ```

- isExcuting

  ```markdown
  表示`operation`是否正在执行
  如果需要实现自定义的异步的`operation`的时候，需要重新`start`方法，在重写`start`方法的时候，需要正确的设置`isExcuting`值，并通过`KVO`机制抛出
  ```

  

- isFinished

  ```markdown
  表示`operation`是否已经完成。这里需要注意的是，`isCancelled`也是被认为是`完成`。
  如果重写了`start`方法，需要在`operation`完成时，将`isFinished`置为`true`，并且通过`KVO`机制，抛出`KVO`通知
  将`isFinished`设置为`true`之后，有下面三个作用：
  -	清除其他`operation`对该`operation`的依赖
  -	`operationQueue`将该`operation`出队
  -	如果会完成回调，那么执行完成回调
  ```

- isCancelled

  ```markdown
  表示`operaion`是否被取消，不需要手动操作该属性
  ```

  



#### `NSInvocationOperation`和`NSBlockOperation`对于`NSOperation`的实现

----------

首先`NSInvocationOperation`和`NSBlockOperation`都是`asynchronous`为`NO`的实现。也就是说，这两个任务，添加到队列，之后，只要调用了`start`方法，这个任务就算是完成了，`operation`的状态也就变成了`isFinished`。

而对于子类实现`asynchronous`返回为`YES`的子类而言，`start`方法执行完之后，`Operation`的状态并不是`isFinished`，具体什么时候变成`isFinished`，要看`start`方法中，开辟的子线程的任务完成后，`程序员`什么时候将状态改变为`isFinished`，只有`isFinished`状态的`operation`才能算是正常的结束，也就会被销毁，否则，这个`operation`会一直挂在那