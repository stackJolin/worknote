## Core Reactive 基础

- Signal
- Event
- SignalProducer
- Property
- Action
- Lifetime



#### Signal

------

单向事件流

一个`信号`的拥有者，对事件流有单边控制权。观察者可以在未来的事件中，随时登记他们的兴趣，但是观察者对信号并没有副作用

一个`信号`就像电视直播--你可以观看并且做出相应的动作，但是你不能对直播流和电视台有任何的副作用



#### Event

----

事件流的基本传输单元

一个`信号`可以拥有任意数量的带有payload的事件，在这些事件的最后，会有一个因为某些原因而存在的结束事件

一个`事件`就像是一次性直播中的某一`帧`-帧包含着视频数据和音频数据，直播会被一个特殊的帧，这个帧标识着直播流的结束



#### SignalProducer

-----



#### Action

-----



#### Lifetime

-----



#### 基础

------



#### 函数式编程、命令式编程、声明式编程

---

- https://www.cnblogs.com/sirkevin/p/8283110.html
- https://blog.csdn.net/bitcarmanlee/article/details/53158840



#### 相关资料

-------

- https://www.jianshu.com/p/e92bac9570c0?utm_campaign=maleskine&utm_content=note&utm_medium=seo_notes&utm_source=recommendation
- https://tech.meituan.com/2015/09/08/talk-about-reactivecocoas-cold-signal-and-hot-signal-part-1.html