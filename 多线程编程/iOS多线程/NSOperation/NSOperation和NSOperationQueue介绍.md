## NSOperation和NSOperationQueue介绍

------------

我们知道，`NSOperation`和`NSOperationQueue`是基于`GCD`的封装。`NSOperationQueue`相当于`dispatch_group`，而`NSOperation`相当于`dispatch_block_t`

需要注意的是，`NSOperation`和`NSOperationQueue`是`iOS2.0`开始就有的，而`GCD`是`iOS4.0`之后推出的，只不过，在`GCD`推出之后，`NSOperation`和`NSOperationQueue`用`GCD`重写了一遍



#### NSOperation和GCD的对比

----------

1. GCD是一套 C 语言API,执行和操作简单高效，因此NSOperation底层也通过GCD实现
2. 依赖关系，NSOperation可以设置操作之间的依赖(可以跨队列设置)，GCD无法设置依赖关系，不过可以通过同步来实现这种效果；
3. KVO(键值对观察)，NSOperation容易判断操作当前的状态(是否执行，是否取消等)，对此GCD无法通过KVO进行判断；
4. 优先级，NSOperation可以设置自身的优先级，但是优先级高的不一定先执行，GCD只能设置队列的优先级，如果要区分block任务的优先级,需要很复杂的代码才能实现；
5. 继承，NSOperation是一个抽象类.实际开发中常用的是它的两个子类:NSInvocationOperation和NSBlockOperation，同样我们可以自定义NSOperation，GCD执行任务可以自由组装，没有继承那么高的代码复用度；
6. 效率，直接使用GCD效率确实会更高效，NSOperation会多一点开销，但是通过NSOperation可以获得依赖，优先级，继承，键值对观察这些优势，相对于多的那么一点开销确实很划算，鱼和熊掌不可得兼，取舍在于开发者自己；
7. 可以随时取消准备执行的任务(已经在执行的不能取消),GCD没法停止已经加入queue 的 block(虽然也能实现,但是需要很复杂的代码)





###### 简介

NSOperation是对GCD的封装，通过NSOperation和NSOperationQueue的结合来实现多线程并发

###### 相关类

- NSOperation
- NSBlockOperation:NSOperation
- NSInvocationOperation:NSOperation
- NSOperationQueue



#### NSBlockOperation

----

```
-(void) invocationOpeation {
    NSInvocationOperation *op1 = [[NSInvocationOperation alloc]initWithTarget:self selector:@selector(operation1) object:nil];
    NSInvocationOperation *op2 = [[NSInvocationOperation alloc]initWithTarget:self selector:@selector(operation2) object:nil];
    NSInvocationOperation *op3 = [[NSInvocationOperation alloc]initWithTarget:self selector:@selector(operation3) object:nil];
    
    [op1 start];
    [op2 start];
    [op3 start];
}

-(void)operation1{
    NSLog(@"1--%@",[NSThread currentThread]);
}

-(void)operation2{
    NSLog(@"2--%@",[NSThread currentThread]);
}

-(void)operation3{
    NSLog(@"3--%@",[NSThread currentThread]);
}
```

打印：

```
RCNSOperationDemo[1368:53571] 1--<NSThread: 0x6000039cb5c0>{number = 1, name = main}
RCNSOperationDemo[1368:53571] 2--<NSThread: 0x6000039cb5c0>{number = 1, name = main}
RCNSOperationDemo[1368:53571] 3--<NSThread: 0x6000039cb5c0>{number = 1, name = main}
```

#### NSInvocationOperation

-------

```
NSBlockOperation *blockOperation = [NSBlockOperation blockOperationWithBlock:^{
    NSLog(@"created block: %@", [NSThread currentThread]);
}];

[blockOperation addExecutionBlock:^{
    NSLog(@"add block 1: %@", [NSThread currentThread]);
}];

[blockOperation addExecutionBlock:^{
    NSLog(@"add block 2: %@", [NSThread currentThread]);
}];

[blockOperation addExecutionBlock:^{
    NSLog(@"add block 3: %@", [NSThread currentThread]);
}];

[blockOperation addExecutionBlock:^{
    NSLog(@"add block 4: %@", [NSThread currentThread]);
}];

[blockOperation start];
```

打印：

```
created block: <NSThread: 0x610000066c40>{number = 1, name = main}
add block 3: <NSThread: 0x600000065800>{number = 5, name = (null)}
add block 4: <NSThread: 0x61000006ba80>{number = 6, name = (null)}
add block 2: <NSThread: 0x61800006b800>{number = 4, name = (null)}
add block 1: <NSThread: 0x608000066b00>{number = 3, name = (null)}
```

可以看得出来，创建时候配置的block是在主线程执行的，而后面通过对象方法add进去的block是并发执行的。并发的一般是在子线程完成，`但是也有可能在主线程执行`！！！

