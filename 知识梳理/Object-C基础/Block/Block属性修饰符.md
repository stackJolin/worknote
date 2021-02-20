## Block的属性修饰符

------------

1.block无论使用`strong`还是`copy`，执行的都是copy操作

2.如果使用`retain`修饰block的时候，相当于assign

3.`block`作为返回值时，会自动copy

3.`block`赋值给变量的时候，会自动copy