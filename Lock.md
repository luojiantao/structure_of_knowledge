# C++11
## 互斥锁
C++11中新增了<mutex>，它是C++标准程序库中的一个头文件，定义了C++11标准中的一些互斥访问的类与方法等。其中std::mutex就是lock、unlock。std::lock_guard与std::mutex配合使用，把锁放到lock_guard中时，mutex自动上锁，lock_guard析构时，同时把mutex解锁。mutex又称互斥量
1. std::mutex类
  - 该类表示普通的互斥锁, 不能递归使用
    ```C++
    #include <mutex>          // std::mutex
    std::mutex mtx;
    ```
2. std::timed_mutex
  - 该类表示定时互斥锁，不能递归使用。std::time_mutex比std::mutex多了两个成员函数
    1. try_lock_for()
    	函数参数表示一个时间范围，在这一段时间范围之内线程如果没有获得锁则保持阻塞；如果在此期间其他线程释放了锁，则该线程可获得该互斥锁；**如果超时(指定时间范围内没有获得锁)，则函数调用返回false**。

    2. try_lock_until()
    	函数参数表示一个时刻，在这一时刻之前线程如果没有获得锁则保持阻塞；如果在此时刻前其他线程释放了锁，则该线程可获得该互斥锁；**如果超过指定时刻没有获得锁，则函数调用返回false**

3. std::recursive_mutex
	- 该类表示递归互斥锁
		递归互斥锁可以被同一个线程多次加锁，以获得对互斥锁对象的多层所有权。例如，同一个线程多个函数访问临界区时都可以各自加锁，执行后各自解锁。**std::recursive_mutex释放互斥量时需要调用与该锁层次深度相同次数的unlock()，即lock()次数和unlock()次数相同**。可见，线程申请递归互斥锁时，**如果该递归互斥锁已经被当前调用线程锁住，则不会产生死锁**。此外，std::recursive_mutex的功能与 std::mutex大致相同。

4. std::recursive_timed_mutex

  - 带定时的递归互斥锁

互斥类的最重要成员函数是lock()和unlock()。在进入临界区时，执行lock()加锁操作，如果这时已经被其它线程锁住，则当前线程在此排队等待。退出临界区时，执行unlock()解锁操作。
​	
用于互斥锁的RAII的类模板：更好的办法是采用”资源分配时初始化”(RAII)方法来加锁、解锁，这避免了在临界区中因为抛出异常或return等操作导致没有解锁就退出的问题。极大地简化了程序员编写Mutex相关的异常处理代码。C++11的标准库中提供了std::lock_guard类模板做mutex的RAII
```C++
#include <mutex>

std::mutex mut;
 
{
       std::lock_guard<std::mutex> lk(mut);
}
 
{
       std::unqiue_lock<std::mutex> lk(mut);
}
```
std::unique_lock 与std::lock_guard都能实现自动加锁与解锁功能，但是std::unique_lock要比std::lock_guard更灵活，但是更灵活的代价是占用空间相对更大一点且相对更慢一点。

## 原子操作类 Atomic

它们提供的方法能保证具有原子性。这些方法是不可再分的，获取这些变量的值时，永远获得修改前的值或修改后的值，不会获得修改过程中的中间数值。

1. atomic_flag
  其实和atomic<bool>相似），它只有test_and_set()和clear()方法。其中，test_and_set会检查变量的值是否为false，如果为false则把值改为true。
2. atomic<T>
  - atomic<T>提供了常见且容易理解的方法
    1. store
      原子写操作

    2. load
      原子读操作

    3. exchange
      允许2个数值进行交换，并保证整个过程是原子的

    4. compare_exchange_weak
      CAS（compare and set）。参数会要求在这里传入期待的数值和新的数值。它们对比变量的值和期待的值是否一致，如果是，则替换为用户指定的一个新的数值。如果不是，则将变量的值和期待的值交换。

    5. compare_exchange_strong
		weak版本的CAS允许偶然出乎意料的返回（比如在字段值和期待值一样的时候却返回了false），不过在一些循环算法中，这是可以接受的。通常它比起strong有更高的性能。
	- CAS的语义是“我认为V的值应该为A，如果是，那么将V的值更新为B，否则不修改并告诉V的值实际为多少”
	```C++
	//DEMO
	#include <atomic>
	//------------------------------------------------------------
	std::atomic<int> aa(10);
	int aa1 = 10;
	aa.compare_exchange_weak(aa1, 11);//执行结果，aa:11, aa1:10,语句返回true
	//------------------------------------------------------------
	
	std::atomic<int> aa(10);
	int aa1 = 12;
	aa.compare_exchange_weak(aa1, 11);//执行结果，aa:10, aa1:10,语句返回false
	
	```
	- 实现无锁对队列的例子
	```C++
	#include <iostream>       // std::cout
	#include <atomic>         // std::atomic
	#include <thread>         // std::thread
	#include <vector>         // std::vector

	// a simple global linked list:
	struct Node { int value; Node* next; };
	std::atomic<Node*> list_head (nullptr);

	void append (int val) {     // append an element to the list
	  Node* oldHead = list_head;
	  Node* newNode = new Node {val,oldHead};

	  // what follows is equivalent to: list_head = newNode, but in a thread-safe way:
	  while (!list_head.compare_exchange_weak(oldHead,newNode)) {
	    newNode->next = oldHead;
	  }
	}
        void pop(int& val){
	    Node* oldtail = list_tail;
	    Node* newtail = odltail->front();
	    
	    while(!list_tail.compare_exchange_weak(oldtail,newtail)){
	        newtail = oldtail->front();
	    }
	    list_tail->next = NULL;
	    val = oldtail->val;
	}
	```
## 锁和CAS
- 锁
	1. 在多线程竞争下，加锁、释放锁会导致比较多的上下文切换和调度延时，引起性能问题。而且在上下文切换的时候，cpu之前缓存的指令和数据都将失效，对性能有很大的损失。用户态的锁虽然避免了这些问题，但是其实它们只是在没有真实的竞争时才有效。
	2. 一个线程持有锁会导致其它所有需要此锁的线程挂起直至该锁释放。
	3. 如果一个优先级高的线程等待一个优先级低的线程释放锁会导致导致优先级反转(Priority Inversion)，引起性能风险
- CAS
	1. 一个线程的失败或者挂起不应该影响其他线程的失败或挂起的算法。现代的CPU提供了特殊的指令，可以自动更新共享数据，而且能够检测到其他线程的干扰，而 compareAndSet() 就用这些代替了锁定
	2. ABA问题。比如说一个线程one从内存位置V中取出A，这时候另一个线程two也从内存中取出A，并且two进行了一些操作变成了B，然后two又将V位置的数据变成A，这时候线程one进行CAS操作发现内存中仍然是A，然后one操作成功。尽管线程one的CAS操作成功，但是不代表这个过程就是没有问题的。如果链表的头在变化了两次后恢复了原值，但是不代表链表就没有变化

