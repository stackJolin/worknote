## GCD高级用法四：dispatch_group

-----------





#### dispatch_group相关方法

----

**<font color='red'>dispatch_group_create</font>**

```objective-c
创建一个任务监听组
```



**<font color='red'>dispatch_group_async</font>**

```objective-c

```

**<font color='red'>dispatch_group_enter</font>**

```objective-c

```

**<font color='red'>dispatch_group_leave</font>**

```objective-c

```

**<font color='red'>dispatch_group_notify</font>**

```object

```

**<font color='red'>dispatch_group_wait</font>**

```objective-c

```



#### 源码分析

-----

相关文章：https://www.jianshu.com/p/e93fd15d93d3



#### dispatch_group用法一：实现线程同步

-----



#### dispatch_group用法二：实现并发编程

-----





#### 使用场景一：朋友圈、微博图片上传后，统一回调结果

------

```objective-c
NSMutableArray *imageURLs= [NSMutableArray array];
dispatch_group_t group = dispatch_group_create();                    // 1
for (UIImage *image in images) {
    dispatch_group_enter(group);                                    // 2
    sendPhoto(image, success:^(NSString *url) {
        [imageURLs addObject:url];
        dispatch_group_leave(group);                                 // 3
    });
}
dispatch_group_notify(group, dispatch_get_global_queue(), ^{         // 4
    postFeed(imageURLs, text);
});
```

想一下，如果用`dispatch_group_async`怎么实现呢？







