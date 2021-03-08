## Swift属性分类

---------

`swift`中的属性，主要分为以下几种：

- 存储属性
  - 实例存储属性
  - 懒加载属性
- 计算属性
  - 计算属性
  - 只读计算属性
- 属性监听
  - willSet
  - didSet
- 属性包裹器
- 全局和局部变量
- 类属性



#### 存储属性

-------





#### 计算属性

--------





#### 属性监听

---------

```swift
class StepCounter {
    var totalSteps: Int = 0 {
        willSet(newTotalSteps) {
            print("About to set totalSteps to \(newTotalSteps)")
        }
        didSet {
            if totalSteps > oldValue  {
                print("Added \(totalSteps - oldValue) steps")
            }
        }
    }
}
```

需要注意的是，那些属性可以使用属性监视器：

- 定义的存储属性
- 继承的存储属性
- 继承的计算属性







