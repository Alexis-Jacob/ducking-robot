#include <iostream>
#include <system_error>
#include "IOService.hpp"

void	handleRead(int fd, IOService &ioservice)
{
  char	buffer[4096] = {0};
  int	ret = 0;
  
  do
    {
      ret = read(fd, buffer, 4096);
      if (ret == -1)
	throw std::system_error(errno, std::system_category());
      std::cout << "nouveau message de[" << fd << "]:\t" << buffer << std::endl; 
      for (auto &kv : ioservice.getFdMap())
	if (kv.first != fd && kv.first != 3)
	  write(kv.first, buffer, ret); //check -1
	
      for (int i = 0; i < 4096; ++i)
	buffer[i] = 0;
    }
  while (ret > 4096);
  
}

 
