#include "task.h"
#include "Queue.h"
#include "ThreadPool.h"
#include <functional>
#include <thread>
#include <list>
int add(int a, int b){
    std::cout << "aaaa" << std::endl;
    return a + b;
}
void add1(){
	int a = 1+ 1;
    return ;
}
void add2(int a, int b){
   sleep(10);
return;
}
#define NUM 200
int main(){
   {
    //unsigned char* m_random = new unsigned char[2];
    //char m_random  = {0};
    //m_random |= 1 << 3; 
    //m_random |= 1 << 7;
    //int retx = (m_random >>  4) &1;
    //std::cout << retx << std::endl;
    //return 0; 
    //m_random |= 1 << 11; 
    //m_random |= 1 << 15; 
    ThreadPoolDemo::TaskAttr attr;
    attr.type = ThreadPoolDemo::Type::Calculate;
    std::function<int(int,int)> func = add;
    std::function<void(int,int)> func2 = add2;
    auto aa = ThreadPoolDemo::Make_CTaskObj(attr, add, 1,1);
    //aa->SetTaskFun(func, 1,6);
    std::cout << "ret: " <<  aa->Start() << std::endl;
    // aa->Start();
    //aa->GetResponse();
    std::cout << "test " << aa->GetResponse() <<std::endl;
    }
    getchar();
    return 0;
   /* std::cout << "test1" << std::endl;
    std::list<int> ll1;
    //ll1.push_front(1);
    ll1.push_front(1);
    ll1.push_back(2);
    ll1.push_back(3);
    ll1.push_back(4);
    ll1.push_back(5);
    //end 可以理解为一个特殊标志位。类似EOF等这些
    std::list<int>::iterator ret = ll1.end(); 
    std::cout << "list ret " << *(--ret) <<std::endl;
    std::cout << (ll1.end()== ll1.begin()) <<std::endl;
    //std::cout << static_cast<const void *>(ll1.begin()) <<std::endl;
    //
    //
    */
    //-------------------------
    {
    }
    ThreadPoolDemo::CPriqueue li;
    {
	auto fun_test = [&li](int v){
	    
	    for(int i =0; i < 10000;i++){
	   //auto func = [i, v](){return v*10000 + i;};
                std::function<void()> func = [](){
			int ret = 1+ 1;
			//usleep(1);
	                std::cout << ret << std::endl;
			return;
		}; 
	        li.push_back(func);
	    }	
	    return; 
	};
	auto fun_test1 = [&li](){
	    for(int i = 0; i < 1000; i++){
            std::function<void()> a = nullptr;	    
            //std::shared_ptr<std::function<void()>> a;	    
	    if(li.pop_front(a)){
	        //(a)();
	    }
	    }
	};
	/*
        std::thread tt3([&li](){
			for(int i = 0;i < 20 ;i ++){
			sleep(1);
			std:: cout << "time " << i << std::endl;
			li.print();}
			});
        tt3.detach();*/	
	std::thread* t[NUM];
	std::thread* t2[NUM];
	int i =0;
        for(i =1; i <= NUM;i++){
	    //std::cout << i << "";
	    //std::cout << "luo " << std::endl;
	    t[i] = new std::thread(fun_test, i);
	    t2[i] = new std::thread(fun_test1);
	}
//	std::thead tt();
	for (i = 1;i <= NUM;i++ ){
	    t[i]->join();
	    t2[i]->join();
	    delete t[i];
	}
	//std::cout << "li " << li.empty() << std::endl;
//        li.push_back(6);
//        li.push_back(7);
	//sleep(2);
	std::cout << "-----------" << std::endl;
	li.print();
	std::cout << "-----------" << std::endl;
	std::function<void()> val;
       int num = 0;
       while(!li.empty()){
           num += 1;
	   li.pop_front(val);
	    //std::cout << "val: " <<val << std::endl;
       } 
	//    break;
	std::cout << "pop num: " << num << std::endl;
	std::cout << "-----------" << std::endl;
	std::cout << "empty: " << li.empty() << std::endl;
	//getchar();
    }
    return 0;
}

