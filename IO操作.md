# 文件读写



## c++
### stream
ofstream是从内存到硬盘，ifstream是从硬盘到内存，其实所谓的流缓冲就是内存空间
1. 插入器(<<)

    向流输出数据。比如说系统有一个默认的标准输出流(cout)，一般情况下就是指的显示器，所以，cout<<"Write Stdout"<<'\n';就表示把字符串"Write Stdout"和换行字符('\n')输出到标准输出流。
2. 析取器(>>)

    从流中输入数据。比如说系统有一个默认的标准输入流(cin)，一般情况下就是指的键盘，所以，cin>>x;就表示从标准输入流中读取一个指定类型的数据。
### fstream
在C++中，对文件的操作是通过stream的子类fstream(file stream)来实现的，所以，要用这种方式操作文件，就必须加入头文件fstream.h。

以输入/输出方式来打开文件。
#### 成员函数 open.

void open(const char* filename,int mode,int access);
1. 参数说明

    filename：　　要打开的文件名
    

　　mode：　　　　要打开文件的方式

　　access：　　　打开文件的属性
  - mode
      ```
      ios::app：　　　以追加的方式打开文件
      ios::ate：　　　文件打开后定位到文件尾，ios:app就包含有此属性
      ios::binary：　以二进制方式打开文件，缺省的方式是文本方式。两种方式的区别见前文
      ios::in：　　　 文件以输入方式打开(文件数据输入到内存)
      ios::out：　　　文件以输出方式打开(内存数据输出到文件)
      ios::nocreate： 不建立文件，所以文件不存在时打开失败
      ios::noreplace：不覆盖文件，所以打开文件时如果文件存在失败
      ios::trunc：　　如果文件存在，把文件长度设为0
      ```
      可以用“或”把以上属性连接起来，如ios::out|ios::binary
   - access
      ```
      0：普通文件，打开访问
      1：只读文件
      2：隐含文件
      4：系统文件
      ```
      可以用“或”或者“+”把以上属性连接起来，如3或1|2就是以只读和隐含属性打开文件。
2. demo
```C++
fstream file1;
file1.open("c:\\config.sys",ios::binary|ios::in,0);
```
#### 成员函数 close()
打开的文件使用完成后一定要关闭，fstream提供了成员函数close()来完成此操作
```C++
file1.close()
```
#### 成员函数eof()
用来检测是否到达文件尾，如果到达文件尾返回非0值，否则返回0。原型是int eof();
```C++
if(in.eof()) ShowMessage("已经到达文件尾!");
```
#### 子类 ifstream
默认以输入方式打开文件。从其他对象，输入到自己。(读)
#### 子类 ofstream
默认以输出方式打开文件。从自己输出到其他对象。(写)
#### 文件定位
和C的文件操作方式不同的是，C++ I/O系统管理两个与一个文件相联系的指针。
1. 读指针

	它说明输入操作在文件中的位置。
	成员函数seekg()是设置读位置。
	istream &seekg(streamoff offset,seek_dir origin);
2. 写指针

	它下次写操作的位置。
	成员函数seekp是设置写位置。
	ostream &seekp(streamoff offset,seek_dir origin);
streamoff定义于 iostream.h 中，定义有偏移量 offset 所能取得的最大值，seek_dir 表示移动的基准位置，是一个有以下值的枚举：
```
　　ios::beg：　　文件开头
　　ios::cur：　　文件当前位置
　　ios::end：　　文件结尾
　　这两个函数一般用于二进制文件，因为文本文件会因为系统对字符的解释而可能与预想的值不同。例：

　　file1.seekg(1234,ios::cur); //把文件的读指针从当前位置向后移1234个字节
　　file2.seekp(1234,ios::beg); //把文件的写指针从文件开头向后移1234个字节
```

## 读文件
### c++
1. 二进制文件读取

	对于二进制的读取就要复杂些。
	1. put：写入一个字符
	2. get：读取一个字符
	3. read：读取数据块　　read(unsigned char *buf,int num);

	4. write：写入数据块	  write(const unsigned char *buf,int num)
2. 文本文件读取

	对于文本文件的读取比较简单，用插入器和析取器就可以了;
#### demo
1. 逐行读入
```C++
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

void readTxt(string file)
{
    ifstream infile; 
    infile.open(file.data());   //将文件流对象与文件连接起来 
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

    string s;
    while(getline(infile,s))
    {
        cout<<s<<endl;
    }
    infile.close();             //关闭文件输入流 
}
```
2. 逐个字符读入（忽略空格与回车

```C++ 
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
void readTxt(string file)
{
    ifstream infile; 
    infile.open(file.data());   //将文件流对象与文件连接起来 
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

    char c;
    while (!infile.eof())
    {
        infile >> c;
        cout<<c<<endl;

    }
    infile.close();             //关闭文件输入流 
}

```
3. 逐个字符读入（包括空格与回车）
```C++
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
void readTxt(string file)
{
    ifstream infile; 
    infile.open(file.data());   //将文件流对象与文件连接起来 
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

    char c;
    infile >> noskipws;//设置cin读取空白符, 
    //cin>>skipws;//将cin恢复到默认状态，从而丢弃空白符
    while (!infile.eof())
    {
        infile>>c;
        cout<<c<<endl;

    }
    infile.close();             //关闭文件输入流 
}


char ch;

ifstream infile("data.txt");

while(infile)

{

infile.get(ch);

cout<<ch;

}

getch();

}
//-------------------------------二进制文件读取
outfile.close();

ifstream inifile("data.dat",ios:binary);

for(i=0;i<100;i++)

{ inifile.read((Char*)n,sizeof(int);

cout<<n[0]<<" ";

}
```
### python 

## 写文件

### c++
#### demo
```C++
char str[]="hello this is a c++ programe!\n"//单个字符写文件

"i know this difference \n"

"and mocive";

ofstream outfile ("data.txt");

for(int i=0;i<strlen(Str);i++)

outfile.put(str[i]);

//------------------------------------- 使用插入器 << 写文件
ofstream outfile("strdata.txt");

outfile<<"use your vote\n"；

outfile<<"ouse your weise\n";

//-----------------------------------二进制文件写入
ofstream outfile("data.dat",ios::binary);

for(int i=0;i<100;i++)

{

n[0]=i+1;

outfile.write((Char*)n,sizeof(int)); //还可以用变量和数组为例

// outfile.write((Char*)&i,sizeof(int)); i 为变量

// outfile.write((Char*)n,sizeof(int)); n为数组

}

outfile.close();
```
### python

# socket。网络IO

## c++

## python 
