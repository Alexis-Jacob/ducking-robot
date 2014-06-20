#ifndef	__IOSERVICE_HPP__
#define __IOSERVICE_HPP__

#include <assert.h>
#include <poll.h>

#include <vector>
#include <functional>
#include <map>
#include <stdexcept>
#include <system_error>
#include <iostream>
 
static	void	defaultCallBack()
{
  std::cout << "default call back" << std::endl;
}

class	IOService
{

  std::vector<struct pollfd> fds_;
  bool	run_;
  std::function<void()>	defaultCallBack_;
  std::map<int, std::function<void()> > callBackForFD_;
  
  void	addFd_(int newFd)
  {
    assert(newFd!=-1); //maybe throw ?
    for (auto & fd : fds_)
      {
	if (fd.fd == -1)
	  {
	    fd.fd = newFd;
	    fd.events = POLLIN|POLLRDHUP;
	    return ;
	  }
      }
    fds_.push_back({newFd, POLLIN|POLLRDHUP,0});
  }
public:
  IOService() : run_(true), defaultCallBack_(defaultCallBack)
  {
  }

  ~IOService()
  {
  }

  const std::map<int, std::function<void()>> &getFdMap() const
  {
    return callBackForFD_;
  }

  void	run()
  {
    while (run_)
    {
      if (poll(fds_.data(), fds_.size(), -1) != -1)
    	{
	  for (auto it = fds_.begin(); it != fds_.end();++it)
    	    {
	      auto fd = *it;
	      if (fd.fd != -1)
		{
		  if (fd.revents & POLLRDHUP)
		    {
		      close(fd.fd);
		      fd.fd = -1;
		    }
		  else if (fd.revents & POLLIN)
		    {
		      try
			{
			  callBackForFD_.at(fd.fd)();
			  break;
			}
		      catch (const std::out_of_range& err)
			{
			  defaultCallBack_();
			}
		    }
		}
    	    }
    	}
      else
    	throw std::system_error(errno, std::system_category());
    }
  }
  
  template <class F, class ... Args>
  void	addFd(int fd, F && fct, Args && ... args)
  {
    callBackForFD_[fd] = std::bind(fct, args...);
    addFd_(fd);
  }

  template<class F>
  void	addFd(int fd, F && fct)
  {
    callBackForFD_[fd] = std::bind(fct);
    addFd_(fd);
  }
  
  void	addFd(int fd)
  {
    addFd_(fd);
  }
};


#endif
