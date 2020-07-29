## Block内存详解

https://www.jianshu.com/p/4a6dca34d980

https://github.com/nst/iOS-Runtime-Headers/tree/master/Frameworks/CoreFoundation.framework

block源码：https://opensource.apple.com/source/libclosure/libclosure-67/



#### 目录

----

- 内存类型的角度划分block类型
- block内存管理
- block防止内存引用



#### 内存类型的角度划分block类型

----

根据block在内存中的位置，系统把block分围3类：NSGlobalBlock、NSStackBlock、NSMallocBlock

- NSGlobalBlock：位于内存全局区
- NSStackBlock：位于内存栈区
- NSMallocBlock：位于内存堆区

下面的测试代码，都是没有经过赋值操作的，也就是没有 id block = block的方法执行，原因下面有讨论：Block什么情况下，会自动执行copy操作

**<font color='orange'>NSGlobalBlock:</font>**

没有引用局部变量的block，实际的类型是NSGlobalBlock

```objective-c
//类型1：没有使用任何外部变量
-(void)test
{
    void (^gBlock1)(int , int ) =^(int a, int b){
        NSLog(@"a + b = %d", a+b);
    };
    
    NSLog(@"%@", gBlock1);
    //打印结果为:
    //<__NSGlobalBlock__: 0x1025e8110>
}

//类型2：使用全局变量

//全局变量
int a = 10;

-(void)test
{
    void (^gBlock)() = ^(){
        NSLog(@"%d", a);
    };
    
    NSLog(@"%@", gBlock);
    //输出结果为：
    //<__NSGlobalBlock__: 0x103676110>
}
```

**<font color='orange'>NSStackBlock:</font>**

引用了局部变量的block叫做`NSStackBlock`， 实例如下：

```objective-c
-(void)test
{
    //局部变量
     NSArray *arr = @[@"zhangsan", @"lisi"];
    
    void (^sBlock)() = ^(){
        NSLog(@"arr = %@", arr);
    };
    
    NSLog(@"%@", sBlock);
    //输出结果为：
    //<__NSStackBlock__: 0x7fff5bbf1a58>
}
```

**<font color='orange'>NSMallocBlock:</font>**

在非ARC下，我们一般不手动创建`NSMallocBlock`，我们把从栈区复制(copy)过来的block称为堆区block。实例如下：

```objective-c
-(void)test
{
    NSArray *arr = @[@"zhangsan", @"lisi"];
    
    //栈区block
    void (^sBlock)() = ^(){
        NSLog(@"arr = %@", arr);
    };
    NSLog(@"%@", sBlock);

    //堆区block
    void (^mBlock)() = [sBlock copy];
    NSLog(@"%@", mBlock);
    
    //输出结果为：
    //<__NSStackBlock__: 0x7fff59bf9a38>
    //<__NSMallocBlock__: 0x7fc173f0dd80>
}
```

总结一下：

- 没有捕获外部变量并且没有被copy的block类型就是NSGlobalBlock
- NSBlobalBlock如果捕获了外部变量，就会变成NSStackBlock
- NSStackBlock或者NSGlobalBlock执行过copy方法后，就会变成NSMallockBlock

那么block什么情况下会执行copy操作呢？

- 显示的调用copy方法
- block作为函数的返回值
- @property声明的block使用了copy和strong
- block赋值给使用__strong修饰的变量时

#### Block的内存管理

-----

**<font color='orange'>Block中，引用外部变量后，外部变量的内存管理</font>**

```objective-c
-(void)test
{

    NSArray *arr = @[@"zhangsan", @"lisi"];
    NSLog(@"arr.retianCount = %ld", arr.retainCount);
    
    //栈区block
    void (^sBlock)() = ^(){
        NSLog(@"arr = %@", arr);
    };
    //栈区block不会对引用的变量引用计数+1
    NSLog(@"arr.retianCount = %ld", arr.retainCount);
    
    //堆区block
    void (^mBlock)() = [sBlock copy];
    //复制到堆区后，引用计数+1
    NSLog(@"arr.retianCount = %ld", arr.retainCount);
}
在block中经常会用到外部变量/对象，如果这个block是存储在堆区，或者被复制到堆区，则对象对应的实例引用+1，当block释放后block的引用-1；
```

**<font color='orange'>Block的循环引用</font>**

我们现在看一下，block什么什么情况下会被复制到堆区，因为产生循环引用一定是在堆区



#### Block什么情况下，会自动执行copy操作

ARC下，某些情况系统会对Block自动执行一次copy操作，将block从栈区转移到堆区。我们先先看一下三种block执行copy的结论：

| Block类型     | 上下文环境                  | 内存区域 | 执行Copy后的结果 |
| ------------- | --------------------------- | -------- | ---------------- |
| NSGlobalBlock | 内部没有访问auto变量        | 全局区   | 内存无变化       |
| NSStaticBlock | 内部访问了auto变量          | 栈区     | 从栈区复制到堆区 |
| NSMallocBlock | NSStaticBlock调用了copy方法 | 堆区     | 引用计数+1       |

下面的测试，只针对NSStaticBlock的copy执行测试，NSGlobalBlock和NSMallockBlock的测试，有兴趣的可以自己验证

<font color=orange>**Block作为返回值时，会执行copy操作**</font>

```objective-c
// 我们知道，block中引用外部非全局变量和生成一个NSStackBlock
- (void)test01 {
    int a = 10;

    // 打印了一个 NSStackBlock
    NSLog(@"%p",^ {
        int b = a;
    });

    __weak id obj1;
    obj1 = ^(){
        int b = a;
    };
    NSLog(@"%p", obj1);
    
    id obj2;
    obj2 = ^(){
        int b = a;
    };

    NSLog(@"%@",obj2);
}
```

打印:

```object-c
2020-07-29 20:32:52.180644+0800 RacTest_OC[79452:2964833] <__NSStackBlock__: 0x7ffee4e0d3a8>
2020-07-29 20:32:52.180724+0800 RacTest_OC[79452:2964833] (null)
2020-07-29 20:32:52.180814+0800 RacTest_OC[79452:2964833] <__NSMallocBlock__: 0x6000023535a0>
```

结果：

- 单纯的声明一个引用非全局变量的block1，这个block1是在栈区
- 当把block1赋值给__weak修饰的弱引用对象的时候，发现为null。也就是weak修饰的变量，在赋值block的时候，并不会执行copy，并且weak只是用来修饰堆区内存对象的，所以这里为null
- 当把block1赋值给强引用类型的变量的时候，block1从栈区被复制到了堆区，成为了新的block2

<font color=orange>**当Block被强指针引用时，会执行copy操作**</font>

```objective-c
MyBlock testFunc() {
    int a = 10;
    return ^ {
        NSLog(@"test --- %d",a);
    };
}

- (void)tet02 {
    NSLog(@"%@", testFunc());
}
```

打印：

```
2020-07-29 20:40:35.590238+0800 RacTest_OC[79491:2968689] <__NSMallocBlock__: 0x6000035b8f00>
```

可以看到，NSStackBlock作为函数返回值时，也是被进行了copy操作，从栈区到了堆区

<font color=orange>**当 Block 做为cocoa API 或 GCD API 的方法参数时也会自动执行 copy 操作**</font>

```objective-c
// cocoa API
[array enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
}];

// GCD 
dispatch_once(&onceToken, ^{
            
});
```



