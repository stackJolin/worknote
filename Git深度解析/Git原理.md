

## Git原理

Git是一个<font color=red>免费&开源</font>的分布式版本控制系统-DVCS(distributed version control system)，旨在快速、高效的处理从小型到大型项目的所有内容

在成为一个VCS之前，Git首先是一个文件系统，并且这个文件系统是<font color=red>内容寻址(content-address)</font>文件系统。在1.5之前，Git的命令晦涩难懂，主要因为：<font color=green>1.5之前是按文件系统的思维去做版本控制的事</font>，1.5之前才慢慢的逐渐形成目前的使用方式，成为一个易用、高效的版本控制系统



#### Git文件系统

-------

###### Git如何存储内容





Git保存的是一系列不同时刻的文件快照

文件系统(content-address) & 版本控制系统

key-value







#### Git数据存储

-----

四个对象：blob object，tree object，commit object，tag object

Git数据存储



![image-20191206161639128](/Users/houlin/Library/Application Support/typora-user-images/image-20191206161639128.png)

![image-20191206162436235](/Users/houlin/Library/Application Support/typora-user-images/image-20191206162436235.png)



#### 分支到底是个什么东西？

-----

![image-20191206162743367](/Users/houlin/Library/Application Support/typora-user-images/image-20191206162743367.png)

#### Tag是个什么东西？

-----



#### Git如何查看远端分支的源头？

-----



#### Git的存储优化:

-----

- ref packet

- objects packet
- 提交压缩



#### Git merge 和 Git rebase

------



#### Git Hook

---------



#### Git的隐忧

-------

- SHA-1的问题 - google 两张不同的pdf文件，SHA-1的值是一直的