## dispatch source简介

-------

`dispatch source`是`GCD`中的一种基本数据类型，称为`调度源`，它用于`处理特定的系统底层事件`。即：`当一些特定的系统低层事件发生时，调度源会捕捉这些事件，然后可以做相应的逻辑处理`



#### dispatch source 可以监听的事件源类型

-----------------

`Dispatch Source`一共可以监听`六类`事件，根据事件的`操作不同`，又可以分为`11`个类型

- `Timer Dispatch Source`：定时事件源

  - `DISPATCH_SOURCE_TYPE_TIMER`

- `Signal Dispatch Source`：监听`UNIX`信号调度源，比如监听`SIGSTOP`-挂起指令信号

  - `DISPATCH_SOURCE_TYPE_SIGNAL`
  - `DISPATCH_SOURCE_TYPE_MEMORYPRESSURE`

- `Descriptor Dispatch Source`：监听文件相关的操作和`Socket`相关操作的调度源

  - `DISPATCH_SOURCE_TYPE_READ`：读文件事件
  - `DISPATCH_SOURCE_TYPE_WRITE`：写文件事件
  - `DISPATCH_SOURCE_TYPE_VNODE`：文件属性更改事件

- `Process Dispatch Source`：监听进程相关的调度源

  - `DISPATCH_SOURCE_TYPE_PROC`

- `Mach port Dispatch Source`：监听`Mach`相关事件的调度源

  - `DISPATCH_SOURCE_TYPE_MACH_SEND`:`Mach`端口发送事件
  - `DISPATCH_SOURCE_TYPE_MACH_RECV`：`Mach`端口接收事件

- `Custom Dispatch Source`：监听自定义事件的调度源

  - `DISPATCH_SOURCE_TYPE_DATA_ADD`：

    ```markdown
    属于自定义事件，可以通过dispatch_source_get_data函数获取事件变量数据，在我们自定义的方法中可以调用dispatch_source_merge_data函数向Dispatch Source设置数据，下文中会有详细的演示。
    ```

  - `DISPATCH_SOURCE_TYPE_DATA_OR`

    ```markdown
    属于自定义事件，用法同上面的类型一样。
    ```

    



#### 如何使用`dispatch source`

-------

- 指定一个希望监听的系统事件类型
- 指定一个捕获到事件后，逻辑处理的闭包或者函数
- 指定执行回调函数的队列

当监听到指定的系统事件发生时，`dispatch source`会将指定回调函数作为一个任务，添加到指定的队列，也就是说，当监听到的系统事件发生时，会将回调函数操作作为一次任务，加入到队列执行

这里与通常的`手动添加任务`的模式不同，一旦将`disptach source`和`dispatch queue`关联后，只要监听到系统事件，`dispatch source`就会自动将回调函数添加到关联的队列中，直到我们调用函数主动取消监听

有些时候回调函数执行的时间较长，在这段时间内Dispatch Source又监听到多个系统事件，理论上就会形成事件积压，但好在Dispatch Source有很好的机制解决这个问题，当有多个事件积压时会根据事件类型，将它们进行关联和结合，形成一个新的事件。



#### 问题

-------

问题1：监听的事件是什么，这些事件都是通过什么方式监听的

问题2：什么是`手动添加监听`，和`自动添加监听`有什么区别

问题3：这些`dispatch source`和`Runloop`中的`source`有什么关系