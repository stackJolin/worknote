## NSOperation

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

###### 作用

```
NSOperation可以调用start方法来执行任务，但默认是同步执行的
如果将NSOperation添加到NSOperationQueue（操作队列）中，系统会自动异步执行NSOperation中的操作
```



###### 最大并发数

```
同时执行的任务数
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

