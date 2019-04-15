#include <iostream>
#include <future>
#include <thread>
#include <memory>
#include <unistd.h>

namespace ThreadPoolDemo{
/*
struct TaskData{
	std::future<ReType> m_response;
	std::function<ReType()> m_TaskFun;
	std::atomic<bool> m_IsRun;
};
*/

    enum Type{
       Io = 0,
       Timing,
       Calculate
    };

    enum Priority{
        Normal = 0,
	Advanced
    };
    struct TaskAttr{
        Type type = Type::Io;
	Priority proprity = Priority::Normal;
	int time = 0;
    };

template<class T>
class CGetResponse{
public:
std::future<T> m_value;
CGetResponse& operator=(std::future<T>&& v){
    m_value = std::move(v);
}
T operator() (){
    try{
        m_value.get();
    }catch(const std::exception& e){
        std::cout << "Exception from the thread11: " << e.what() << '\n';
    }
}
T get(){
    return m_value.get();
}
};

template<class T>
class CGetResponse<T&>{
public:
std::future<T&> m_value;
CGetResponse& operator=(std::future<T&>&& v){
    m_value = std::move(v);
}
T& operator() (){
    try{
       return m_value.get();
    }catch(const std::exception& e){
        std::cout << "Exception from the thread11: " << e.what() << '\n';
    }
}
T& get(){
    return m_value.get();
}
};

template<>
class CGetResponse<void>{
public:
std::future<void> m_value;
CGetResponse& operator=(std::future<void>&& v){
    m_value = std::move(v);
}
void operator() (){
    try{
        m_value.get();
    }catch(const std::exception& e){
        std::cout << "Exception from the thread11: " << e.what() << '\n';
    }
}
void get(){
    m_value.get();
}
};


template<class F, class... Args>
class CTaskObj{
friend std::shared_ptr<ThreadPoolDemo::CTaskObj<F, Args...>> std::make_shared<ThreadPoolDemo::CTaskObj<F, Args...>>();
//friend  std::shared_ptr<ThreadPoolDemo::CTaskObj<F, Args...>> Make_CTaskObj<F, Args...>(F&& f, Args&&... args);
using ReType = typename std::result_of<F(Args...)>::type;
public:
    CTaskObj(const CTaskObj &a) = delete;
    virtual ~CTaskObj() {
        std::cout << "task del" << std::endl;
    };
    void SetTaskFun(F&& f, Args&&... args);
    void SetTaskAttr(TaskAttr& attr);
    bool Start();
/*    ReType GetResponse(){
	try{
            return m_response.get();
	}catch(const std::exception& e){
	     std::cout << "Exception from the thread11: " << e.what() << '\n';
	     return 0;
	}
    }*/
    //template<>
    //void GetResponse<void>(){}
    CTaskObj(){}
    CGetResponse<ReType> m_response;
private:
    std::future<ReType> m_response1;
    std::function<ReType()> m_TaskFun;
    //std::atomic<bool> m_IsRun;
    TaskAttr m_attr;
    //ReType m_default_value;
   // std::packaged_task<ReType()> m_task;
};

template<class F, class... Args>
class Make_CTaskObjf{
using ReType = typename std::result_of<F(Args...)>::type;

};

template<class F, class... Args, class Re>
std::shared_ptr<ThreadPoolDemo::CTaskObj<F, Args...>> Make_CTaskObj(TaskAttr attr, F&& f, Re value,Args&&... args);
}

template<class F, class... Args, class Re>
std::shared_ptr<ThreadPoolDemo::CTaskObj<F, Args...>> ThreadPoolDemo::Make_CTaskObj(TaskAttr attr ,F&& f, Re value,Args&&... args){
	auto Obj =  std::make_shared<ThreadPoolDemo::CTaskObj<F, Args...>>();
	Obj->SetTaskFun(std::forward<F>(f), std::forward<Args>(args)...);
	return Obj;
}
template<class F, class... Args>
bool ThreadPoolDemo::CTaskObj<F, Args...>::Start(){
    return true;
}
template<class F, class... Args>
void ThreadPoolDemo::CTaskObj<F, Args...>::SetTaskFun(F&& f, Args&&... args){
    /*auto task = std::make_shared<std::packaged_task<return_type()> >(  //指向F函数的智能指针
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...)  //传递函数进行构造
		);*/
    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    auto m_task = std::packaged_task<ReType()>(func);
    m_response = m_task.get_future();

    //std::thread th(std::move(task));
    //int value = ret.get();
    //std::cout << "The countdown lasted for " << value << " seconds.\n";
    //th.detach();
    return;
}
