## Functors、Applicatives and Monads



#### 相关概念

-----

###### 数学原理

```
原理来自数学理论：半群(semigroup)与幺半群(monoid)。范畴论，高阶函数
连接：https://hongjiang.info/semigroup-and-monoid/
连接：https://hongjiang.info/understand-monad-4-what-is-functor/
```



###### 柯里化/Curry

```
只传递给函数一部分参数来调用它，让它返回一个函数去处理剩下的参数(接受多个参数的函数实际上是接受一个参数然后回传一个接受剩余参数的函数)
柯里化的作用：
-	延迟计算
-	参数复用
-	动态创建函数
```



###### 高阶函数

```
函数A作为某个函数B的参数或者返回值，函数B就是高阶函数
```

###### Functor law

```
fdfd
```

######Monoid law

```
1.必须有一个值作为identity
2.一个遵守结合律的二元函数
```





#### 相关文章

-------

- http://learnyouahaskell.com/functors-applicative-functors-and-monoids

- http://learnyouahaskell.com/a-fistful-of-monads

- http://learnyouahaskell.com/for-a-few-monads-more

- http://adit.io/posts/2013-04-17-functors,_applicatives,_and_monads_in_pictures.html

- http://blog.leichunfeng.com/blog/2015/11/08/functor-applicative-and-monad/

- Haskell：http://learnyouahaskell.com/chapters。中文版本1：https://github.com/MnO2/learnyouahaskell-zh，中文版本2：https://wiki.jikexueyuan.com/project/haskell-guide/functors-applicative-functors-and-monoids.html

  

- 