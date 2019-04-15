- [C++类四个默认函数---构造函数、析构函数、拷贝函数、赋值函数](#C++类四个默认函数---构造函数、析构函数、拷贝函数、赋值函数)
    - [注意事项](#注意事项)
- [虚函数和虚基类](#虚函数和虚基类)
# C++类四个默认函数---构造函数、析构函数、拷贝函数、赋值函数
每个类只有一个析构函数和一个赋值函数，但可以有多个构造函数（包含一个拷贝构造函数，其它的称为普通构造函数）。
对于任意一个类A，如果不想编写上述函数，C++编译器将自动为A 产生四个缺省的函数，例如：
```C++
A(void); // 缺省的无参数构造函数

A(const A &a); // 缺省的拷贝构造函数

~A(void); // 缺省的析构函数

A & operate =(const A &a); // 缺省的赋值函数
```
## 注意事项
缺省的拷贝构造函数”和“缺省的赋值函数”均采用“位拷贝”而非“值拷贝”的方式来实现，倘若类中含有指针变量，这两个函数注定将出错。

## 构造函数
构造函数是一种特殊的成员函数，无返回值，函数名与类同名。它提供了对成员变量进行初始化的方法，使得在声明对象时能自动地初始化对象。
因为当程序创建一个对象时，系统会自动调用该对象所属类的构造函数。
1. 一旦类中有了一个带参数的构造函数而又没无参数构造函数的时候系统将无法创建不带参数的对象，此时以下三种声明都是错误的：
```C++
Student stu;
Student *stu = new Student;
Student *stu = new Student();
```
1.1 构造函数初始化列表
初始化表位于函数参数表之后，却在函数体 {} 之前。**这说明该表里的初始化工作发生在函数体内的任何代码被执行之前**。
1.1.1  如果类存在继承关系，派生类必须在其初始化表里调用基类的构造函数。例如：
```C++
class A

{   …

    A(int x); // A 的构造函数

};

class B : public A

{   …

    B(int x, int y);// B 的构造函数

};

B::B(int x, int y): A(x) // 在初始化表里调用A 的构造函数

{   …

}
```
1.1.2 类的 const 常量只能在初始化表里被初始化，因为它不能在函数体内用赋值的方式来初始化。
**注：不要在类定义.h文件里声明const常量时直接初始化，会有警告，只有在c++11中才有效**
如：
```C++
//A.h

class A
{
    const int n =1;               
}
```
```
B::B(const A &a) : m_a(a)
{  
    …
}

示例9-2(b) 成员对象在函数体内被初始化：

B::B(const A &a)
{      
    m_a = a;
    … 
}
```
a 初始化列表: 调用成员函数的拷贝构造
b 赋值： 先调用默认构造函数，在调用成员对象的赋值构造。（多进行一次赋值构造调用）

1.2  拷贝函数和赋值函数

1.2.1  这些缺省的拷贝构造函数和赋值函数都是采用“值”操作的方式（深复制和浅复制的问题）

1.2.2  拷贝构造函数是在对象被创建时调用的，而赋值函数只能被已经存在了的对象调用
## 拷贝函数
拷贝构造函数使用传入对象的值生成一个新的对象的实例。拷贝构造函数是在对象被创建时调用的。  
**如果产生了新的对象实例，那调用的就是拷贝构造函数。**
```C++
class A{
    A(const A &a);//拷贝构造函数
};
```
调用拷贝构造函数主要有以下场景：
- 对象作为函数的参数，以值传递的方式传给函数。　
- 对象作为函数的返回值，以值的方式从函数返回
- 使用一个对象给另一个对象初始化
	
## 赋值函数
**如果没有产生新的对象实例，那就是对已有的对象赋值，调用的是赋值运算符**
```C++
class Person{
	Person& operator=(const Person& p);
};
```
## 拷贝函数和赋值函数demo
- 参考链接  
![https://www.cnblogs.com/wangguchangqing/p/6141743.html](https://www.cnblogs.com/wangguchangqing/p/6141743.html)
```C++
class Person
{
public:
    Person(){}
    Person(const Person& p)
    {
        cout << "Copy Constructor" << endl;
    }

    Person& operator=(const Person& p)
    {
        cout << "Assign" << endl;
        return *this;
    }

private:
    int age;
    string name;
};

void f(Person p)
{
    return;
}

Person f1()
{
    Person p;
    return p;
}

int main()
{
    Person p;
    Person p1 = p;    // 拷贝构造
    Person p2;
    p2 = p;           // 赋值构造
    f(p2);            // 拷贝构造

    p2 = f1();        // 先拷贝构造生成临时变量。再使用赋值函数

    Person p3 = f1(); // 拷贝构造(RVO优化)

    getchar();
    return 0;
}
```
# 虚函数和虚基类
## 原理
- `虚函数`
    1. 虚函数
  ```C++
  class test{
    virtual string toString(){}
  };
  ```
    2. 纯虚函数
  ```C++
   class test{
    virtual string toString() = 0;
  };
  ```
2.1. 注意
如果一个类中含有纯虚函数，那么任何试图对该类进行实例化的语句都将导致错误的产生，因为抽象基类(ABC)是不能被直接调用的。必须被子类继承重载以后，根据要求调用其子类的方法。

# 友元
友元可以是一个函数，该函数被称为友元函数，函数既可以是全局也可以是类的成员；友元也可以是一个类，该类被称为友元类。  
是让其它不属于本类的成员(全局函数，其它类的成员函数)，成为类的成员而具备了本类成员的属性。
## 友元函数
```C++
void test();
class B{
    void test();
};
class A{
    friend void test();
    friend void B::test();
};
```
## 友元类
```C++
class test;
class A{
    friend class test;
};
```

## 注意事项
(1)友元关系不能被继承。
(2)友元关系是单向的，不具有交换性。若类B是类A的友元，类A不一定是类B的友元，要看在类中是否有相应的声明。
(3)友元关系不具有传递性。若类B是类A的友元，类C是B的友元，类C不一定是类A的友元，同样要看类中是否有相应的申明

# 模板
1. 类模板和函数模板都可以被全局特化；
2. 类模板能局部特化，不能被重载；
3. 函数模板能被重载，不能被局部特化。
## 模板函数
```C++
template <typename T>　　//定义一个抽象的数据类型T
T myMax(T a, T b) {　　　　//声明函数模板
　　return a > b ? a : b; 
}
```
### 特化
## 模板类
```C++
template<typename T>
class S
{
    public:
        void info()
        {    
            std::cout << "generic (S<T>::info() \n)";
        }
};
```
### 特化
```C++
template<>
class S<void>
{
    public:
        void msg()
        {
            std::cout << "fully specialized (S<void>::msg()) \n";
        }
};
template<typename T, typename U = typename Types<T>::I>
class S;       // (1)

```
## 模板一些技巧
```

```