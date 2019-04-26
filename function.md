# lambda
## c++
```C++
int a;
int b;
auto f0 = [](){}//1
auto f = [a](){};//大小为4
auto f2 = [a, b](){}//8
```
### 内存模型
- 问题： 没有捕获的匿名函数和C调用很相似。

  匿名函数是一个匿名类型的c++对象（类实例）,在栈上被创建。  
  一个没有捕获的匿名函数（[]）能够被转化成一个函数指针（是否适合C的调用约定转化是依赖实现的）,但是这不意味它是一个函数指针。

- 问题：当一个捕获值的匿名函数从一个函数返回，被包装在std::function中。这个时候内存是怎么样的？

  1. 在C++11中一个lambda没有任何特别。它是一个对象和其他任何对象一样。一个匿名表达式结果在一个临时变量中，在栈中它可以被用于初始化一个变量


- std::function 和 lambda对象 使用值语义,copyalbe
```C++
{
  auto lamb = []() {return 5;};
  auto func_lamb = std::function<int()>(lamb);// flag1
}
```
1. flag1 时有两份lamb值，lamb 和 func_lamb
2. 在离开‘}’时，按照清理栈变量规则回收资源
3. 因为std::function 是一个类所以有如下
```c++
//通常使用没有捕获的lamba，构造std::function,那么这个std::function占一个字节大小（空类）
new std::function
```

### 例子解释

```C++

```

## 内存屏障

通过函数封装可以实现简单的内存屏障。（防止编译器优化）

```C++
struct test{
test();
test(int a1, int b1):a(a1),b(b1)
int a;
int b = ;
};
test *g = NULL;
test* g2 = NULL
//由于编译器优化可能在函数,使
//g2 = g,先于g,a=1; g.b = 2; 执行
//多线程环境下，其他线程获取g2,访问属性a,b得不到预期值。
void fun_a(){
    g = new test();
    g.a = 1;
    g.b = 2;
    g2 = g;
}
//-------------------------
//修改如下

void fun_a(){
    //依靠类的初始化机制实现简单的内存屏障
    g = new test(1， 2);
    g2 = g;
}
```
# std::function
通过std::function对C++中各种可调用实体（普通函数、Lambda表达式、函数指针、以及其它函数对象等）的封装，形成一个新的可调用的std::function对象；让我们不再纠结那么多的可调用实体。一切变的简单粗暴。

## 参考链接
[https://zh.cppreference.com/w/cpp/utility/functional/function/function](https://zh.cppreference.com/w/cpp/utility/functional/function/function)

## 理解
- 是一个类对象
- 重载了()操作符
- 64位，机器下固定大小为32 TODO
```C++

```

## std::placeholders 占位符

```C++
namespace placeholders {
  extern /* unspecified */ _1;
  extern /* unspecified */ _2;
  extern /* unspecified */ _3;
  // ...
}
```
其中_1, _2, _3是未指定的数字对象，用于function的bind中。 _1用于代替回调函数中的第一个参数， _2用于代替回调函数中的第二个参数，以此类推.
### 参考链接
https://blog.csdn.net/nanjiye/article/details/52164279
### example
```C++
ExampleFunction f = std::bind(&Object::hello, &instance, std::placeholders::_1);
```
1. std::is_placeholder  
	std::is_placeholder 用于判断T是否为占位符，它有一个成员变量value。如果T是placeholder类型，value的值为1代表 _1，2代表 _2；如果T不是，则value为0
2. std::is_bind_expression   
  判断是否是bind表达式，有value成员，返回值是true或false
## example
```C++
#include <functional>

// 类成员函数
TestClass testObj;
Functional = std::bind(&TestClass::ClassMember, testObj, std::placeholders::_1);
result = Functional(40);
```
- std::function对象最大的用处就是在实现函数回调，使用者需要注意，它不能被用来检查相等或者不相等，但是可以与NULL或者nullptr进行比较。