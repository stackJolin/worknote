## KVO Crash的原因

KVO造成日常崩溃的原因通常有以下几个：

- KVO添加次数和移除次数不一致
- 被观察者被提前释放，被观察者在dealloc时仍然注册着KVO
- 添加了观察者，但是没有实现``observeValueForKeyPath:ofObject:change:context`方法
- 添加或者移除观察时，keypath == nil或者keypath在被观察者中不存在，导致崩溃



#### KVO添加次数和移除次数不一致

----

移除多次报错：

```objective-c
 *** Terminating app due to uncaught exception 'NSRangeException', 
reason: 'Cannot remove an observer <ViewController 0x139e07220> for the key path "name" 
from <YZPerson 0x281322f20> because it is not registered as an observer.'
```



#### 被观察者提前释放

------



#### 没有实现 observeValueForKeyPath:ofObject:change:context 方法

----



#### 添加或者移除观察者时，keypath == nil 或者 keypath本身不存在

----