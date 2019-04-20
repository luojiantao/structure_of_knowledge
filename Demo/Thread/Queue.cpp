#include "Queue.h"
static std::atomic<int> lflag(0);
static std::atomic<int> lflag1(0);
void ThreadPoolDemo::CPriqueue::push_back(std::function<void()> task){
    Node* oldtail = m_tail;
    Node* newNode = new Node(task);
    //newNode->next = nullptr;
    if(newNode == 0){
	std::cout << "new error" <<std::endl;
        return;
    }
    
    while(!m_tail.compare_exchange_weak(oldtail, newNode)){
	 std::this_thread::yield();
        // usleep(10);
    }
    //oldtail->next = newNode;
    oldtail->link_tail(newNode);
    return;
    //lazy clear
    /*while( m_Eof != m_head){
        Node* tmp = m_Eof;
	m_Eof = m_Eof->next;
	delete tmp;
    }*/
}

bool ThreadPoolDemo::CPriqueue::pop_front(std::function<void()>& task){
    //判断节点是否指向Eof
    //YES,m_head = m_Eof->next
    //NO, flag = m_head 不处理
   if( m_head == m_tail){
       lflag1 += 1;
       return false;
   }
     
    Node* oldhead = m_head;
    while(oldhead->next == nullptr){
            lflag +=1;
	    //usleep(100);
	    std::this_thread::yield();
    }
    //Node* newhead = oldhead->next;
    //CAS:if m_head == oldhead, then m_head = newhead
    while(!m_head.compare_exchange_weak(oldhead, oldhead->next)){
     while(oldhead->next == nullptr){
	     std::this_thread::yield();
     }
	     std::this_thread::yield();
    }
    while(true){
        //oldhead->next.load()->load(task);
        oldhead->next->load(task);
        if(task){
	    break;
	}else{
	    //std::cout << "luo" << std::endl;
            std::this_thread::yield();
	    continue;
	}
	//usleep(100);
    }
    
    delete oldhead;  // 需要修改
    return true;
}
/*
//bool pop_front(std::shared_ptr<std::function<void()>>& task);
bool ThreadPoolDemo::CPriqueue::pop_front(std::shared_ptr<std::function<void()>>& task){
    //判断节点是否指向Eof
    //YES,m_head = m_Eof->next
    //NO, flag = m_head 不处理
   if( m_head == m_tail){
       lflag1 += 1;
       return false;
   }
     
    Node* oldhead = m_head;
    while(oldhead->next == nullptr){
	    std::this_thread::yield();
    }
    //Node* newhead = oldhead->next;
    //CAS:if m_head == oldhead, then m_head = newhead
    while(!m_head.compare_exchange_weak(oldhead, oldhead->next)){
     while(oldhead->next == nullptr){
	     std::this_thread::yield();
     }
	     std::this_thread::yield();
    }
    //task = *((oldhead->next.load()->ptr_task).load());
    task = oldhead->next.load()->ptr_share_task;
    while(task.get() == nullptr){
       lflag +=1;
       task = oldhead->next.load()->ptr_share_task;
    //task = oldhead->next->task;
    //task = *((oldhead->next.load()->ptr_task).load());
    //task = oldhead->next.load()->ptr_task;
    std::this_thread::yield();
    }
    delete oldhead;  // 需要修改
    return true;
}*/





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
    std::cout <<"head: " << static_cast<void *>(m_head.load()) << std::endl;
    std::cout <<"tail: " << static_cast<void *>(m_tail.load()) << std::endl;
}

bool ThreadPoolDemo::CPriqueue::empty(){
    return (m_head == m_tail);
}

void ThreadPoolDemo::CPriqueue::init(){
    std::function<void()> tt = [](){return;};
    m_head = new Node(tt);
    //m_head = m_Eof;
    m_tail = m_head.load();
    m_head.load()->next = nullptr;
}
