#pragma once
#include <iostream>
#include <future>
#include <thread>
#include <memory>
#include <unistd.h>
#include <queue>

namespace ThreadPoolDemo{
/*
struct TaskData{
	std::future<ReType> m_response;
	std::function<ReType()> m_TaskFun;
	std::atomic<bool> m_IsRun;
};
*/
//临时代码
//------------------------------------
static std::queue< std::function<void()> > *tasks  = new std::queue< std::function<void()> >();
template<class T>
void push(T task){
//    (*task)();
    //tasks->emplace([task]() { (*task)(); });
    std::thread th([task](){ sleep(3); std::cout << "exec" << std::endl; (*task)();});
    th.detach();
} 
//-------------------------------------
#define PUSHTASK(task, attr) push(task); 


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

template<class F, class... Args>
class CTaskObj;

template<class F, class... Args>
std::shared_ptr<CTaskObj<F, Args...>> Make_CTaskObj(TaskAttr attr, F&& f, Args&&... args);

template<class T>
class CGetResponse{
public:
CGetResponse(){};
CGetResponse& operator=(std::future<T>&& v){
    m_value = std::move(v);
    return *this;
}
CGetResponse(const CGetResponse& a) = delete;
CGetResponse& operator=(const CGetResponse& a) = delete;
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
private:
    std::future<T> m_value;
};

template<class T>
class CGetResponse<T&>{
public:
CGetResponse(){};
CGetResponse& operator=(std::future<T&>&& v){
    m_value = std::move(v);
    return *this;
}
CGetResponse(const CGetResponse& a) = delete;
CGetResponse& operator=(const CGetResponse& a) = delete;
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
private:
    std::future<T&> m_value;
};

template<>
class CGetResponse<void>{
public:
CGetResponse(){};
CGetResponse& operator=(std::future<void>&& v){
    m_value = std::move(v);
    return *this;
}
CGetResponse(const CGetResponse& a) = delete;
CGetResponse& operator=(const CGetResponse& a) = delete;
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
private:
    std::future<void> m_value;
};

template<class F, class... Args>
class CTaskObj{
friend  std::shared_ptr<CTaskObj<F, Args...>> Make_CTaskObj<F, Args...>(TaskAttr attr, F&& f, Args&&... args);
using ReType = typename std::result_of<F(Args...)>::type;
public:
    CTaskObj(const CTaskObj &a) = delete;
    virtual ~CTaskObj() {
        std::cout << "task del" << std::endl;
    };
    void SetTaskFun(F&& f, Args&&... args);
    void SetTaskAttr(TaskAttr& attr);
    bool Start();
    CGetResponse<ReType> GetResponse;
private:
    CTaskObj(){}
    std::function<ReType()> m_TaskFun;
    //std::atomic<bool> m_IsRun;
    TaskAttr m_attr;
     //std::shared_ptr<std::packaged_task<ReType()>> m_task;
     std::weak_ptr<std::packaged_task<ReType()>> m_task;
};
}

template<class F, class... Args>
void ThreadPoolDemo::CTaskObj<F, Args...>::SetTaskAttr(TaskAttr& attr){
	m_attr = attr;
}

template<class F, class... Args>
std::shared_ptr<ThreadPoolDemo::CTaskObj<F, Args...>> ThreadPoolDemo::Make_CTaskObj(TaskAttr attr ,F&& f, Args&&... args){
	//auto Obj =  std::make_shared<ThreadPoolDemo::CTaskObj<F, Args...>>();
	ThreadPoolDemo::CTaskObj<F, Args...>* noname =  new ThreadPoolDemo::CTaskObj<F, Args...>(); 
	std::shared_ptr<ThreadPoolDemo::CTaskObj<F, Args...>> Obj(noname);
	Obj->SetTaskFun(std::forward<F>(f), std::forward<Args>(args)...);
	Obj->SetTaskAttr(attr);
	return Obj;
}
template<class F, class... Args>
bool ThreadPoolDemo::CTaskObj<F, Args...>::Start(){
    {
	if(!m_TaskFun){
    	    std::cout << "fun error "<< std::endl;
	    return false;	
	}
        auto task = std::make_shared<std::packaged_task<ReType()> >( std::move(m_TaskFun)  //传递函数进行构造
		    );
	m_TaskFun = nullptr;
        m_task = task;
	GetResponse = task->get_future();
        PUSHTASK(task, m_attr);
    }
    std::cout << "ref count: " << m_task.use_count() << std::endl;
    return !m_task.expired();
}
template<class F, class... Args>
void ThreadPoolDemo::CTaskObj<F, Args...>::SetTaskFun(F&& f, Args&&... args){
    m_TaskFun = std::bind(std::forward<F>(f), std::forward<Args>(args)...); 
    return;
}
