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

**<font color='orange'>NSGlobalBlock:</font>**

没有引用局部变量的block叫做NSGlobalBlock

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

