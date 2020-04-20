## ReactiveCocoa

#### 关键类

- RACStream
  - RACSignal
  - RACSequence
- RACSignal：一对一单向数据流
- RACMuticastConnection：一对多单向数据流
- RACCompoundDisposable
- RACSubject:
- RACScheduler:
  - RACImmediateScheduler
  - RACQueueScheduler
    - RACTargetQueueScheduler
  - RACSubscriptionScheduler
  - RACTestScheduler
- RACChannel：一对一双向数据流
- RACReplaySubject：可以先发送信号再订阅信号。每次被订阅的时候，会将原有的值发送一遍，send的时候，发送新值
- RACCommand:
- RACTuple:
- RACSequence:



#### ReactiveCocoa的核心模块

----

- 信号源：`RACStream` 及其子类

- 订阅者：`RACSubscriber` 的实现类及其子类
- 调度器：`RACScheduler` 及其子类
- 清洁工：`RACDisposable` 及其子类



#### Rac所解决的问题

-----





#### 其他知识点

------

- OC中的泛型

  `@interface RACStream<__covariant ValueType> : NSObject`

  - __covariant(协变):用于泛型数据强转类型，可以向上强转，子类可以转成父类。
  - __contravariant(逆变):用于泛型数据强转类型，可以向下强转，父类可以转成子类。

- dispatch_set_target_queue

  - 设置队列优先级
  - 建立队列的执行阶层

#### 链接

----

- https://www.open-open.com/lib/view/open1487303366014.html