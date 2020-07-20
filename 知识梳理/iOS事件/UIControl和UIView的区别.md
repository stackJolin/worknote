## Block内存详解

https://www.jianshu.com/p/4a6dca34d980

https://github.com/nst/iOS-Runtime-Headers/tree/master/Frameworks/CoreFoundation.framework

block源码：https://opensource.apple.com/source/libclosure/libclosure-67/

https://blog.leichunfeng.com/blog/2015/06/26/objective-c-associated-objects-implementation-principle/



#### 结论

-------

本来在栈区的Block在被放入堆区之后，如果let blockA = 堆区block，blockA是一个栈区block



问题：什么时候会从栈区讲block复制到堆区