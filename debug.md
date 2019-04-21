# 工具说明
- Valgrind：适合解决非法读写，无主内存泄露等问题定位。
- gperftools：长期运行进程内存使用量应该是稳定的，gperftools适合发现运行过程中内存差异，提供泄露的函数范围。而且对影响影响小，适合高压力快速复现。
- getrusage：虽然对于定位问题帮助不大，但是可以用以守护进程监控管理业务进程的内存使用情况。

# gperftools
 
 ## 安装
  1. 安装libunwind 
  wget http://download.savannah.gnu.org/releases/libunwind/libunwind-0.99-beta.tar.gz
  ubuntu apt-get install libunwind8-dev
  2. 安装google-perfile
  wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.5/gperftools-2.5.tar.gz
  
# heap profiler
定位程序在哪些地方分配了内存以及分配多少内存，当然也可以用作检测内存泄漏(函数级别)
## 配置方法
  1. vi /etc/ld.so.conf.d/usr_local_lib.conf  
  2. 添加 /usr/local/lib(默认安装路径), ldconfig
  3. 设置应用程序
  ```sh
  #export LD_PRELOAD=/usr/loacl/lib/libtcmalloc.so #连接这个库
  #export HEAPPROFILE=/root/luo/test               #打印文件名字和路径
  
  g++ [...] -g -o myprogram -ltcmalloc
  HEAPPROFILE=/tmp/netheap ./myprogram
  # 如果项目中不可以链接tcmalloc,可以使用如下调用方式(执行程序需要带有调试信息，符号表)
  LD_PRELOAD="/usr/local/lib/libtcmalloc.so" HEAPPROFILE=/tmp/netheap ./myprogram
  ```
## 使用
  1. 运行要测的程序(如果没有参数打印文件，需要重新编译程序连接libtcmalloc.so这个库)
  2. 使用pprof分析: pprof --text 程序执行绝对路径 打印文件绝对路径
  
  
  ```sh
  pprof --text ./myprogram /tmp/netheap.0001.heap
  ```
### 报告说明
```
Total: 237.3 MB
   129.2  54.5%  54.5%    129.2  54.5% deflateInit2_
    57.5  24.2%  78.7%     57.5  24.2% os::malloc
    48.0  20.2%  98.9%     48.0  20.2% init
```
  - 第一列代表这个函数调用本身直接使用了多少内存
  - 第二列表示第一列的百分比
  - 第三列是从第一行到当前行的所有第二列之和
  - 第四列表示这个函数调用自己直接使用加上所有子调用使用的内存总和
  - 第五列是第四列的百分比
  
# Heap Checker
用来检查内存泄漏的，这里 还有一个google的组件也是做类似事情。主要是代码级别的问题
## 使用

```
gcc [...] -o myprogram -ltcmalloc
HEAPCHECK=normal ./myprogram
# 如果项目中不可以链接tcmalloc,可以使用如下调用方式(执行程序需要带有调试信息，符号表)
LD_PRELOAD="/usr/local/lib/libtcmalloc.so" HEAPCHECK=normal ./myprogram
```
## 报告说明
这个报告说明查看参考链接就行

# cpu profiler
检查程序性能（函数级别）
## 使用

