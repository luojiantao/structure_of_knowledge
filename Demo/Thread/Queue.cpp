#include "Queue.h"
static std::atomic<int> lflag(0);
static std::atomic<int> lflag1(0);
void ThreadPoolDemo::CPriqueue::push_back(std::function<void()> task){
    Node* newNode = new Node(task);
    //newNode->next = nullptr;
    if(newNode == 0){
	std::cout << "new error" <<std::endl;
        return;
    }
    Node* oldtail = m_tail;
    Node* temp = nullptr;
    {
	bool iff = true;
        do{
	    if (!iff){
	        std::this_thread::yield();
	    }else{
	        iff = false;
	    }
	    //Node* oldtail = m_tail;
	    while(oldtail->next != nullptr){
	        //oldtail = oldtail->next;
                oldtail = m_tail;
	    }
	    temp = nullptr;
	}while(!oldtail->next.compare_exchange_weak(temp, newNode));

	m_tail.compare_exchange_strong(oldtail, newNode);
    }

    {
	    /*
        while(!m_tail.compare_exchange_weak(oldtail, newNode)){
	    std::this_thread::yield();
	}

	oldtail->next = newNode;
        */
	/*
	Node* temp = nullptr;
	while(!oldtail->next.compare_exchange_weak(temp, newNode)){
	    std::this_thread::yield();
	    temp = nullptr;
	    oldtail = m_tail;
	}
        m_tail.compare_exchange_strong(oldtail, newNode);*/
    }
    return;
}

bool ThreadPoolDemo::CPriqueue::pop_front(std::function<void()>& task){
    Node* oldhead = m_head;//内存引用计数加一
    do{
        //oldhead = m_head;
	if(oldhead->next == nullptr){
	    lflag1 +=1;
	    return false;
	}
    }while(!m_head.compare_exchange_weak(oldhead, oldhead->next));
		lflag += 1;
    oldhead->next.load()->load(task);
    delete oldhead;//TODO,内存引用计数，内存引用计数减一
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
    return (m_head.load()->next == nullptr);
}

void ThreadPoolDemo::CPriqueue::init(){
    std::function<void()> tt = [](){return;};
    m_head = new Node(tt);
    //m_head = m_Eof;
    m_tail = m_head.load();
    m_head.load()->next = nullptr;
}
