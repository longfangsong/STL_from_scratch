# STL from scratch

> We should build a wheel which is easy to disassemble.

这是一个妄图从0开始实现STL的项目，目标之一是创建一个比较用于适合学习数据结构、算法与C++高级技巧的样例代码库。

（就像minix的源码对于学习操作系统的意义那样）

也可以算作是本人阅读《STL源码分析》、《算法导论》和学习数据结构、算法的读书笔记和代码片段仓库。

其中容器和算法的实现尽量为理解数据结构和算法本身优化（而非为效率优化）。

项目中将会用到C++的这些特性：

- 面向对象部分
- 模版
  - 包括traits技法在内的比较高级的模版技巧
- 面向过程部分
- `::new`、`::delete` 等运算符
- 标准异常

项目中不会用到C++的这些特性：

- 现有的STL :-) 
  - 目前只能做到尽量不用

如果你对如何阅读代码手足无措，可以阅读HowToRead.md