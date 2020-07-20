## Swift函数参数修饰符inout

如果你有一些C或者是C++的背景,在Swift 中的inout参数前面使用的&符号会给人一种它是传递引用的印象,但是事实并非如此,inout 做的事情是通过值传递,然后复制回来,而并不是传递引用,以下一句话引用《Swift 编程语言》

```````````
inout 参数将一个值传递给函数,函数可以改变这个值,然后将原来的值替换,并从函数中传出。
```````````

验证这个结论的代码：

`````
func test(_ a:inout Int) {
    
    var ptr = withUnsafePointer(to: a) { UnsafeRawPointer($0)}
    print("2:\(ptr)")
}

var a:Int = 10

test(&a)

var ptr = withUnsafePointer(to: a) { UnsafeRawPointer($0)}

print("1:\(ptr)")
//////////////////////////////////////////////
1:0x00007ffee1e819e0
2:0x00007ffee1e81b10
`````



#### 相关文章

----

- https://juejin.im/post/5a309c5751882535cd4ad103
- https://www.jianshu.com/p/4961d0baf182