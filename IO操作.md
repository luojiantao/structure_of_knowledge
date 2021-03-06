# 文件读写
## 二进制文件
二进制读写是将内存里面的数据直接读写入文本中

二进制文件可以存储char/int/short/long/float/……各种变量值。

而二进制文件elf和bmp等，都往往有一个head，告诉你文件信息和解释方式

二进制文件是把内存中的数据按其在内存中的存储形式原样输出到磁盘上存放，也就是说存放的是数据的原形式
## 文本文件
从存储方式来说，文件在磁盘上的存储方式都是二进制形式，所以，文本文件其实也应该算二进制文件。

实际上也是一种特殊的二进制文件。所以，我们很难区分二者的不同，因为他们的概念上不是完全互斥的。我们说文本文件是特殊的二进制文件，是因为文本文件实际上的解释格式已经确定了：ASCII或者unicode编码。文本文件的一个缺点是，它的熵往往较低，也就是说，其实本可以用更小的存储空间记录这些信息。比如，文本文件中的一个数字65536，需要用5个字节来存储；但是用二进制格式，采用int存储，仅仅需要2个字节

文本文件只能存储char型字符变量。
文本呢，则是内存数据将数据先转换成了字符串，再写入到文本中。
以读文件为例：
实际上是磁盘 》》 文件缓冲区》》应用程序内存空间这两个转化过程。

文本文件是把数据的终端形式的二进制数据输出到磁盘上存放，也就是说存放的是数据的终端形式。

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
## python 
关于open()的mode参数：

'r'：读

'w'：写

'a'：追加

'r+' == r+w（可读可写，文件若不存在就报错(IOError)）

'w+' == w+r（可读可写，文件若不存在就创建）

'a+' ==a+r（可追加可写，文件若不存在就创建）

对应的，如果是二进制文件，就都加一个b就好啦：

'rb'　　'wb'　　'ab'　　'rb+'　　'wb+'　　'ab+

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
 - read()
 	
	 每次读取整个文件，它通常用于将文件内容放到一个字符串变量中。如果文件大于可用内存，为了保险起见，可以反复调用read(size)方法，每次最多读取size个字节的内容。
 - readline() 
 
 	每次只读取一行，通常比readlines() 慢得多。仅当没有足够内存可以一次读取整个文件时，才应该使用 readline()。
 - readlines()
 
 	之间的差异是后者一次读取整个文件，象 .read() 一样。.readlines() 自动将文件内容分析成一个行的列表，该列表可以由 Python 的 for ... in ... 结构进行处理。
```python
try:
    f = open('/path/to/file', 'r')
    print(f.read())
finally:
    if f:
        f.close()
//-------------------------
with open('/path/to/file', 'r') as f:
    print(f.read())
```
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
```python
>>> f = open('test.txt', 'w') # 若是'wb'就表示写二进制文件
>>> f.write('Hello, world!')
>>> f.close()


with open('test.txt', 'w') as f:
    f.write('Hello, world!')
    
f1 = open('test1.txt', 'w')
f1.writelines(["1", "2", "3"])
#    此时test1.txt的内容为:123

f1 = open('test1.txt', 'w')
f1.writelines(["1\n", "2\n", "3\n"])
#    此时test1.txt的内容为:
#    1
#    2        
#    3
```
# socket。网络IO

