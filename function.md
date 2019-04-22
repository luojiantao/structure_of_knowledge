# lambda
## c++
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

