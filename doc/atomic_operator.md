# 什么才是原子操作
参考链接[http://www.parallellabs.com/2010/04/15/atomic-operation-in-multithreaded-application/](http://www.parallellabs.com/2010/04/15/atomic-operation-in-multithreaded-application/)
## 原子操作定义
原子操作就是不可再分的操作。在多线程程序中原子操作是一个非常重要的概念，它常常用来实现一些同步机制，同时也是一些常见的多线程Bug的源头。

## 多线程程序中对变量的读写操作是否是原子的?
根据Intel手册8.1.1节的介绍,从Intel486 processor开始，以下的基本内存操作是原子的：
1. 一个字节的读写
2. 对齐到16位边界的字的读写
3. 对齐到32位边界的双字的读写
4. 对齐到64位边界的四字的读写
5. 未缓存且在32位数据总线范围之内的内存地址的访问
6. 对单个cache line中缓存地址的未对齐的16/32/64位访问
- 非原子操作定义
	说点简单点，那些被总线带宽、cache line以及page大小给分隔开了的内存地址的访问不是原子的
```C++
int x;
int y = 2;
x = 1;//原子的
/*
因为x是int类型，32位CPU上int占32位，在X86上由硬件直接提供了原子性支持。实际上不管有多少个线程同时执行类似x=1这样的赋值语句，x的值最终还是被赋的值（而不会出现例如某个线程只更新了x的低16位然后被阻塞，另一个线程紧接着又更新了x的低24位然后又被阻塞，从而出现x的值被损坏了的情况）
*/
x++;//非原子
++x;//非原子
/*
其实类似x++, x+=2, ++x这样的操作在多线程环境下是需要同步的。因为X86会按三条指令的形式来处理这种语句：从内存中读x的值到寄存器中，对寄存器加1，再把新值写回x所处的内存地址（见上面的反汇编代码）
*/
x = y;//非原子
```
```
x = y;
mov eax,dword ptr [y]
mov dword ptr [x],eax
 
x++;
mov eax,dword ptr [x]
add eax,1
mov dword ptr [x],eax
 
++x;
mov eax,dword ptr [x]
add eax,1
mov dword ptr [x],eax
 
x = 1;
mov dword ptr [x],1
```
## 多线程程序中对Bit field（位域）的读写操作是否是线程安全的？
常用来高效的存储有限位数的变量，多用于内核/底层开发中。一般来说，对同一个结构体内的不同bit成员的多线程访问是无法保证线程安全的。
- 0bit
	CPU对内存操作的最小单位
	连续的多个非0bit的bit fields是属于同一个memory location的；长度为0bit的bit field会把占单独的一个memory location。对同一个memory location的读写不是线程安全的；对不同memory location的读写是线程安全的。

