#include "ThreadPool.h"

namespace ThreadPoolDemo{
	/**
	CThreadIOPool
	*/
	CThreadIOPool::CThreadIOPool(int limit, std::shared_ptr<CPriqueueManager> queue):
	CThreadPoolBase(queue), 
	m_limit(limit)
	{
		m_idle = true; 
		auto t = std::make_shared<CThread>(std::bind(&CThreadIOPool::base_run, this));
		m_workers_map.emplace(t->get_id(), t);
		
		m_random |= 1<<3;
		m_random |= 1<<5;
	}
	
	void CThreadIOPool::push_task(const std::function<void()>& task, TaskAttr attr){
		m_queue->push_task(task, attr);
        m_cv.notify_one();
        if(!m_idle && m_workers_map.size() < m_limit){
            auto t = std::make_shared<CThread>(std::bind(&CThreadIOPool::run, this));
			m_workers_map.emplace(t->get_id(), t);
		}
	}
	
	bool CThreadIOPool::get_task(std::function<void()>& task, Priority pri){
        return m_queue->pop_task(task, pri, Type::Io);
    }
	
	void CThreadIOPool::run(){
		std::function<void()> task;
        int counter = 0;
		Priority p;
		int index = 0;
		while(true){
			
			if (random(index)){
				p= Priority::Normal;
			}else{
				p = Priority::Advanced;
			}
			
			if(!get_task(task, p)){
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.wait_for(lock, std::chrono::seconds(1));
			if (counter > 3){
				break;
			}
				counter += 1;
				continue;	    
			}
	    //执行任务
			std::cout << "Io run" << std::endl;
			task();
		}
        
        auto t = *(m_workers_map.find(std::this_thread::get_id()));
        m_workers_map.erase(std::this_thread::get_id());
		TaskAttr attr;
		attr.type = Type::Timing;
		attr.once = true;
		attr.time = 0;
        m_queue->push_task([t](){}, attr);//timeing quue
	}
	
	void CThreadIOPool::base_run(){
		std::function<void()> task;
       
		Priority p;
		int index = 0;
		while(true){
			if (random(index)){
				p= Priority::Normal;
			}else{
				p = Priority::Advanced;
			}
			if(!get_task(task, p)){
				std::unique_lock<std::mutex> lock(m_mutex);
				m_idle = true;
				m_cv.wait_for(lock, std::chrono::seconds(1));

				continue;	    
			}
			 m_idle = false;
			//执行任务
			std::cout << "Io run" << std::endl;
			task();
		}
	}
	
	bool CThreadIOPool::random(int& index){
		if(index > 7){
			index = 0;
		}else{
			index++;
		}
		return (m_random>>index) & 1;
	}
	/**
	CThreadTimingPool
	*/
	CThreadTimingPool::CThreadTimingPool(std::shared_ptr<CPriqueueManager> queue,CThreadPoolBase* Obj):
	CThreadPoolBase(queue)
	{
		m_t = new CThread(std::bind(&CThreadTimingPool::run, this));
                m_IoPool = Obj;
	}
		
	CThreadTimingPool::~CThreadTimingPool(){
		delete m_t;
	}
	
	void CThreadTimingPool::push_task(const std::function<void()>& task, TaskAttr attr){
		m_queue->push_task(task, attr);
	}
	
	bool CThreadTimingPool::get_task(std::function<void()>& task, Priority pri = Priority::Normal){
		return m_queue->pop_task(task, pri, Type::Timing);
	}
	
	void CThreadTimingPool::run(){
		while(true){
			std::function<void()> task;
			if(get_task(task)){
				std::cout << "Timing run" << std::endl;
				//TaskAttr(Type t = Type::Io, Priority pri = Priority::Normal, in tim = 0, bool one = false)
				m_IoPool->push_task(task, TaskAttr());
			}else{
				sleep(2);
			}
		}
	}

	/*
	CThreadCalculatePool
	*/
	CThreadCalculatePool::CThreadCalculatePool(int cpuNum, std::shared_ptr<CPriqueueManager> queue):
	CThreadPoolBase(queue),
	m_cpuNum(cpuNum)
	{
		for(int i = 0; i < m_cpuNum; i++)
		{
			auto t = std::make_shared<CThread>(std::bind(&CThreadCalculatePool::run, this));
			m_workers_map.emplace(t->get_id(), t);
		}

	}

	void CThreadCalculatePool::push_task(const std::function<void()>& task, TaskAttr attr) {
		m_queue->push_task(task, attr);
		m_cv.notify_all();
	}

	bool CThreadCalculatePool::get_task(std::function<void()>& task, Priority pri) {
		return m_queue->pop_task(task, pri, Type::Calculate);
	}


	void CThreadCalculatePool::run() {
		std::function<void()> task;
		Priority p = Priority::Normal;
		int counter = 0;
		while (true) {
			if (!get_task(task, p)) {
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.wait_for(lock, std::chrono::seconds(1));
				
				if(counter < 3){
					counter += 1;
					continue;
				}else{
					counter = 0;
				}
				m_cv.wait(lock);
			}
			std::cout << "Calculate run" << std::endl;
			//执行任务
			task();
		}
	}

	
	
	
	/*
	CThreadManger
	*/
	void CThreadManger::init(int max_io_num, int cpu_num){
        auto queue = std::make_shared<CPriqueueManager>();
		m_Calculate = new CThreadCalculatePool(cpu_num, queue);
		m_IO = new CThreadIOPool(max_io_num, queue);
		m_Timing = new CThreadTimingPool(queue, m_IO);
	}
	
	
	void CThreadManger::push_task(const std::function<void()>& task, TaskAttr attr){
		switch(attr.type){
			case Type::Io:
			{      
				m_IO->push_task(task, attr);
			}
			break;
			case Type::Timing:
			{
				m_Timing->push_task(task, attr);
			}
			break;
			case Type::Calculate:
			{
				m_Calculate->push_task(task, attr); 
			} 
			default:
			return;
		}
	}
	
	
	
	
	
}
