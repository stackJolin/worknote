

# Manual Memery Managment In Swift

#### Swift指针 和 C指针的对应关系 

```
UnsafePointer<T>                      const T *
UnsafeMutablePointer                  T *

UnsafeBufferPointer                   const T * // 指向一个连续的已知类型的区域
UnsafeMutableBufferPointer            T * // 指向一个连续已知对象区域

UnsafeRawPointer                      const void *
UnsafeMutableRawPointer               void *

UnsafeRawBufferPointer                const void * // 指向一个连续未知类型的区域
UnsafeMutableRawBufferPointer         void * // 指向一个连续未知类型的区域

// 注：如果命名中带有'raw'说明是原始指针，也就是未知类型的。否则就是类型指针，因为指针指向的地址存储的内容的类型已知
// 在此观察一下，可以看出，上面的八种基本上由几个关键字组合的’pointer‘、’rawPointer‘、’bufferPoint‘。pointer指的是非可变指针，raw指的是可变指针，而bufferPoint指的是，该结构体指针，实现了collection协议，可以使用各种方法来遍历指针指向的元素
// 说白了，说白了用四种指针，划分维度为’指针是否可变‘以及’是否是指定类型的指针‘，这这两个维度上，又添加了相应的操作指针的维度’collection操作‘。所以构成了8种
```

关于类型指针和原始指针、可变指针和不可变指针、缓冲区指针和非缓冲区指针三个维度的介绍，请参考:https://academy.realm.io/cn/posts/nate-cook-tryswift-tokyo-unsafe-swift-and-pointer-types/



#### 全局操作函数(Memory Access)

----

````
// 将第一个参数T以指针的形式传递给closure
func withUnsafePointer<T, Result>(to: T, (UnsafePointer<T>) -> Result) -> Result
func withUnsafePointer<T, Result>(to: inout T, (UnsafePointer<T>) -> Result) -> Result
func withUnsafeMutablePointer<T, Result>(to: inout T, (UnsafeMutablePointer<T>) -> Result) -> Result
````

```
// 将第一个参数以无类型指针的形式传递给closure
func withUnsafeBytes<T, Result>(of: T, (UnsafeRawBufferPointer) -> Result) -> Result
func withUnsafeBytes<T, Result>(of: inout T, (UnsafeRawBufferPointer) -> Result) -> Result
func withUnsafeMutableBytes<T, Result>(of: inout T, (UnsafeMutableRawBufferPointer) -> Result) -> Result
```

```
//保持变量T在closure里面是不会被销毁的,可以使用在异常调用的时候,可保证变量T的生命同期
func withExtendedLifetime<T, Result>(T, (T) -> Result) -> Result
func withExtendedLifetime<T, Result>(T, () -> Result) -> Result
```

```
//交换两个变量的值
func swap<T>(inout T, inout T)
```



#### 指针操作函数

-----

######pointer

```swift
//将指针的类型转换成一个临时的给定类型的指针传递给closure
func withMemoryRebound<T, Result>(to: T.Type, capacity: Int, (UnsafePointer<T>) -> Result) -> Result
```

```go
//向下移动一位,并返回一个新的指针 
func successor() -> UnsafePointer<Pointee>
//向上移动一位,并返回一个新的指针
func predecessor() -> UnsafePointer<Pointee>
```

###### rawpointer

```swift
//转换给指定类型的指针
func assumingMemoryBound<T>(to: T.Type) -> UnsafeMutablePointer<T>
//转换成指定类型的指针,capacity指定了这个指针读取的T数据数量
func bindMemory<T>(to type: T.Type, capacity count: Int) -> UnsafeMutablePointer<T>

func foo(_ p: UnsafeRawPointer) {
    let buffer = UnsafeBufferPointer<Int>.init(start: p.bindMemory(to: Int.self, capacity: 2), count: 2)
    //let buffer = UnsafeBufferPointer<Int>.init(start: p.assumingMemoryBound(to: Int.self), count: 5)
    buffer.forEach {
        print($0)
    }
    
}
foo([1,2,3,4,5])

在使用时可以发现bindMemory与assumingMemoryBound在操作连续空间时好像没有那么明确的界限了
```

###### bufferPointer

无



#### 指针的创建

-----



#### 指针的转换

-----



#### 其他函数

-----

- unsafeBitCast



#### 相关文章

-----

- https://www.jianshu.com/p/a9956ee1ab61
- 

