#include "task.h"
#include <functional>
int add(int a, int b){
    return a + b;
}
void add2(int a, int b){
   sleep(10);
return;
}
int main(){
   // ThreadPoolDemo::CTaskObj<std::function<int(int,int)>, int, int> aa1;
   //auto aa1 = std::make_shared<ThreadPoolDemo::CTaskObj<std::function<int(int,int)>, int, int>>();
   // aa.SetTaskFun(add, 1,6);
    //std::unique_ptr<ThreadPoolDemo::CTaskObj<std::function<int(int,int)>, int, int>> aa2( new ThreadPoolDemo::CTaskObj<std::function<int(int,int)>, int, int>());
   {
    ThreadPoolDemo::TaskAttr attr;
    std::function<int(int,int)> func = add;
    std::function<void(int,int)> func2 = add2;
    auto aa = ThreadPoolDemo::Make_CTaskObj(attr, func, 1,1, 0);
    //aa->SetTaskFun(func, 1,6);
    aa->m_response();
    //std::cout << "test " << aa->GetResponse() <<std::endl;
    }
    std::cout << "test1" << std::endl;
    return 0;
}
