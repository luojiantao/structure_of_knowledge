# 编译器中的优化
## RVO (return value optimization) 和NRVO (named return value optimization)
是C++在处理一个函数返回类对象并将返回值赋给另一个对象时，为了减少拷贝构造次数以及析构次数而采用的一种编译器优化技术。
## 测试
```C++
class Point3d
{
public:
    int m_x;
    int m_y;
    int m_z;
public:
    Point3d(int x,int y,int z):m_x(x),m_y(y),m_z(z)
    {
        cout << "constructor"<<endl;
    }
    ~Point3d()
    {
        cout << "deconstructor"<<endl;
    }
    Point3d(const Ponint3d &other)
    {
        this.m_x = other.m_x;
        this.m_y = other.m_y;
        this.m_z = other.m_z;
        cout << "copy constructor"<<endl;
    }
    Point3d &operator=(const Point3d &other)
    {
        if(this != &other)
        {
            this.m_x = other.m_x;
            this.m_y = other.m_y;
            this.m_z = other.m_z;
        }
        cout << "operator="<<endl;
        return *this;
    }
}

Point3d factory()
{
    Point3d po(1,2,3)
    return po;
}

int main()
{
    Point3d p = factory();
    return 1;
}

```
1. 不做任何返回值优化。在g++中有一个选项-fno-elide-constructors可以去掉任何返回值优化.执行情况如下：
```C++
Point3d factory()
{
    Point3d po(1,2,3)//constructor      //factory函数中构造po对象
    return po;       //copy constructor //factory函数中用po对象,拷贝构造临时对象_temp
}                   //deconstructor    //factory函数中返回时，析构掉局部对象po

int main()
{
    Point3d p = factory(); //copy constructor //main函数中用factory函数中拷贝构造的临时对象,拷贝构造对象p
                          //deconstructor    //析构临时对象_temp
    return 1;
}                        //deconstructor    //main函数结束时，析构对象p
```
2. 使用RVO优化，VS在debug模式下使用RVO而不用NRVO
```C++
Point3d factory()
{
    Point3d po(1,2,3)     //constructor 
    return po;
}

int main()
{
    Point3d p = factory();//copy constructor
                          //deconstructor  po
    return 1;
}                        //deconstructor       //main函数中p的析构
```
- 在《在深度探索C++对象模型》书中，对应的解释为,编译器优化的伪代码如下：
```C++
//这样将p直接做为参数，在factory()函数中进行构造，就不需要先将po拷贝构造出临时对象_temp，再析构_temp。
factory(const Point3d &_result)
{
    Point3d po;
    po.Point3d::Point3d(1,2,3);
    _result.Point3d::Point3d(po);   //用po拷贝构造_result;
    po.Point3d::~Point3d();         //po对象析构
    return;
}
int main()
{
  Point3d p;
  factory(p);
}
```
3. 进一步优化，能不能直接构造p，而不需要先构造po，再拷贝构造p呢。答案是可以的，需要将factory()函数改写为如下形式:
```C++
factor()
{
    return Point3d(1,2,3);
}
int main(){
    Point3d p = factory();
    return 1;
}
//-----------------------------优化后
factory(const Point3d &_result)
{
    _result.Point3d::Point3d(1,2,3);  //直接将p作为参数构造
    return;
}

int main(){
    Point3d p； //声明伪代码，只是声明不会调用构造
    factory(p);
    return 1;
}
```
# RAII ”资源分配时初始化”

