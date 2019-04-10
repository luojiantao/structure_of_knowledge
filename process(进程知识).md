# 进程数据布局
从高地址到地址：  
  1.环境变量(Unix/Linux中全局环境变量，命令行参数)  
  2.stack区，存放内容和上文同。  
  3.heap区，存放内容和上文同。值得说明的是：stack区起始地址是在高地址，即是从高地址向低地址延伸。而heap区起始地址是在低地址，即是从低地址向高地址延伸。总结：stack起始地址固定在高地址，heap起始地址固定在低地址，然后两个区都向中间延伸。直到stack区和heap区的结束地址重合则表示没有stack和heap内存空间了。  
  4.data区，分为bss未初始化的数据区和初始化的数据区。  
  5.文本(text)区，存放代码的区域。  

# 进程锁
```C++

#include <stdio.h>
int main()
{
 
 
    HANDLE mutex;
    mutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,"memtest");
    if(mutex == NULL)
    {
        mutex = CreateMutex(NULL,FALSE,"memtest");
    }
    printf("0X%X\n",mutex);
    printf("0X%X\n",WaitForSingleObject(mutex,INFINITE));//测试返回码
    int x;
    while(scanf("%d",&x)!=EOF)
    {
        printf("x= %d\n",x);
        if(x == 10)
             break;
    }
    //ReleaseMutex(mutex);
    return 0;
}
```
## 文件锁
### linux
1. 函数名  
  int flock(int fd,int operation);
2. 头文件  
   #include<sys/file.h>
3. 函数说明  
  flock()会依参数operation所指定的方式对参数fd所指的文件做各种锁定或解除锁定的动作。**此函数只能锁定整个文件，无法锁定文件的某一区域**
4. 参数 operation 说明  
  LOCK_SH 建立共享锁定。多个进程可同时对同一个文件作共享锁定。  
  LOCK_EX 建立互斥锁定。一个文件同时只有一个互斥锁定。  
  LOCK_UN 解除文件锁定状态。  
  LOCK_NB 无法建立锁定时，此操作可不被阻断，马上返回进程。通常与LOCK_SH或LOCK_EX 做OR(|)组合。  
5. 单一文件无法同时建立共享锁定和互斥锁定，而当使用dup()或fork()时文件描述词不会继承此种锁定。返回值  返回0表示成功，若有错误则返回-1，错误代码存于errno。
#### 使用
flock只要在打开文件后，需要对文件读写之前flock一下就可以了，用完之后再flock一下，前面加锁，后面解锁。  
一个进程去打开文件，输入一个整数，然后上一把写锁（LOCK＿EX），再输入一个整数将解锁（LOCK＿UN），另一个进程打开同样一个文件，直接向文件中写数据，发现锁不起作用，能正常写入（我此时用的是超级用户）。google了一大圈发现flock不提供锁检查，也就是说在用flock之前需要用户自己去检查一下是否已经上了锁，说明白点就是读写文件之前用一下flock检查一下文件有没有上锁，如果上锁了flock将会阻塞在那里(An attempt to lock the file using one of these file descriptors may be denied by a lock that the calling process has already placed via another descriptor )，除非用了LOCK_NB。
```C++
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


int fd,i;
char path[]="/home/taoyong/test.txt";
extern int errno;
fd=open(path,O_WRONLY|O_CREAT);
if(fd!=-1)
{
   if(flock(fd,LOCK_EX)==0)
   {
     printf("the file was locked./n");
   }else
   {
     printf("the file was not locked./n");
   }
   
    if(flock(fd,LOCK_UN)==0)
    {
      printf("the file was unlocked./n");
    }else
    {
      printf("the file was not unlocked./n");
    }
     close(fd);
}
//---------------------------------
   int fd,i;
    char path[]="/home/taoyong/test.txt";
    char s[]="writing.../nwriting....../n";
    extern int errno;
    fd=open(path,O_WRONLY|O_CREAT|O_APPEND);
    if(fd!=-1)
            {
        printf("open file %s ./n",path);

            if(flock(fd,LOCK_EX|LOCK_NB)==0)
            {
            printf("the file was locked by the process./n");    
                if(-1!=write(fd,s,sizeof(s)))
                    {
                printf("write %s to the file %s/n",s,path);
                        }
                else
                       {
                printf("cannot write the file %s/n",path);
                printf("errno:%d/n",errno);
                printf("errMsg:%s/n",strerror(errno));
                    }        
                
            }
        else
            {
            printf("the file was locked by other process.Can't write.../n");
                printf("errno:%d:",errno);
            }
        
        close(fd);


            }
        else
           {
        printf("cannot open file %s/n",path);
        printf("errno:%d/n",errno);
        printf("errMsg:%s",strerror(errno));
            }

```
# 父子进程的关系
# 进行收到信号的行为
# 进程间通信
## 信息量
## 管道
## 共享内存
