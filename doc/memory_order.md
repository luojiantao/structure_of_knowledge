# memory order
## 不同角度 指令顺序理解
###单线程角度
- 编译器和cpu只能对单线程代码做出优化
- 代码的执行顺序叫做PO，在实际执行中，在不改变单线程语义的情况下，编译器和CPU会做执行顺序的优化
- 每个单线程只关心看到自己相关的变量（包括全局变量），如果这个全局变量被其他线程同时引用。由于执行单线程的指令优化可能会有不是预期的结果。需要使用者（程序员）设置执行顺序（memory order）
### 多线程执行模型
![png\v2-10ad4b3f7f4a6a18229562c7316f8303_hd.jpg](png\v2-10ad4b3f7f4a6a18229562c7316f8303_hd.jpg)

- program order
	单线程中，代码显示的执行顺序。可能会在实际执行时改变顺序。顺序在编译阶段就确定了
- memory order
	memory order是全局的多处理器对共享变量的操作的一个排序。对共享变量操作落在memory order这条线上的点就是对外表现出的执行顺序。  
	全局指令实际执行顺序，包括所有的多线程program order.  
	不同线程的program order实际执行顺序的组合。由程序运行阶段确定。组合就像 两幅扑克牌洗牌组合。单线程中保持实际执行顺序不变，多线程顺序随机组合。

### 线程视角
只能看到自己执行的代码，变量。



### c++ 内存顺序（memory order）
#### 目的
解决单线程program order 优化导致的执行顺序的改变。那些全局变量的操作顺序的改变，虽然不改变单线程语义。但是完成没有考虑多线程。 限制编译器和CPU作某些不必要的优化
#### 模型
1. relaxed memory consistency（宽松型）
	实现对于load与store顺序完全放开，除了对同一memory location的操作保证load看到是最新的store以外其他都不进行约束，编码人员如果想强加order可以通过的FENCE。  
	relaxed order允序单线程上不同的memory location进行reorder，但是对于同一个memory location不能进行reorder。
	
2. 