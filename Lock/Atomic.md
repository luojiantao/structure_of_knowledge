# 原子操作类 Atomic


## 原子操作类 Atomic

### 参考链接
[https://www.cnblogs.com/haippy/archive/2013/09/05/3301408.html](https://www.cnblogs.com/haippy/archive/2013/09/05/3301408.html)
[https://www.jianshu.com/p/c80de7978f8f](https://www.jianshu.com/p/c80de7978f8f)(atomic<T> 类型说明)
[https://zh.cppreference.com/w/cpp/atomic/memory_order](https://zh.cppreference.com/w/cpp/atomic/memory_order)

### 简单说明
它们提供的方法能保证具有原子性。这些方法是不可再分的，获取这些变量的值时，永远获得修改前的值或修改后的值，不会获得修改过程中的中间数值。

1. atomic_flag
  其实和atomic<bool>相似），它只有test_and_set()和clear()方法。其中，test_and_set会检查变量的值是否为false，如果为false则把值改为true。
2. atomic<T>
  - atomic<T>提供了常见且容易理解的方法
    1. store
      原子写操作

    2. load

        **T load (memory_order sync = memory_order_seq_cst) const volatile noexcept;
        T load (memory_order sync = memory_order_seq_cst) const noexcept;**
        原子读操作

        |Memory Order 值|Memory Order 类型|描述|
        |------|-----|-----|
        |memory_order_relaxed| Relaxed|https://zh.cppreference.com/w/cpp/atomic/memory_order|
        |memory_order_consume| Consume |11|
        |memory_order_acquire| Acquire|11|
        |memory_order_seq_cst| Sequentially consistent|11|
    3. exchange
      	允许2个数值进行交换，并保证整个过程是原子的
       |Memory Order 值|Memory Order 类型|描述|
       |------|-----|-----|
       |memory_order_relaxed| Relaxed|11|
       |memory_order_consume| Consume |11|
       |memory_order_acquire| Acquire|11|
       |memory_order_seq_cst| Sequentially consistent|11|
       |memory_order_release| release|11|
       |memory_order_acq_rel| Acquire/Release|11|
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
  - 流程说明,任务出队
    ![avatar](../Lock_png/CAS说明.png)
    1. S1 初始状态
    2. S2 抢占状态，抢占处理
    3. S3 被抢占状态
    4. S4 被抢占后处理

```flow
st=>start: 开始框

op0=>operation: S1
op1=>operation: S2
op2=>operation: S3
op3=>operation: S4

cond=>condition: 被其他线程处理(是或否?)

sub1=>subroutine: 子流程

io=>inputoutput: 输入输出框

e=>end: 结束框



st->op0->cond
cond(no)->op1->e
cond(yes)->op2->op3->op0
```

## 锁和CAS
- 锁
	1. 在多线程竞争下，加锁、释放锁会导致比较多的上下文切换和调度延时，引起性能问题。而且在上下文切换的时候，cpu之前缓存的指令和数据都将失效，对性能有很大的损失。用户态的锁虽然避免了这些问题，但是其实它们只是在没有真实的竞争时才有效。
	2. 一个线程持有锁会导致其它所有需要此锁的线程挂起直至该锁释放。
	3. 如果一个优先级高的线程等待一个优先级低的线程释放锁会导致导致优先级反转(Priority Inversion)，引起性能风险
- CAS
	1. 一个线程的失败或者挂起不应该影响其他线程的失败或挂起的算法。现代的CPU提供了特殊的指令，可以自动更新共享数据，而且能够检测到其他线程的干扰，而 compareAndSet() 就用这些代替了锁定
	2. ABA问题。比如说一个线程one从内存位置V中取出A，这时候另一个线程two也从内存中取出A，并且two进行了一些操作变成了B，然后two又将V位置的数据变成A，这时候线程one进行CAS操作发现内存中仍然是A，然后one操作成功。尽管线程one的CAS操作成功，但是不代表这个过程就是没有问题的。如果链表的头在变化了两次后恢复了原值，但是不代表链表就没有变化