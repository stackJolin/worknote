## Block源码详解之内存管理

我们在'Block源码详解'中，了解了block在C++层面的工作机制。这篇文章，我们主要来看，‘Block源码详解’文章中遗留的历史问题：

`__main_block_dispose_0`和`__main_block_copy_0`的工作原理

上面两个函数，在我们用clang生成的C++代码里面是没有实现的，因为他们是在runtime中