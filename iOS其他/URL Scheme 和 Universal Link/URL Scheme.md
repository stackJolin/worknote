## URL Scheme

-----

###### 产生`URL Scheme`的原因：

```markdown
1.由于苹果的app都是在沙盒中，相互是不能访问数据的。苹果提供了一个可以在app之间跳转的方法：URL Scheme。简单的说，URL Scheme就是一个可以让app相互之间可以跳转的协议。
2.每个app的URL Scheme都是不一样的，如果存在一样的URL Scheme，那么系统就会响应先安装那个app的URL Scheme，因为后安装的app的URL Scheme被覆盖掉了，是不能被调用的。
```

###### 什么是`scheme`:

```markdown
简单来说，就是`://`前面的部分，例如`https://www.baidu.com`、`http://www.baidu.com`，`http`和`https`就是所谓的`scheme`
可参考文章：https://juejin.cn/post/6844903697198088199
```

其实，说白了，iOS开发中的`URL Scheme`就是用来在App之前跳转的。举一个场景：`App-A`想跳转到`App-B`的话，`App-A`只需要知道`App-B`的`Scheme`就可以，就可以从`App-A`中，跳转到`App-B`。同样，`App-B`想要跳转到`App-A`，只需要知道`App-A`的`Scheme`，也同样可以跳转到`App-B`。并且这个跳转的过程中，是可以携带参数的



#### `URL Scheme`的使用

------

###### 配置：



###### 捕获跳转：



#### 关于`URL Scheme`的几个问题:

-------

###### 关于`URL Scheme`白名单：

```markdown
`URL Scheme白名单`是`iOS 9`之后才有的，`iOS 9`之前是不需要设置的，如果`iOS 9`之后不添加白名单

在`iOS 9`之前，调用`canOpenUrl`方法，会返回当前App是否真正安装了，但是`iOS 9`之后，出于对隐私保护的考虑，这个方法调用会一直失败。只有添加了`URL Scheme 白名单`，在调用`canOpenUrl`的时候才会返回正确的结果。相当于，目前把`canOpenUrl`的作用范围限定在了`URL Scheme 白名单`中。

需要注意的是，上面的这个改动，并不会应用`openUrl`的调用，这也就是为什么微信没有我们的`scheme`，确依旧能跳转回我们自己App的原因。

另外，白名单的最大数量是`50`个，
```

###### 关于多个App `URL Scheme`重复，以及`URL Scheme 劫持`:

```markdown
我们上面说到过：不同应用的`Scheme`如果相同的话，后安装的会被无视掉，有人测试过：其实是和`Bundle Id`有关系。优先级如下：
1.系统应用的优先级高于一般应用(我们自己开发的应用都是一般应用)
2.对于一般应用来说，同一个`Scheme`的多个应用，优先调用哪一个，其实和`Bundle id`有关，通过精心伪造的`Bundle id`可以在其他`App`之前被调用到，进而拦截跳转，这也是`URL Scheme 劫持`的基础
```



