#include "IOService.hpp"
#include "Socket.hpp"

void	handleRead(int fd, IOService &ioservice);

int	main()
{
  IOService	ioservice;
  Socket s;

  s.paramSocket();
  s.bind();
  s.listen();
  
  ioservice.addFd(s.getFD(), [&](int fd)
  		  {
		    struct sockaddr_in param;
  		    socklen_t	len = sizeof(param);
  		    int	newFd = accept(fd, reinterpret_cast<struct sockaddr*>(&param), &len);
		    if (newFd == -1)
  		      throw std::system_error(errno, std::system_category());
  		    ioservice.addFd(newFd, handleRead, newFd, ioservice);
		  }
  		  , s.getFD());
  ioservice.run();
}
