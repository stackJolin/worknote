## dispatch_queue的基本操作

------

- dispatch_suspend(queue)
- dispatch_resume(queue)



#### dispatch_suspend(queue)

-------

这不会影响当前正在运行的任何任务，而只是阻止新任务在该队列上启动。另外，显然，您仅挂起您创建的队列(不是全局队列，不是主队列)。



#### dispatch_resume(queue)

-------

没有“自动恢复”的概念，因此您只需要在适当时手动恢复它即可。