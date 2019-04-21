#pragma once
#include "task.h"
#include <list>
#include <atomic>
#include <mutex>
#include <memory>
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
    Node():next(nullptr), task([](){return;})
    {
      //  std::unique_lock<std::mutex> lk(m_mtx);
    }
    Node(std::function<void()>& func)
    {
        //std::unique_lock<std::mutex> lk(m_mtx);
	task = func;
	next = nullptr;
    }
    void load(std::function<void()>& func){
        //std::unique_lock<std::mutex> lk(m_mtx);
	func = task; 
	//func = [](){}; 
    }
    void link_tail(Node* n){
        next = n;
    }
    ~Node(){
	//next = nullptr;
    }
    std::function<void()> task;
    //Node* next;
    std::atomic<Node*> next; 
    int value = 0;
    //std::atomic_flag// TODO
    std::mutex m_mtx;
    char mm[100];
};

class CPriqueue{
public:
    CPriqueue(){
        init();
    }
    ~CPriqueue(){
        delete m_head.load();
	m_head = nullptr;
    }
    //void push_back(int v);
    void push_back(std::function<void()> task);

    //bool pop_front(int& task);
    bool pop_front(std::function<void()>& task);
    
    bool pop_front(std::shared_ptr<std::function<void()>>& task);

    void print();
    bool empty();

    void print_ptr(){
     //  std::cout << "EOF: " << m_Eof->value << std::endl;
       std::cout << "head: " << (m_head.load())->value << std::endl;
       std::cout << "tail: " << (m_tail.load())->value << std::endl;
    }

private:
    void init();
    Type m_type;
    Priority m_pri;
    std::atomic<Node*> m_head;
    std::atomic<Node*> m_tail;
    //Node* m_Eof;
};

}
