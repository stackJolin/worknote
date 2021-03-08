## isMemberOfClass、isKindOfClass、isSubclassOf

---------------



#### isMemeberOfClass

-----------

实例方法，当前对象所属的类必须和参数类完全匹配，子类也不行



#### isKindOfClass

------

实例方法，当前对象所属的类和参数类是同一个类，或者是参数类的子类



#### isSubclassOf

-------

isSubclassOf 首先是类方法，用类调用。如果调用该方法的类是参数类的一个子类，或者就是参数类本身的时候返回YES，否则返回NO。