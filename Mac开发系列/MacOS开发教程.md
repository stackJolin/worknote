## MacOS开发教程



#### 窗口对象

------

KeyWindow(keyboard window)：可以接收输入事件(鼠标、键盘、触摸板等)的窗口对象

mainWindow：当前的活动窗口

一个时刻，只能有一个keywindow和一个mainWindow

![image-20191205202744325](/Users/houlin/Library/Application Support/typora-user-images/image-20191205202744325.png)

keyWindow和mainWindow可以使同一个窗口，也可以是不同的窗口。当mainWindow可以接收输入事件时，他同时也是keyWindow

![image-20191205202941291](/Users/houlin/Library/Application Support/typora-user-images/image-20191205202941291.png)



面板

NSPanel、NSColorPanel、NSFontPanel、NSSavePanel等子类化的Window只能做mainWindow不能做keyWindow



###### 窗口界面组成

- titleBar
  - 控制按钮
  - 标题
- contentView
- contentBorder



###### 模态窗口



###### NSWindowController和NSWindow的关系

你中有我，我中有你。NSWindowcontroller强引用NSWindow，NSWindow弱引用NSWindowController

结论：手工创建的NSWindow，关闭后，系统会检查这个Window有没有NSWindowController引用它，有的话就释放这个window对象。所有注意这个window的释放问题，注意crash





#### 视图控制器

-----

NSViewController的生命周期

NSView的生命周期

###### 视图控制器和窗口控制器的关系

- 视图与窗口：窗口依赖视图而存在。窗口必须有一个根视图，即内容视图contentView
- 每个视图都存在于一个窗口中，可以通过self.view.window获取view的window







#### 鼠标键盘事件

-------



#### 20 - 多文档应用

-----



#### 25 - 多线程

------



#### 26 - RunLoop

------

###### RunLoop存在的意义

- 因为一个线程一次只能执行一个任务，执行完成后线程就会退出。如果需要线程一直可以响应不退出， EventLoop 模型（事件循环）可以满足这样的需求。使用事件循环，需要考虑这样几个问题：
  - 如何管理事件、消息？
  - 如何在无任务处理时避免资源占用？
  - 如何在有任务时被唤醒？

需要关注的问题：线程、Runloop的关系

多线程解决了多任务并发的问题，提高了系统性能。在现代交互系统中，还存在大量不确定的未知不确定的异步事件，这时候现成是一直是出于等待状态的，直到有事件发生才去唤醒现成去执行，执行完成后，系统又恢复到以前的等待状态。如何控制线程在等待和执行任务状态间无缝切换，就引入了runloop的概念

每种事件会存储到不同的FIFO先进先出队列，等待Runloop依次处理。被Runloop管理的线程挂起时，不会占用系统的CPU资源，可以说Runloop是非常搞笑的线程管理技术

线程和runloop是一一对应的

GCD和Runloop没有直接的关系，主线程自动绑定到一个Runloop，所以推出一个结论：GCD中提交到主线程队列的任务会在主线程Runloop循环周期内调用

Runloop三大核心组件：定时器、输入源Input Sources、观察者Observer



###### RunLoop有两种事件源

- input source：输入源，传输异步事件
- timer：定时源，传递同步事件



###### RunLoop的三种状态

- 睡眠

- 唤醒

- 处理事件

  睡眠 -> 唤醒 -> 处理事件->睡眠->唤醒->处理事件………..。当有事件需要处理时，RunLoop 被唤醒开始处理事件，事件处理结束后，RunLoop 重新进入睡眠状态。



###### Runloop的Modes

```
Runloop在每个时刻运行一个特定的模式Mode
Runloop在运行时，只处理注册到当前Mode下的事件和通知模式相关的观察者，当前模式运行区间发生的其他模式的事件并不会被处理，只能被存储到消息队列等到Runloop下一次切换到对应的Mode时，才能被处理

注册模式的原则：如果不是高优先级需要实时处理的事件，可以采用默认模式。如果Runloop运行在任何模式都需要处理这个事件，那么这个事件就注册在NSRunloopCommonModes/kCFRunLoopCommonModes Common模式


```



###### RunLoop的活动状态

```

```





###### Runloop事件处理流程

```

```



###### RunLoop使用场景

```

```



###### RunLoop的Input Source

```
RunLoop有三种Source：
1.基于Port的Source：基于Port的Source是Cocoa系统内部两个线程间类型TCP/IP以端口的方式通讯的一种机制
2.Perform Selector Sources：
3.用户自定义的Source
```



#### iOS和OSX对比

---

`iOS是完整OSX精简之后的版本，和OSX有两大主要区别：1.iOS的架构是基于ARM，而OSX是基于intel x86或x86_64；2.为了满足移动设备的局限性和特性需求，有一些组件被简化或者干脆被移除了`