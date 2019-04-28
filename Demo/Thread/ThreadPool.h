#pragma once
//#include "task.h"
#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <map>
#include "BaseStruct.h"
#include "Queue.h"
/*
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
*/
namespace ThreadPoolDemo{

class CThread{
public:
    CThread(std::function<void()> func){
        m_t = new std::thread(func);
    }
    CThread(std::thread* t):m_t(t){}
    CThread(const CThread& p){
    }
    std::thread::id get_id(){
        return m_t->get_id();
    }
    ~CThread(){
        std::cout << "~CThread" << std::endl;
	if(m_t == nullptr)
	    return;
        if(m_t->joinable())
            m_t->join();
        delete m_t;
    }
private:
    std::thread* m_t;

};

class CThreadPoolBase{
public:
    CThreadPoolBase(std::shared_ptr<CPriqueueManager> queue): m_queue(queue)
	{}
    virtual ~CThreadPoolBase(){
    
    }
    virtual bool get_task(std::function<void()>& task, Priority pri)= 0;//
	
	virtual void push_task(const std::function<void()>& task, TaskAttr attr) = 0;

protected:
    //Type m_type;//    Priority m_pri;
    //Priority m_pri;
    std::shared_ptr<CPriqueueManager> m_queue;
};

class CThreadIOPool: public CThreadPoolBase{
public:
    CThreadIOPool(int limit, std::shared_ptr<CPriqueueManager> queue);
	
    virtual bool get_task(std::function<void()>& task, Priority pri);
	
	virtual void push_task(const std::function<void()>& task, TaskAttr attr);
private:
    void run();
	
    void base_run();
	
	bool random(int& index);
	
    std::map<std::thread::id, std::shared_ptr<CThread>> m_workers_map;
    std::atomic<bool> m_idle;
    int m_limit;
    std::mutex m_mutex;
    std::condition_variable m_cv;
	
	//random
	char m_random = {0};
};

class CThreadTimingPool: public CThreadPoolBase{
public:
    CThreadTimingPool(std::shared_ptr<CPriqueueManager> queue,CThreadPoolBase* Obj);
	
    virtual ~CThreadTimingPool();
	
	virtual void push_task(const std::function<void()>& task, TaskAttr attr);
	
    virtual bool get_task(std::function<void()>& task, Priority pri);

private:
     void run();
	 
     CThread* m_t;
     CThreadPoolBase* m_IoPool;
};

class CThreadCalculatePool: public CThreadPoolBase{
public:
    CThreadCalculatePool(int cpuNum, std::shared_ptr<CPriqueueManager> queue);
	
	virtual ~CThreadCalculatePool(){};
	
	virtual void push_task(const std::function<void()>& task, TaskAttr attr);
	
	virtual bool get_task(std::function<void()>& task, Priority pri);
	
private:
	void run();

	int m_cpuNum;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::map<std::thread::id, std::shared_ptr<CThread>> m_workers_map;
};

class CThreadManger{
public:
	CThreadManger(int max_io_num, int cpu_num){
		 
		init(max_io_num, cpu_num);
	 }
	void init(int max_io_num, int cpu_num);
	
    void push_task(const std::function<void()>& task, TaskAttr attr);

private:
    std::vector<CThreadPoolBase*> m_vpool;//Calculate. Timing, IOAdvanced, IONormal
    CThreadPoolBase* m_Calculate;
    CThreadPoolBase* m_Timing;
    CThreadPoolBase* m_IO;
};
}
