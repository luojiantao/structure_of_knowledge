# 工具说明
- Valgrind：适合解决非法读写，无主内存泄露等问题定位。
- gperftools：长期运行进程内存使用量应该是稳定的，gperftools适合发现运行过程中内存差异，提供泄露的函数范围。而且对影响影响小，适合高压力快速复现。
- getrusage：虽然对于定位问题帮助不大，但是可以用以守护进程监控管理业务进程的内存使用情况。

# gperftools
 
 ## 安装
  1. 安装libunwind 
  wget http://download.savannah.gnu.org/releases/libunwind/libunwind-0.99-beta.tar.gz
  2. 安装google-perfile
  wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.5/gperftools-2.5.tar.gz
  
## 配置方法
  1. vi /etc/ld.so.conf.d/usr_local_lib.conf  
  2. 添加 /usr/local/lib(默认安装路径), ldconfig
  3. 设置应用程序
  ```sh
  export LD_PRELOAD=/usr/loacl/lib/libtcmalloc.so #连接这个库
  export HEAPPROFILE=/root/luo/test               #打印文件名字和路径 
  ```
## 使用
  1. 运行要测的程序(如果没有参数打印文件，需要重新编译程序连接libtcmalloc.so这个库)
  2.使用pprof分析: pprof --text 程序执行绝对路径 打印文件绝对路径
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
