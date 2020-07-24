## copy和strong的区别

我们先约定一个前提：这里说的copy，值得是@property中的属性关键字copy。

理解下面的内容，建议先看之前的内容`深拷贝和浅拷贝`

那么我们下问几个问题：

- @property中的copy是深拷贝(copy)还是浅拷贝(mutableCopy)

- copy一般用来修饰那些类型，为什么

- copy修饰的属性，那些情况下可以用strong，那些情况下不能用strong

  

#### @property中的copy是深拷贝(mutableCopy)还是浅拷贝(copy)

----

先说一下结论：有可能是浅copy也有可能是深copy，因为归根结底，当使用copy这个关键字的时候，就是对被复制的对象执行copy操作，然后copy之后的结果赋值给对象所对应的数组属性所对应的成员变量。

```objective-c
@interface OCCopyTest : NSObject

@property(nonatomic , copy) NSObject *obj1;

@property(nonatomic , copy) OCCopyTest *obj2;

@property(nonatomic , copy) NSArray *arr1;
@property(nonatomic , copy) NSMutableArray *arr2;

@property(nonatomic , strong) NSArray *arr3;
@property(nonatomic , strong) NSMutableArray *arr4;


+ (void)copyTest;
+ (void)copySetTest;
+ (void)testPropertyCopy;

@end

+ (void)testPropertyCopy {
    OCCopyTest *test1 = [OCCopyTest new];
    test1.arr1 = @[@1, @2];
    test1.arr2 = [NSMutableArray arrayWithArray:@[@1, @2]];
    
    test1.arr3 = @[@1, @2, @3];
    test1.arr4 = [NSMutableArray arrayWithArray:@[@1, @2, @3]];
    
    NSArray *arr1 = @[@1, @2, @3, @4];
    NSMutableArray *arr2 = [NSMutableArray arrayWithArray:@[@1, @2, @3, @4, @5]];;
    
    test1.arr1 = arr1;
//    test1.arr2 = [arr2 copy];
    test1.arr2 = arr2;
    // 这里会崩溃，说明copy关键字，就是对被复制的对象执行了一下copy方法，也就是 test1._arr2 = [arr2 copy]
    // [test1.arr2 addObject:@[@0]];
    
    test1.arr3 = arr1;
//    test1.arr4 = [arr2 copy];
    test1.arr4 = arr2;
    [test1.arr4 addObject:@[@0]];
    
    NSLog(@"\n原地址：arr1:%@, arr2:%@\n", arr1, arr2);
    
    NSLog(@"\ntest1.arr1-地址：%p，类型：%@ ，第一个元素的地址：%p \
          \ntest1.arr2-地址：%p，类型：%@ ，第一个元素的地址：%p \
          \ntest1.arr3-地址：%p，类型：%@ 第一个元素的地址：%p \
          \ntest1.arr4-地址：%p，类型：%@ 第一个元素的地址：%p",
          test1.arr1, [test1.arr1 class], test1.arr1[0],
          test1.arr2, [test1.arr2 class], test1.arr2[0],
          test1.arr3, [test1.arr3 class], test1.arr3[0],
          test1.arr4, [test1.arr4 class], test1.arr4[0]);
}
```

打印：

```objective-c
2020-07-24 19:43:08.201811+0800 RacTest_OC[89093:1356149] 
原地址：arr1:0x6000007683c0, arr2:0x600000768450

2020-07-24 19:43:08.201985+0800 RacTest_OC[89093:1356149] 
test1.arr1-地址：0x6000007683c0，类型：__NSArrayI ，第一个元素的地址：0xd1a463f5469f5daf           
test1.arr2-地址：0x600001c5c640，类型：__NSArrayI ，第一个元素的地址：0xd1a463f5469f5daf           
test1.arr3-地址：0x6000007683c0，类型：__NSArrayI ，第一个元素的地址：0xd1a463f5469f5daf           
test1.arr4-地址：0x600000768450，类型：__NSArrayM ，第一个元素的地址：0xd1a463f5469f5daf
```

结果属性的成员变量

- test1._arr1 = [arr1 copy]
- test1._arr2 = [arr2 copy]
- test1._arr3 = [arr3 copy]
- test1._arr4 = [arr4 copy]

关于copy方法详解见<a src='./深拷贝和浅拷贝.md'>深copy和浅copy</a>



#### 所以，什么情况下@property中要使用copy关键字呢？

----

其实，当某个对象A的属性B，在执行A.B=C之后，不希望对于C本身的改动波及到B上。举几个例子：

- 将NSMutableString的属性，赋值给属性类型为NSString类型的属性
- 将NSMutableArray的属性，赋值给属性类型NSArray类型的属性
- 将NSMutableDictionary的属性，赋值给属性类型为NSDictionary类型的属性
- 将NSMutableData的属性，赋值给属性类型为NSData类型的属性

。。。。

由于NSMutableString、NSMutableArray、NSMutableDictionary、NSMutableData类型的数据obj，执行A.B = [obj copy]之后，生成的是一个新的不可变的对象，和原来的对象除了子元素引用之外，没有任何关系。所以对于obj本身的改动，不会波及到A.B的数据