##XAML界面原理和语法

XAML全称：Extensible Application Markup language，是用于实例化.NET对象的标记语言。XAML是微软技术体系中的UI编程语言。注意：游戏除外。

Windows 10应用程序中的界面是由xaml文件组成的，和这些xaml文件一一对应的是xaml.cs文件，这就是微软典型的Code-Behind(代码后置)模式编程方式。xaml文件的语法类似于XML+HTML的结合体，这是微软XAML语言特有的语法结构



####XAML四大原则

- XAML是区分大小写的，元素和属性名称是区分大小写的
- 所有的属性值，无论他是什么类型，都需要双引号
- 所有的元素必须是封闭的
- 最终的XAML文件也必须是合适的XML文档



####XAML命名空间

关键字`xmlns`,专门用来声明命名空间

Window10两个关键的命名空间

- `http://schemes.microsoft.com/winfx/2006/xaml/presentation`是window10的核心命名空间，包含了用来构建用户见面的控件类
- `http://schemas.microsoft.com/windfx/2006/xaml`是XAML命名空间，包含了各种XAML特性，这些特性可以影响文档的解析方式



####XAML对象元素

page作为根元素，作为当前界面的最顶层的元素



####XAML的原理



####动态加载XAML

关键字：XamlReader





XAML的树结构

----

XAML是界面编程语言，树形结构，对象树，可视化树，VisualTreeHelper