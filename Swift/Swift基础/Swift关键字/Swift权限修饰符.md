## Swift权限修饰符

------

`Swift`中一共有五种权限修饰符:

- fileprivate
- private
- internal
- public
- open



#### private

------

`private`所修饰的属性或者方法，只能在当前类里访，无论是类的本类、类的分类、是否在同一个文件，都可以访问



#### fileprivate

--------

`fileprivate`所修饰的方法或者属性，只能在当前的文件中可以访问



#### internal(默认访问级别，internal修饰符可写可不写)

-------

`internal`访问级别所修饰的属性或者方法，在当前模块中均可访问。

如果是框架或者库，则在框架内或者库内部可以访问，外部不可以访问



#### public

---------

`public`修饰的属性或者方法，可以本任何模块访问，无论是本模块或者是其他模块，但是：

- 只能在模块内`override`和继承
- 本模块外不`override`和继承



#### open

-------

`open`所修饰的属性，可以被任何模块继承或者`override`