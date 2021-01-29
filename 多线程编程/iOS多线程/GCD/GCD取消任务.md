#### GCD如何取消任务

-------------

在`iOS 8`以后，系统提供了一个函数`dispatch_block_cancel`，只不过这个只能适用于通过`dispatch_block_create`创建的`block`。

```objective-c
-(void)GCD_cancel{
  dispatch_queue_t queue = dispatch_queue_create("concurrent", DISPATCH_QUEUE_CONCURRENT);

  dispatch_block_t block1 = dispatch_block_create(0, ^{
      NSLog(@"1");
  });

  dispatch_block_t block2 = dispatch_block_create(0, ^{
      NSLog(@"2");
  });

  dispatch_block_t block3 = dispatch_block_create(0, ^{
      NSLog(@"3");
  });

  dispatch_async(queue, block1);
  dispatch_async(queue, block2);
  dispatch_async(queue, block3);

  dispatch_block_cancel(block1);

}
```

