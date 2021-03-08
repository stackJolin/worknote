## GLSL介绍

------

`着色器(shader)`是运行在`GPU`上的小程序，这些小程序为图形渲染管线的某个特定的部分而运行。从基本意义上来讲，着色器只是一种把输出转换成输出的程序。着色器也是一种非常独立的程序，因为各个着色器之间不能相互沟通，他们之间唯一的沟通只有通过输入和输出

而编写这些`着色器`的语言，叫做`GLSL`。`GLSL`是一种类`C`语言，专门为图形计算量身定制的，它包含一些对向量和矩阵操作的有用特性



#### GLSL程序的一般组成

----------

```glsl
#version version_number

in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

int main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```

- 首先声明版本
- 接下来说输入和输出变量
- 在接下来是`uniform`，什么是`uniform`我们后面将
- 在接下来就是man函数。这个函数里面，我们会处理所有的输入的数据，将处理的结果放到输出的变量中





#### GLSL数据类型

------

`GLSL`的数据类型除了包含C中的基本数据类型(`int`,`float`,`double`,`uint`,`bool`)外，还包含两个容器类型：

- 向量(Vector)
- 矩阵(Matrix)

###### 向量:

GLSL中的向量是一个可以包含有1、2、3或者4个分量的容器，分量的类型可以是前面默认基础类型的任意一个。它们可以是下面的形式（`n`代表分量的数量）：

| 类型    | 含义                            |
| ------- | ------------------------------- |
| `vecn`  | 包含`n`个float分量的默认向量    |
| `bvecn` | 包含`n`个bool分量的向量         |
| `ivecn` | 包含`n`个int分量的向量          |
| `uvecn` | 包含`n`个unsigned int分量的向量 |
| `dvecn` | 包含`n`个double分量的向量       |

大多数时候我们使用`vecn`，因为float足够满足大多数要求了。





#### GLSL输入与输出

------

虽然着色器是各自独立的小程序，但是它们都是一个整体的一部分，出于这样的原因，我们希望每个着色器都有输入和输出，这样才能进行数据交流和传递。GLSL定义了`in`和`out`关键字专门来实现这个目的。每个着色器使用这两个关键字设定输入和输出，只要一个输出变量与下一个着色器阶段的输入匹配，它就会传递下去。但在顶点和片段着色器中会有点不同。











#### 相关文章

-----

https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/05%20Shaders/