```sh
gcc [...] -o myprogram -lprofiler
CPUPROFILE=cpu_perf.prof ./myprogram
# 如果项目中不可以链接tcmalloc,可以使用如下调用方式(执行程序需要带有调试信息，符号表)
LD_PRELOAD="/usr/local/lib/libprofiler.so" CPUPROFILE=cpu_perf.prof ./myprogram
```
使用下面命令解析输出文件
```sh
pprof --text ./myprogram cpu_perf.prof
```
## 报告说明
是通过定时采样的方式，来统计各个函数性能
报告如下：
```sh
Total: 62 samples
      56  90.3%  90.3%       56  90.3% __write_nocancel
       2   3.2%  93.5%        2   3.2% __GI__IO_fflush
       1   1.6%  95.2%        1   1.6% __GI___mempcpy
       1   1.6%  96.8%        1   1.6% __gnu_cxx::stdio_sync_filebuf::sync
       1   1.6%  98.4%        4   6.5% std::ostream::flush
       1   1.6% 100.0%       57  91.9% std::ostream::put
       0   0.0% 100.0%       56  90.3% _IO_new_do_write
       0   0.0% 100.0%       56  90.3% _IO_new_file_write
       0   0.0% 100.0%        1   1.6% _IO_new_file_xsputn
       0   0.0% 100.0%       56  90.3% __GI__IO_file_overflow
       0   0.0% 100.0%        1   1.6% __GI__IO_fwrite
       0   0.0% 100.0%       56  90.3% __GI__IO_putc
       0   0.0% 100.0%       62 100.0% __libc_start_main
       0   0.0% 100.0%       62 100.0% _start
       0   0.0% 100.0%       62 100.0% main
       0   0.0% 100.0%        1   1.6% std::__ostream_insert
       0   0.0% 100.0%       57  91.9% std::endl
       0   0.0% 100.0%        1   1.6% std::operator<< 

```
- 第一列 当前函数采样次数
- 第二列 采样百分比
- 第三列 第K行的值是前面K行带有百分比总和
- 第四列 当前函数包含子函数，累积采样次数
- 第五列 累积采样百分比

# 注意事项
如果运行程序是非正常退出（被信号杀死），可能不会产生输出分析文件。
解决办法：
1. 重载信号处理函数
```c++
void signal_handler(int signo)
{
    signal(signo, signal_handler);
    INFO_LOG("recv signal[%d]", signo);
    switch(signo)
    {      
        case SIGTERM:
             //程序自己退出，或shell里调用kill缺省该进程。该信号可以被阻塞，或被处理
             //可以在这里做一些程序退出前的最后处理工作
             ProfilerStop();
             INFO_LOG("Process recieve SIGTERM");
             break;      
    }
    exit(0);
}
//主函数
signal(SIGTERM, &signal_handler);
```
2. 使用指定函数开始和结束，分析过程
```C++
#include <gperftools/profiler.h>
....
int main(int argc, const char* argv[])
{
	ProfilerStart("test_capture.prof");
	.....
	ProfilerStop();
}

//-----------------------------------------------
#include <gperftools/heap-profiler.h>
....
int main(int argc, const char* argv[])
{
	HeapProfilerStart("test_capture.prof");
	.....
	HeapProfilerStop();
}
//------------------------------------------------
 #include <cstdio>
 #include <cstdlib>
 #include <cassert>
 #include <google/heap-checker.h>
 int* fun(int n)
 {
     int *p2;
     HeapLeakChecker heap_checker("fun");
     {
         new int[n];
         p2=new int[n];
         //delete [] p1;
     }
     assert(!heap_checker.NoLeaks());
     return p2;    
 }
 int main(int argc,char* argv[])
 {
     int n;
     scanf("%d",&n);
     int *p=fun(n);
     delete [] p;
     return 0;
 } 
 //注意被检查程序的main函数形式必须为带2个参数的形式，否则会在编译时报告重复定义。运行env命令行将会报告assert失败。

  //  另外，还可以跳过某些代码段的检查，方式如下： 

        {
        HeapLeakChecker::Disabler disabler;
        <leaky code>
    } 

  <leaky code>处的代码将被heap-checker忽略。
```
# 参考连接
[link](https://www.cnblogs.com/my_life/articles/4683313.html)
[gperftools使用](https://dirtysalt.github.io/html/gperftools.html#org5322dbc)
https://github.com/gperftools/gperftools/blob/master/README
https://gperftools.github.io/gperftools/cpuprofile.html
https://github.com/gperftools/gperftools/wiki
https://blog.csdn.net/okiwilldoit/article/details/50884670

