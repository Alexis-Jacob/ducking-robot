#ifndef	__THREADPOOL_HPP__
#define	__THREADPOOL_HPP__

#include <future>
#include <functional>
#include <deque>
#include <thread>
#include <vector>
#include <boost/any.hpp>
#include <iostream>

class	Worker;

class	ThreadPool
{
  friend class Worker;
  bool	stop_;
  std::mutex queueMutex_;
  std::condition_variable condition_;
  std::vector<std::thread> threadsList_;
  std::deque<std::function<void()> >	tasks_;
public:
  //template<class F, class... Args>
  //void enqueue(F&& f, Args&&... args)
  void enqueue(std::function<void()> fct)
  {
    std::unique_lock<std::mutex> lock(queueMutex_);
    //tasks_.push_back(std::bind(f, args ...));
    tasks_.push_back(fct);
    condition_.notify_one();
  }
  ThreadPool(std::size_t size);
  ~ThreadPool();
};

class Worker 
{
  ThreadPool &pool_;
public:
  Worker(ThreadPool &s) : pool_(s) { }

  void operator()()
  {
    std::function<void()> task;
    while (true)
      {
	std::unique_lock<std::mutex> lock(pool_.queueMutex_);
	while(!pool_.stop_ && pool_.tasks_.empty())
	  pool_.condition_.wait(lock);
	if(pool_.stop_ && pool_.tasks_.size() == 0)
	  return;
	task = pool_.tasks_.front();
	pool_.tasks_.pop_front();
	lock.unlock();
	task();
      }
  }

};

ThreadPool::ThreadPool(std::size_t size) : stop_(false)
{
  for(size_t i = 0;i<size;++i)
    threadsList_.push_back(std::thread(Worker(*this)));
}

ThreadPool::~ThreadPool()
{
  stop_ = true;
  condition_.notify_all();
  for (auto & key : threadsList_)
    key.join();
}


#endif
