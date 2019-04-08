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

## 什么是值语义
