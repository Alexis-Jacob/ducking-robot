#ifndef	__SERVER_HPP__
#define	__SERVER_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <system_error>
#include "FileDescriptor.hpp"

class	Socket :  FileDescriptor // final ? 
{
  int	socket_;
  struct sockaddr_in param_;
  int	protocol_;
public:
  ~Socket()
  {
    close(socket_);
  }
  
  Socket(int fd, sockaddr_in & param) : socket_(fd), param_(param), protocol_(param.sin_family)
  {}
  Socket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0) : 
    socket_(-1), protocol_(protocol)
  {
    assert(protocol != AF_INET); // faire pour l'ip6 plus tard
    if ((socket_ = socket(domain, type, protocol)) == -1)
      throw std::system_error(errno, std::system_category());
  }

  void	paramSocket(int port = 4096, const char *ip = nullptr)
  {
    param_.sin_addr.s_addr = (ip == nullptr ? htonl(INADDR_ANY) : inet_addr(ip));   
    param_.sin_family = protocol_;
    param_.sin_port = htons(port);
  }
  
  void	bind()
  {
    int	ok = true;
    
    if(setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(int)) == -1)
      throw std::system_error(errno, std::system_category());
    if ((::bind(socket_, reinterpret_cast<const struct sockaddr*>(&param_), sizeof(param_))) == -1)
      throw std::system_error(errno, std::system_category());
  }
  
  void	listen(int queueSize = 5)
  {
    if ((::listen(socket_, queueSize)) == -1)
      throw std::system_error(errno, std::system_category());
  }
  
  virtual int getFD() const
  {
    return (socket_);
  }
  
};

#endif
