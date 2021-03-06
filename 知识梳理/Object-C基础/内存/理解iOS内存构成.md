## 理解iOS内存构成

#### 内存的种类

- ###### 系统内存(System Memory)

  - 物理内存(Physical Memory)

    ```
    就是实际的物理限制，现在的操作系统都不会直接去操作物理内存
    ```

  - 虚拟内存(Virtual Memory)

    ```
    每当启动一个进程的时候都会创建一个logical address space，和物理内存或者其他应用程序的虚拟地址都不对称。
    ```

  ```
  系统将地址空间分成相同大小的块，称作页（page）。进程和内存管理单元包含了一个分页表（page table）来管理分页，在程序运行的时候就通过这个分页表转换到实际的硬件内存地址。
  
  早版本的iOS页的大小，页的尺寸是4kb，在最新的iOS，A7和A8的系统开放了16kb的64位用户空间对应了4kb的物理页，A9的时候开放了16kb的页并且对应了16kb的物理页。
  
  虚拟内存包含很多区域，包括代码部分（Code Segments），动态库(Dynamic Libraries)，GPU驱动内存(GPU Driver Memory)，malloc堆(malloc heap)和其他的。
  ```

- ###### GPU驱动内存(GPU Driver Memory)

  ```
  由虚拟内存组成，用于驱动，本质上就是IOS的显存。iOS中所谓统一架构，CPU和GPU共享相同的内存（虽然现代硬件的GPU有更大的传输带宽）大多数内存申请都在驱动中完成，并且大多数都是贴图和网格信息。
  ```

- ###### Malloc堆

  ```
  堆内存是虚拟内存中应用能够申请的部分（通过malloc和calloc函数）。
  也就是内存申请允许访问的地方。
  苹果没有最大程度上地开放堆内存，理论上虚拟地址只被指针大小限制（比如64位那就有2的64次方的bytes），这是进程架构决定的。
  ```

- ###### 常驻内存

  ```
  常驻内存是游戏实际使用的物理内存数量。
  一个进程能够申请一个虚拟内存块，但是系统实际上是给了一个相符的物理内存块然后进行写入。这种情况，这个申请的物理内存块就是这个程序的常驻内存。
  ```

- ###### 分页

  ```
  分页是移动物理内存页从内存中放到后台储存中。
  进程申请内存的时候会将空闲的内存块申请出来并且标志为常驻内存。
  
  分页是移动物理内存页从内存中放到后台储存中
  
  当一个进程申请了块虚拟内存，系统会寻找在物理内存中的空闲的内存页并且将它们映射到已申请的虚拟内存页上（因此将这些内存页作为程序的常驻内存）
  
  如果在物理内存中已经没有可使用的部分的话，系统将根据平台尝试释放已经存在的页，以保证有足够的空间申请新的页。通常情况下，一些使用比较少的页会被移动到后备储存中，并且像一般的文件一样进行储存下来这被称作 **paging out**.
  
  但在iOS上没没有后台储存，所以页不会page out。但是只读也依旧可以被从内存中移除并且在需要的情况下从磁盘中重载，进程的这种行为被称为**page in**
  
  如果当前请求的应用程序申请的地址并不在当前的物理内存上，会产生一个页错误。当这种事情发生时，虚拟内存系统调用一个特殊的也错误处理器来应对这种情况，定位一个空闲物理内存，从后备储存中加载包含所需数据的页，更新page table，然后归还代码的控制权。
  ```

- ###### Clean Memory

  ```
  是一个应用常驻内存的只读内存页集，iOS能够安全地从磁盘中移除或重载。
  内存申请时将以下的这些看做是Clean的：
  
  * 系统framework
  * 程序的二进制可执行文件
  * 内存映射文件
  
  当一个应用程序链接到framework上，Clean Memory集合会增加二进制framework文件的尺寸。但大多时候，只有一部分二进制文件被加载到物理内存中。
  因为Clean Memory是只读的所以，应用程序可以共享framework以及library，就像其他只读或者写时拷贝的页一样。
  ```

- ###### Dirty Memory

  ```
  DirtyMemory是无法被系统移除的常驻内存部分。
  因为他是脏的数据……
  <…>
  
  ### 交换压缩内存（Swapped Compressed Memory）
  swapped（Compressed Memory）是Dirty Memory的一部分，是被系统认为用的比较少并且放在一个被压缩的区域。
  
  用于计算移动和压缩这些内存块的算法并没有被开放出来，但是测试显示iOS经常频繁调用这个算法，以此来降低Dirty Memory的数量。
  ```

- ###### Unity内存

  ```
  
  ```

  



#### 相关文章

- [https://zhongwuzw.github.io/2018/06/13/iOS%E5%86%85%E5%AD%98%E7%AE%A1%E7%90%86%E4%B9%8BSwapped-Memory/](https://zhongwuzw.github.io/2018/06/13/iOS内存管理之Swapped-Memory/)