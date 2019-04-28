#include <thread>
#include "Queue.h"
#include <iostream>
static std::atomic<int> lflag(0);
static std::atomic<int> lflag1(0);

/*
ThreadPoolDemo::CPriqueue
*/
void ThreadPoolDemo::CPriqueue::push_back(std::function<void()> task){
    //Node* newNode = new Node(task);
    /*if(newNode == 0){
		std::cout << "new error" <<std::endl;
        return;
    }*/
    lflag += 1;
	ThreadPoolDemo::Node newNode(task);
	//newNode.task = task;
    m_q.enqueue(newNode);
    return;
}


bool ThreadPoolDemo::CPriqueue::pop_front(std::function<void()>& task){
    //Node* nn = nullptr;
	ThreadPoolDemo::Node nn;
    bool ret = m_q.try_dequeue(nn);
    if(ret){
        lflag1 += 1;	
        //task = nn->task;
		task = nn.task;
		//delete nn;
		return true;
    }
    return false;
}

void ThreadPoolDemo::CPriqueue::print(){
    std::cout << "lflag: " << lflag.load() << std::endl;
    std::cout << "lfag1: "<< lflag1.load() << std::endl;
}

bool ThreadPoolDemo::CPriqueue::empty(){
    return (m_q.size_approx() == 0);
}

void ThreadPoolDemo::CPriqueue::init(){
}

/*
ThreadPoolDemo::CTimingQueue
*/
void ThreadPoolDemo::CTimingQueue::push_back(const std::function<void()>& task, int time, bool once){
	ThreadPoolDemo::CTimingQueue::TimingNode node;
	node.task = task;
	node.delay = time;
	node.once = once;
	m_q.enqueue(node);	
}

void ThreadPoolDemo::CTimingQueue::push_back(const ThreadPoolDemo::CTimingQueue::TimingNode& node){
	m_q.enqueue(node);
}

bool ThreadPoolDemo::CTimingQueue::pop_front(std::function<void()>& task){
	ThreadPoolDemo::CTimingQueue::TimingNode n;
	if(m_q.try_dequeue(n)){
		task = [n, this](){
			sleep(n.delay);
			n.task();
			if(!n.once){
			   this->push_back(n);
			}
		};
		return true;
	}
	return false;
}
/*
ThreadPoolDemo::CPriqueueManager
*/

void ThreadPoolDemo::CPriqueueManager::push_task(const std::function<void()>& task, TaskAttr attr){
	switch(attr.type){
		case Type::Io:
		{      
			if(attr.priority == Priority::Advanced){
				m_AdvancedQueue.push_back(task);

			}else{
				m_NormalQueue.push_back(task);
			} 
		}
		break;
		case Type::Timing:
		{
			m_TimingQueue.push_back(task, attr.time, attr.once);
		}
		case Type::Calculate:
		{
			//TODO
			m_CalculateQueue.push_back(task);
		} 
		default:
		return;
	}
}

bool ThreadPoolDemo::CPriqueueManager::pop_task(std::function<void()> &task, Priority pri, Type type){
	switch(type){
		case Type::Io:
		{
			//TODO
			if(pri == Priority::Advanced){
                                return m_AdvancedQueue.pop_front(task) ? true : m_NormalQueue.pop_front(task);
/*				if(!m_AdvancedQueue.empty()){
					return m_AdvancedQueue.pop_front(task);
				}else{
					return m_NormalQueue.pop_front(task);
				}*/
			}else{
                                return m_NormalQueue.pop_front(task) ? true : m_AdvancedQueue.pop_front(task);
				/*if(!m_NormalQueue.empty()){
					return m_NormalQueue.pop_front(task);
				}else{
					return m_AdvancedQueue.pop_front(task);
				}*/
		   }
		}
		break;
	   case Type::Timing:
	   {
		   return m_TimingQueue.pop_front(task);
	   }
	   break;
	   case Type::Calculate:
	   {
		   return m_CalculateQueue.pop_front(task);
	   }
	}	
	return false;
}


bool ThreadPoolDemo::CPriqueueManager::empty(Type type){
	return m_AdvancedQueue.empty() && m_NormalQueue.empty(); 
}



