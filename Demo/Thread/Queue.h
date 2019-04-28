#pragma once
//#include "task.h"
//#include "blockingconcurrentqueue.h"
#include "concurrentqueue.h"
#include <list>
#include <atomic>
#include <mutex>
#include <memory>
#include <unistd.h>
#include <iostream>
#include "BaseStruct.h"
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

struct Node{
	Node(){}
    Node(std::function<void()>& t):task(t){}
    std::function<void()> task;
};

class CPriqueue{
public:
    CPriqueue(Type type = Type::Io, Priority pri = Priority::Normal):m_type(type), m_pri(pri){
        init();
    }
    ~CPriqueue(){
    }
    void push_back(std::function<void()> task);
    bool pop_front(std::function<void()>& task);
    
    bool pop_front(std::shared_ptr<std::function<void()>>& task);

    bool empty();
    void print();
private:
    void init();
    Type m_type;
    Priority m_pri;
    moodycamel::ConcurrentQueue<Node> m_q;
};

class CTimingQueue{
public:
struct TimingNode{
   int delay = 0;
   bool once = false;
   std::function<void()> task;
};
    void push_back(const std::function<void()>& task, int time = 0, bool once = false);
	
    void push_back(const TimingNode& node);
	
    bool pop_front(std::function<void()>& task);
	
private:
    moodycamel::ConcurrentQueue<TimingNode> m_q;
};

class CPriqueueManager{
public:
    CPriqueueManager():m_AdvancedQueue(Type::Io, Priority::Advanced), m_NormalQueue(Type::Io, Priority::Normal)
    {
    }
    ~CPriqueueManager(){}
	
	void push_task(const std::function<void()>& task, TaskAttr attr);
	
    bool pop_task(std::function<void()> &task, Priority pri, Type type);
	
    bool empty(Type type);
private:
   CPriqueue m_AdvancedQueue;
   CPriqueue m_NormalQueue;
   CPriqueue m_CalculateQueue;
   CTimingQueue m_TimingQueue;
   CPriqueueManager(const CPriqueueManager&) = delete;
   CPriqueueManager& operator=(const CPriqueueManager&) = delete;
};
}
