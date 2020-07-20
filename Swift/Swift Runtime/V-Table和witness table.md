## V-table和witness table

Swift的动态分发是：每个类型都会创建一张表，表内是一个包含了方法指针的数组。

对于class来说，每个类型都会创建虚函数表指针，指向一个叫做V-table的表。拥有继承关系的子类会在虚函数表内通过继承顺序去展示虚函数表指针







#### 相关文章

----

- https://www.jianshu.com/p/6ae6754923b4

