# 编程方式
典型的就是C语言，不是说C语言没有办法编写面向对象和函数式。而是，C语言在这些方面没有语法上的支持，需要编写者自己把握，这就对写代码的人提出了很高的要求
## example
对于1+1 等于多少这个问题  
### 面向过程，就是和正常人们思考的过程一样，比如
```C
int func(){
    return 1 + 1;
}
int ret = func();
```
1. 关注解决问题的步骤
2. 面对的问题是具体的，特定的问题
3. 1 + 1，直接执行
- 优点
	直接明了，易于理解,是人类正常的思考方式
- 缺点
	当需要的功能比较复杂的时候或者需要修改的时候，func代码会呈现线性增长，当func由几千行的时候，就难以理解。  
	而且对于一个大的工程，很难由这种方式表现出，各个模块和模块间是如何合作
### 函数式的做法
```C
int add(int a, int b){
    return a + b;//函数式形式: a() + b();
}
int ret = add(1, 1);//在函数式看法来看，1也是一个函数可以被调用，1() 返回 1函数
```
1. 关注解决问题的方法
2. 先将通用的加法抽象成一个函数，
3. 然后输入函数1和函数1作为参数，成为一个函数组合 add 1 1
- 优点
	都是通过函数的组织形式调用。能最大的程度复用代码。  
	因为是惰性执行的，只有需要结果的时候才执行代码。所有可以热更新（这个需要语言支持，静态语言明显做不到，或者是我认识浅薄）
	由于入参是不会被改变的，而且函数执行的结果和环境无关，只和函数和入参有关。所有天然支持并行多线程。（静态语言，满足这个条件，也能现实无锁并行）
- 缺点
	代码高度抽象。往往函数和函数的组合形成的函数，很难用语言去描述它的功能。（可以认为是解决一种具体的问题）
### 面向对象的做法
```C++
class add{
public:
add(int a, int b):m_a(a), m_b(b)
{}
int GetRet(){
    return m_ret;
}
void Dofunc(){
    m_ret = m_a + m_b;
}
private:
    int m_a;
    int m_b;
    int m_ret;
};

add Obj(1,1);
Obj.Dofunc();
int ret = Obj.GetRet();
```
1. 定义一个加法类
2. 创建一个加法对象，数据集为1，1
3. 通过成员函数Dofunc改变对象状态
4. 通过GetRet,获取执行状态（结果）
- 优点
	能够描述复杂场景。描述各个模块间的关系和合作方法
	由许多现由的描述对象简单模式（设计模式）
	
- 缺点
	代码比较复杂。（但是由固定模式，像下棋那样由一些固定开局方式）
	一个对象聚集很多数据，占的空间比较大。当只需要一个简单的功能时候。
	