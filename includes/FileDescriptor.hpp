#ifndef	__FILEDESCRIPTOR_HPP__
#define __FILEDESCRIPTOR_HPP__

class	FileDescriptor
{
public:
  virtual int	getFd() const
  {
    return (-1);
  }
  virtual void	disconnect()
  {
  }
};


#endif
