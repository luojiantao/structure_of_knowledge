#include "Queue.h"
static std::atomic<int> lflag(0);
static std::atomic<int> lflag1(0);
void ThreadPoolDemo::CPriqueue::push_back(int v){
    Node* oldtail = m_tail;
    Node* newNode = new Node();
    newNode->value = v;
    newNode->next = nullptr;
    
    while(!m_tail.compare_exchange_weak(oldtail, newNode)){
        // usleep(10);
    }
    oldtail->next = newNode;
    lflag1 += 1;
    return;
    //lazy clear
    /*while( m_Eof != m_head){
        Node* tmp = m_Eof;
	m_Eof = m_Eof->next;
	delete tmp;
    }*/
}

bool ThreadPoolDemo::CPriqueue::pop_front(int& task){
    //判断节点是否指向Eof
    //YES,m_head = m_Eof->next
    //NO, flag = m_head 不处理
   if( m_head == m_tail){
       lflag +=1;
       return false;
   }
     
    Node* oldhead = m_head;
    while(oldhead->next == nullptr){
       usleep(100);
    }
    //Node* newhead = oldhead->next;
    //CAS:if m_head == oldhead, then m_head = newhead
    while(!m_head.compare_exchange_weak(oldhead, oldhead->next)){
     while(oldhead->next == nullptr){
         usleep(100);
     }
    }
    int val = oldhead->next->value;
    task = val;
    delete oldhead;  // 需要修改
    return true;
}

void ThreadPoolDemo::CPriqueue::print(){
    Node* flag = m_head.load()->next;
    int num = 0;
    while(flag != nullptr){
        ++num; 
	//std::cout << flag->value << " ";
	flag = flag->next;
    }
    std::cout << std::endl;
    std::cout <<"num: " <<num << std::endl;
    std::cout <<"flag: " << lflag.load() << std::endl;
    std::cout <<"flag1: " << lflag1.load() << std::endl;
}

bool ThreadPoolDemo::CPriqueue::empty(){
    return (m_head == m_tail);
}

void ThreadPoolDemo::CPriqueue::init(){
    m_head = new Node();
    //m_head = m_Eof;
    m_tail = m_head.load();
    m_head.load()->next = nullptr;
}