`注意`NSBlockOperation的任务一旦开始，是不能再往里面添加任务的。在上面的例子中，如果把后面的start方法写在前面的任务添加之前，则会报错

```
*** Terminating app due to uncaught exception 'NSInvalidArgumentException', reason: '*** -[NSBlockOperation addExecutionBlock:]: blocks cannot be added after the operation has started executing or finished'
```



#### NSOperationQueue

-----

###### 两种队列：

- `[NSOperationQueue  mainQueue]`：主队列
- `[NSOperaionQueue new]`：自定义队列



###### 作用

```
NSOperation可以调用start方法来执行任务，但默认是同步执行的
如果将NSOperation添加到NSOperationQueue（操作队列）中，系统会自动异步执行NSOperation中的操作
```



###### 最大并发数

```
同时执行的任务数。默认值是-1，表示不进行限制。设置为1时，队列为串行队列。大于1时，队列为并发队列
-(NSInteger)maxConcurrentOperationCount;
-(void)setMaxConcurrentOperationCount:(NSInteger)cnt;
```

###### 队列的取消、暂停、恢复

- 取消队列的所有操作

  ```
  -(void)cancelAllOperations;
  提示：也可以调用NSOperation的- (void)cancel方法取消单个操作
  ```

- 暂停和恢复 

  ```
  -(void)setSuspended:(BOOL)b; // YES代表暂停队列，NO代表恢复队列
  -(BOOL)isSuspended;
  ```

###### 操作依赖

```
NSOperation之间可以设置依赖来保证执行顺序
比如一定要让操作A执行完后，才能执行操作B，可以这么写
[operationB addDependency:operationA]; // 操作B依赖于操作A

可以在不同queue的NSOperation之间创建依赖关系
```



#### 例子

-----

###### 

```
-(void) invocationOpeation {
    NSInvocationOperation *op1 = [[NSInvocationOperation alloc]initWithTarget:self selector:@selector(operation1) object:nil];
    NSInvocationOperation *op2 = [[NSInvocationOperation alloc]initWithTarget:self selector:@selector(operation2) object:nil];
    NSInvocationOperation *op3 = [[NSInvocationOperation alloc]initWithTarget:self selector:@selector(operation3) object:nil];
    
    [op1 start];
    [op2 start];
    [op3 start];
}

-(void)operation1{
    NSLog(@"1--%@",[NSThread currentThread]);
}

-(void)operation2{
    NSLog(@"2--%@",[NSThread currentThread]);
}

-(void)operation3{
    NSLog(@"3--%@",[NSThread currentThread]);
}
```

打印：

```
RCNSOperationDemo[1368:53571] 1--<NSThread: 0x6000039cb5c0>{number = 1, name = main}
RCNSOperationDemo[1368:53571] 2--<NSThread: 0x6000039cb5c0>{number = 1, name = main}
RCNSOperationDemo[1368:53571] 3--<NSThread: 0x6000039cb5c0>{number = 1, name = main}
```



######NSBlockOperation的使用

```
NSBlockOperation *blockOperation = [NSBlockOperation blockOperationWithBlock:^{
    NSLog(@"created block: %@", [NSThread currentThread]);
}];

[blockOperation addExecutionBlock:^{
    NSLog(@"add block 1: %@", [NSThread currentThread]);
}];

[blockOperation addExecutionBlock:^{
    NSLog(@"add block 2: %@", [NSThread currentThread]);
}];

[blockOperation addExecutionBlock:^{
    NSLog(@"add block 3: %@", [NSThread currentThread]);
}];

[blockOperation addExecutionBlock:^{
    NSLog(@"add block 4: %@", [NSThread currentThread]);
}];

[blockOperation start];
```

打印：

```
created block: <NSThread: 0x610000066c40>{number = 1, name = main}
add block 3: <NSThread: 0x600000065800>{number = 5, name = (null)}
add block 4: <NSThread: 0x61000006ba80>{number = 6, name = (null)}
add block 2: <NSThread: 0x61800006b800>{number = 4, name = (null)}
add block 1: <NSThread: 0x608000066b00>{number = 3, name = (null)}
```

可以看得出来，创建时候配置的block是在主线程执行的，而后面通过对象方法add进去的block是并发执行的。并发的一般是在子线程完成，`但是也有可能在主线程执行`！！！

`注意`NSBlockOperation的任务一旦开始，是不能再往里面添加任务的。在上面的例子中，如果把后面的start方法写在前面的任务添加之前，则会报错

```
*** Terminating app due to uncaught exception 'NSInvalidArgumentException', reason: '*** -[NSBlockOperation addExecutionBlock:]: blocks cannot be added after the operation has started executing or finished'
```