# 值语义
## 参考连接
    [res](http://www.cnblogs.com/Solstice/archive/2011/08/16/2141515.html)
## 术语 
### POD
POD全称Plain Old Data。通俗的讲，一个类或结构体通过二进制拷贝后还能保持其数据不变，那么它就是一个POD类型

## 什么是值语义
值语义(value sematics)指的是对象的拷贝与原对象无关，就像拷贝 int 一样。C++ 的内置类型(bool/int/double/char)都是值语义，标准库里的 complex<> 、pair<>、vector<>、map<>、string 等等类型也都是值语意，拷贝之后就与原对象脱离关系。Java 语言的 primitive types 也是值语义。

与值语义对应的是“对象语义/object sematics”，或者叫做引用语义(reference sematics)，由于“引用”一词在 C++ 里有特殊含义，所以我在本文中使用“对象语义”这个术语。对象语义指的是面向对象意义下的对象，对象拷贝是禁止的。例如 muduo 里的 Thread 是对象语义，拷贝 Thread 是无意义的，也是被禁止的：因为 Thread 代表线程，拷贝一个 Thread 对象并不能让系统增加一个一模一样的线程。

同样的道理，拷贝一个 Employee 对象是没有意义的，一个雇员不会变成两个雇员，他也不会领两份薪水。拷贝 TcpConnection 对象也没有意义，系统里边只有一个 TCP 连接，拷贝 TcpConnection  对象不会让我们拥有两个连接。Printer 也是不能拷贝的，系统只连接了一个打印机，拷贝 Printer 并不能凭空增加打印机。凡此总总，**面向对象意义下的“对象”是 non-copyable**。

值语义的对象不一定是 POD，例如 string 就不是 POD，但它是值语义的。

值语义的对象不一定小，例如 vector<int> 的元素可多可少，但它始终是值语义的。当然，很多值语义的对象都是小的，例如complex<>、muduo::Date、muduo::Timestamp。
## 值语义和生命周期

值语义的一个巨大好处是生命期管理很简单，就跟 int 一样——你不需要操心 int 的生命期。值语义的对象要么是 stack object，或者直接作为其他 object 的成员，因此我们不用担心它的生命期（一个函数使用自己stack上的对象，一个成员函数使用自己的数据成员对象）。相反，对象语义的 object 由于不能拷贝，我们只能通过指针或引用来使用它

一旦使用指针和引用来操作对象，那么就要担心所指的对象是否已被释放，这一度是 C++ 程序 bug 的一大来源。此外，由于 C++ 只能通过指针或引用来获得多态性，那么在C++里从事基于继承和多态的面向对象编程有其本质的困难——资源管理。

考虑一个简单的对象建模——家长与子女：a Parent has a Child, a Child knows his/her Parent

在 C++ 里边就要为资源管理费一番脑筋：Parent 和 Child 都代表的是真人，肯定是不能拷贝的，因此具有对象语义。Parent 是直接持有 Child 吗？抑或 Parent 和 Child 通过指针互指？Child 的生命期由 Parent 控制吗？如果还有 ParentClub 和 School 两个 class，分别代表家长俱乐部和学校：ParentClub has many Parent(s)，School has many Child(ren)，那么如何保证它们始终持有有效的 Parent 对象和 Child 对象？何时才能安全地释放 Parent 和 Child ？
直接但是易错的写法：
```C++
class Child;

class Parent : boost::noncopyable
{
 private:
  Child* myChild;
};

class Child : boost::noncopyable
{
 private:
  Parent* myParent;
};
```
### 问题
如果直接使用指针作为成员，那么如何确保指针的有效性？如何防止出现空悬指针？Child 和 Parent 由谁负责释放？在释放某个 Parent 对象的时候，如何确保程序中没有指向它的指针？在释放某个 Child 对象的时候，如何确保程序中没有指向它的指针？

### 解决办法

这一系列问题一度是C++面向对象编程头疼的问题，不过现在有了 smart pointer，我们可以借助 smart pointer 把对象语义转换为值语义，从而轻松解决对象生命期。让 Parent 持有 Child 的 smart pointer，同时让 Child 持有 Parent 的 smart pointer，这样始终引用对方的时候就不用担心出现空悬指针。当然，其中一个 smart pointer 应该是 weak reference，否则会出现循环引用，导致内存泄漏。到底哪一个是 weak reference，则取决于具体应用场景。

如果 Parent 拥有 Child，Child 的生命期由其 Parent 控制，Child 的生命期小于 Parent，那么代码就比较简单：

```C++
class Parent;
class Child : boost::noncopyable
{
 public:
  explicit Child(Parent* myParent_)
    : myParent(myParent_)
  {
  }

 private:
  Parent* myParent;
};

class Parent : boost::noncopyable
{
 public:
  Parent()
    : myChild(new Child(this))
  {
  }

 private:
  boost::scoped_ptr<Child> myChild;
};
```
在上面这个设计中，Child 的指针不能泄露给外界，否则仍然有可能出现空悬指针。

如果 Parent 与 Child 的生命期相互独立，就要麻烦一些：

```C++
class Parent;
typedef boost::shared_ptr<Parent> ParentPtr;

class Child : boost::noncopyable
{
 public:
  explicit Child(const ParentPtr& myParent_)
    : myParent(myParent_)
  {
  }

 private:
  boost::weak_ptr<Parent> myParent;
};
typedef boost::shared_ptr<Child> ChildPtr;


class Parent : public boost::enable_shared_from_this<Parent>,
               private boost::noncopyable
{
 public:
  Parent()
  {
  }

  void addChild()
  {
    myChild.reset(new Child(shared_from_this()));
  }

 private:
  ChildPtr myChild;
};

int main()
{
  ParentPtr p(new Parent);
  p->addChild();
}
```

## 值语义与标准库

C++ 要求凡是能放入标准容器的类型必须具有值语义。准确地说：type 必须是 SGIAssignable concept 的 model。但是，由 于C++ 编译器会为 class 默认提供 copy constructor 和 assignment operator，因此除非明确禁止，否则 class 总是可以作为标准库的元素类型——尽管程序可以编译通过，但是隐藏了资源管理方面的 bug。

## 值语义和C++
C++ 的 class 本质上是值语义的，这才会出现 object slicing 这种语言独有的问题，也才会需要程序员注意 pass-by-value 和 pass-by-const-reference 的取舍。在其他面向对象编程语言中，这都不需要费脑筋。

值语义是C++语言的三大约束之一，C++ 的设计初衷是让用户定义的类型(class)能像内置类型(int)一样工作，具有同等的地位。为此C++做了以下设计（妥协）：

- class 的 layout 与 C struct 一样，没有额外的开销。定义一个“只包含一个 int 成员的 class ”的对象开销和定义一个 int 一样
- 甚至 class data member 都默认是 uninitialized，因为函数局部的 int 是 uninitialized。
- class 可以在 stack 上创建，也可以在 heap 上创建。因为 int 可以是 stack variable。
- class 的数组就是一个个 class 对象挨着，没有额外的 indirection。因为 int 数组就是这样
- 编译器会为 class 默认生成 copy constructor 和 assignment operator。其他语言没有 copy constructor 一说，也不允许重载 assignment operator。C++ 的对象默认是可以拷贝的，这是一个尴尬的特性。
- 当 class type 传入函数时，默认是 make a copy （除非参数声明为 reference）。因为把 int 传入函数时是 make a copy
- 当函数返回一个 class type 时，只能通过 make a copy（C++ 不得不定义 RVO 来解决性能问题）。因为函数返回 int 时是 make a copy。
- 以 class type 为成员时，数据成员是嵌入的。例如 pair<complex<double>, size_t> 的 layout 就是 complex<double> 挨着 size_t
    
 这些设计带来了性能上的好处，原因是 memory locality


