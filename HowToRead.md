(每次想读大型项目的源码却无处着手的时候就会想：要是有个HowToRead就好了)

# 如何阅读这些源码

本文尽量指导想要阅读源码者看懂这些代码，并理解其中的原理，明白我的思路。

推荐按以下顺序阅读。

### 1. allocator

首先要阅读的是STL的“幕后英雄”`allocator`（内存分配器）的源码。

它在各个STL容器的运作过程中起着重要的作用。

它会为指定数量的对象分配内存空间，但不会实际构造出那么多对象。

代码位于./memory/allocator.h

### 2. uninitialized_memory操作函数

这也是制作容器时要用到的一系列函数。负责填充/将数据复制到未初始化的内存中。

代码位于./memory/uninitialized_memory_functions.h

### 3. STL的灵魂：Iterator和Traits

STL的算法可以通过迭代器来直接访问容器内的数据而无需考虑容器内部数据的组织方式。

而Traits编程技法部分弥补了C++没有反射的缺点，它可以将迭代器的各种信息“萃取”出来，让STL在利用模版偏特化针对不同类型的迭代器实现不同的算法的同时对外呈现的接口仍然相同（而且并不损失运行效率）。

你可能需要结合一些STL算法的实现才能明白Traits究竟是如何工作的。（推荐阅读较为简单的`advance`和`distance`函数的源码，它就在文件的下方）。

代码位于./memory/iterator_traits.h

### 4. 第一个容器

由于我校教学顺序的问题，我们看的第一个容器不是常见自制STL过程中最常见的`vector`，而是单向链表`forward_list`。（这样安排自有学校的道理，因为`forward_list`的迭代器类型是最简单的单向迭代器，而不会涉及到反向迭代器等一系列问题）

SGI STL对于`forward_list`（当时还叫`slist`）的实现非常精妙，它的头节点是一个“只有`next`却不持有数据”的特殊节点，这样一来在处理链表内部数据的时候无需在针对头节点进行特判，十分方便，这里的容器实现也采取了这种方式。

`forward_list`在STL容器中属于比较特殊的一个，使用的方式不同于正常的STL容器，很多方法都是`*_after`，即针对某一元素之后的元素进行操作，这无疑是为了效率考虑。

另外，在实现`forward_list`的过程中，我们第一次认识到了traits技法在实现容器过程中也是非常必要的，并体验了一把利用`type_traits`进行`dispatch`的滋味。

在实现过程中我们顺手实现了`pair`。

forward_list代码位于./memory/iterator_traits.h

type_traits代码位于./type_traits/ 下，此处用到了is_integral.h及其间接`#include`的部分。

pair相关代码位于./utility/utility.h