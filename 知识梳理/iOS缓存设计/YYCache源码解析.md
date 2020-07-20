## YYCache源码解析

#### YYCache主要技术点

----

- 内存缓存(YYMemoryCache)
  - 思想： hashMap+双向链表
    - hasMap用于高速读取
    - 双向链表用于维护缓存表(基于LRU算法)
- 磁盘缓存(YYDiskCache)
  - 文件缓存
  - SQLite3数据库

#### 类

------

###### 内存缓存

- YYMemoryCache
- _YYLinkedMap
- _YYLinkedMapNode

###### 磁盘缓存