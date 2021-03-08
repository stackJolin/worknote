## Swift 5.x关键字mutating

-------

使用场景：

- 实例方法中修改属性
- 在`mutating`修饰的方法中，赋值给`self`
- 枚举的`mutating`用法



#### 实例方法中修改属性

------------

背景：

```markdown
结构体和枚举是值类型. 默认情况下, 值类型属性不能被自身的实例方法修改.
```

例如：

```swift
struct Point {
    var x = 0.0, y = 0.0
    func moveBy(x deltaX: Double, y deLtaY: Double) {
        x += deltaX
        y += deLtaY
    }
}

var somePoint = Point(x: 1.0, y: 1.0)
somePoint.moveBy(x: 2.0, y: 2.0)
print("The point is now at (\(somePoint.x), \(somePoint.y))")
```

这个时候就会报错：

```swift
Left side of mutating operator isn't mutable: 'self' is immutable
```

可以通过`mutating`关键字达到上面的目的：

```swift
struct Point {
    var x = 0.0, y = 0.0
    mutating func moveBy(x deltaX: Double, y deLtaY: Double) {
        x += deltaX
        y += deLtaY
    }
}

var somePoint = Point(x: 1.0, y: 1.0)
somePoint.moveBy(x: 2.0, y: 2.0)
print("The point is now at (\(somePoint.x), \(somePoint.y))")
```



#### 在`mutating`修饰的方法中，重新给自己赋值

------

```swift
struct Point {
    var x = 0.0, y = 0.0
    mutating func moveBy(x deltaX: Double, y deLtaY: Double) {
        self = Point(x: 3.0, y: 3.0)
    }
}
```



#### 枚举的`mutating`方法

-------

```swift
enum TriStateSwitch {
    case off, low, high
    
    mutating func next() {
        switch self {
        case .off:
            self = .low
        case .low:
            self = .high
        case .high:
            self = .off
        }
    }
}

var ovenLight = TriStateSwitch.low
ovenLight.next()
ovenLight.next()

print(ovenLight)
```