## c++
参考连接[https://www.cnblogs.com/kefeiGame/p/7246942.html](https://www.cnblogs.com/kefeiGame/p/7246942.html)
```C++
#include <string.h>

#include <sys/socket.h>

#include <sys/types.h>

#define MYPORT 3490 /*用户接入端口*/

#define BACKLOG 10 /* 多少等待连接控制*/

main()

{

　　int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd */

　　struct sockaddr_in my_addr; /* 地址信息 */

　　struct sockaddr_in their_addr; /* connector's address information */

　　int sin_size;

　　sockfd = socket(AF_INET, SOCK_STREAM, 0); /* 错误检查*/

　　my_addr.sin_family = AF_INET; /* host byte order */

　　my_addr.sin_port = htons(MYPORT); /* short, network byte order */

　　my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */

　　bzero(&(my_addr.sin_zero),; /* zero the rest of the struct */

　　/* don't forget your error checking for these calls: */

　　bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));

　　listen(sockfd, BACKLOG);

　　sin_size = sizeof(struct sockaddr_in);

　　new_fd = accept(sockfd, &their_addr, &sin_size);
  ....
```
### send 和 recv
这两个函数用于流式套接字或者数据报套接字的通讯。(SOCK_STREAM/SOCK_DGRAM)
```C++
char *msg = "Beej was here!";

int len, bytes_sent;

……

len = strlen(msg);

bytes_sent = send(sockfd, msg, len, 0);
```
send() 返回实际发送的数据的字节数--它可能小于你要求发送的数 目！ 注意，有时候你告诉它要发送一堆数据可是它不能处理成功。它只是 发送它可能发送的数据，然后希望你能够发送其它的数据。记住，如果 send() 返回的数据和 len 不匹配，你就应该发送其它的数据。但是这里也 有个好消息：如果你要发送的包很小(小于大约 1K)，它可能处理让数据一 次发送完。最后要说得就是，它在错误的时候返回-1，并设置 errno

int recv(int sockfd, void *buf, int len, unsigned int flags);

sockfd 是要读的套接字描述符。buf 是要读的信息的缓冲。len 是缓 冲的最大长度。flags 可以设置为0。(请参考recv() 的 man page。) recv() 返回实际读入缓冲的数据的字节数。或者在错误的时候返回-1， 同时设置 errno。

### sendto() 和 recvfrom()函数
 to 是个指向数据结构 struct sockaddr 的指针，它包含了目的地的 IP 地址和端口信息。tolen 可以简单地设置为 sizeof(struct sockaddr)。 和函数 send() 类似，sendto() 返回实际发送的字节数(它也可能小于 你想要发送的字节数！)，或者在错误的时候返回 -1。
 ```C++
 //你已经看到了，除了另外的两个信息外，其余的和函数 send() 是一样 的。 to 是个指向数据结构 struct sockaddr 的指针，它包含了目的地的 IP 地址和端口信息。tolen 可以简单地设置为 sizeof(struct sockaddr)。 和函数 send() 类似，sendto() 返回实际发送的字节数(它也可能小于 你想要发送的字节数！)，或者在错误的时候返回 -1。
 int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, int tolen);
 
 //又一次，除了两个增加的参数外，这个函数和 recv() 也是一样的。from 是一个指向局部数据结构 struct sockaddr 的指针，它的内容是源机器的 IP 地址和端口信息。fromlen 是个 int 型的局部指针，它的初始值为 sizeof(struct sockaddr)。函数调用返回后，fromlen 保存着实际储存在 from 中的地址的长度。

//recvfrom() 返回收到的字节长度，或者在发生错误后返回 -1。
 int recvfrom(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen);
 ```
 记住，如果你用 connect() 连接一个数据报套接字，你可以简单的调 用 send() 和 recv() 来满足你的要求。这个时候依然是数据报套接字，依 然使用 UDP，系统套接字接口会为你自动加上了目标和源的信息。
 

## python 
参考连接[https://www.cnblogs.com/fanweibin/p/5053328.html](https://www.cnblogs.com/fanweibin/p/5053328.html)
### demo
server
```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-

import socket

ip_port = ('127.0.0.1',9999)

sk = socket.socket()
sk.bind(ip_port)
sk.listen(5)

while True:
    print 'server waiting...'
    conn,addr = sk.accept()

    client_data = conn.recv(1024)
    print client_data
    conn.sendall('不要回答,不要回答,不要回答')

    conn.close()

socket server
```

client
```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-
import socket
ip_port = ('127.0.0.1',9999)

sk = socket.socket()
sk.connect(ip_port)

sk.sendall('请求占领地球')

server_reply = sk.recv(1024)
print server_reply

sk.close()

socket client
```
