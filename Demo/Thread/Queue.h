#pragma once
#include "task.h"
#include <list>
#include <atomic>
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
    Node():next(nullptr), task(nullptr)
    {}
    Node(std::function<void()>&& func):next(nullptr), task(func)
    {}
    ~Node(){}
    std::function<void()> task;
    Node* next;
    int value = 0;
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
    void push_back(int v);

    bool pop_front(int& task);

    void print();
    bool empty();

    void print_ptr(){
       std::cout << "EOF: " << m_Eof->value << std::endl;
       std::cout << "head: " << (m_head.load())->value << std::endl;
       std::cout << "tail: " << (m_tail.load())->value << std::endl;
    }

private:
    void init();
    Type m_type;
    Priority m_pri;
    std::atomic<Node*> m_head;
    std::atomic<Node*> m_tail;
    Node* m_Eof;
};

}
