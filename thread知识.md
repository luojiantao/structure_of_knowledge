# 基础知识
- 线程是执行指令的单元

## C++ 线程知识
### 使用
```C++
#include <iostream>
#include <thread>

using namespace std;

void output(int i)
{
    cout << i << endl;
}

int main()
{
    
    for (uint8_t i = 0; i < 4; i++)
    {
        thread t(output, i);
        t.detach(); 
    }
        
    getchar();
    return 0;
}
```
#### 注意事项
1. **异常情况下等待线程完成**
当决定以detach方式让线程在后台运行时，可以在创建thread的实例后立即调用detach，这样线程就会后thread的实例分离，即使出现了异常thread的实例被销毁，仍然能保证线程在后台运行。但线程以join方式运行时，需要在主线程的合适位置调用join方法，如果调用join前出现了异常，thread被销毁，线程就会被异常所终结。为了避免异常将线程终结，或者由于某些原因，例如线程访问了局部变量，就要保证线程一定要在函数退出前完成，就要保证要在函数退出前调用join
2. **转移线程的所有权**
thread是可移动的(movable)的，但不可复制(copyable)。可以通过move来改变线程的所有权，灵活的决定线程在什么时候join或者detach
```C++
thread t1(f1);
thread t3(move(t1));
```
将线程从t1转移给t3,这时候t1就不再拥有线程的所有权，调用t1.join或t1.detach会出现异常，要使用t3来管理线程。这也就意味着thread可以作为函数的返回类型，或者作为参数传递给函数，能够更为方便的管理线程。

3. **线程的标识类型为std::thread::id**，有两种方式获得到线程的id。

- 通过thread的实例调用get_id()直接获取

- 在当前线程上调用this_thread::get_id()获取

## 异步使用c++11
### std::future
- 定义
std::future是一个类模板(class template)，其对象存储未来的值
- 原理
事实上，一个std::future对象在内部存储一个将来会被赋值的值，并提供了一个访问该值的机制，通过get()成员函数实现。但如果有人试图在get()函数可用之前通过它来访问相关的值，那么get()函数将会阻塞，直到该值可用。

### std::promise
- 定义
  也是一个类模板，其对象有可能在将来对值进行赋值

- 原理
  每个std::promise对象有一个对应的std::future对象，一旦由std::promise对象设置，std::future将会对其赋值。并且**std::promise对象与其管理的std::future对象共享数据**
### demo
```C++
#include <future>       // std::packaged_task, std::future
std::promise<int> promiseObj;
/*目前为止，该promise对象没有任何管理的值，但它承诺肯定会有人对其进行赋值，一旦被赋值，就可以通过其管理的std::future对象来获取该值。
但是，假设线程1创建了该promise对象并将其传给线程2，那么线程1怎样知道线程2什么时候会对promise对象进行赋值呢？
答案是使用std::future对象
每个std::promise对象都有个对应的std::future对象，其他人可以通过它来获取promise设置的值。
所以，线程1将会创建std::promise对象，然后在将其传递给线程2之前从它那里获取std::future对象
*/
std::future<int> futureObj = promiseObj.get_future();
/*现在，线程1将promiseObj传递给线程2.
那么线程1将会获取到线程2通过std::future的get函数设置在std::promise中的值，*/
int val = futureObj.get();
/*但是如果线程2还没有对该值进行设置，那么这个调用将会阻塞，直到线程2在promise对象中对该值进行设置。*/
promiseObj.set_value(45);
```