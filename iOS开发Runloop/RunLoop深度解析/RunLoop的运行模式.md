#### RunLoop的运行模式

------

`RunLoop`一共有五种运行模式：

- kCFRunLoopDefaultMode

  ```markdown
  App的默认运行模式，通常主线程是在这个运行模式下运行
  ```

- UITrackingRunLoopMode

  ```markdown
  跟踪用户交互事件（用于 ScrollView 追踪触摸滑动，保证界面滑动时不受其他Mode影响）
  ```

- kCFRunLoopCommonModes

  ```markdown
  伪模式，不是一种真正的运行模式
  ```

- UIInitializationRunLoopMode

  ```markdown
  在刚启动App时第进入的第一个Mode，启动完成后就不再使用
  ```

- GSEventReceiveRunLoopMode

  ```markdown
  接受系统内部事件，通常用不到
  ```

  

