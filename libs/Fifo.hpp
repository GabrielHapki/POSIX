#ifndef FIFO_H
#define FIFO_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <cerrno>
#include <cstring>
#include <clocale>

namespace posix{

class FifoBase {
protected:
    int fifoFd;
    bool Open(const char *pathname, const int flags)
    {
        if (this->fifoFd >= 0)
            return false;

        if (mkfifo(pathname, 0666) < 0) {
            std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
            if (errno != 17)
                return false;
        }

        this->fifoFd = open(pathname, flags);
        if (this->fifoFd < 0) {
            this->fifoFd = -1;
            std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
            return false;
        }

        return true;        
    }

public:
    FifoBase() :
        fifoFd(-1)
    {

    }
    
    ~FifoBase()
    {
        Close();
    }

    bool Close()
    {
        if (this->fifoFd < 0)
            return false;

        if (close(this->fifoFd) == 0) {
            this->fifoFd = -1;
            return true;
        }
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        return false;
    }
};

class FifoClient : public FifoBase{
public:
    bool Open(const char *pathname)
    {
        return FifoBase::Open(pathname, O_WRONLY | O_NONBLOCK);
    }

    bool Write(const void *buf, size_t count, ssize_t *sendLen)
    {
        if (this->fifoFd < 0)
            return false;

        if ((buf == nullptr) || sendLen == nullptr)
            return false;

        *sendLen = write(this->fifoFd, buf, count);
        if (*sendLen > 0)
            return true;
        std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        return false;
    }
};

class FifoServer : public FifoBase{
public:
    bool Open(const char *pathname)
    {
        return FifoBase::Open(pathname, O_RDONLY | O_NONBLOCK);
    }

    bool Read(void *buf, size_t count, ssize_t *recvLen)
    {
        if (this->fifoFd < 0)
            return false;

        if ((buf == nullptr) || recvLen == nullptr)
            return false;

        *recvLen = read(this->fifoFd, buf, count);
        if (*recvLen > 0) {
            return true;
        }else
        if (*recvLen < 0) {
            std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
        }
        return false;
    }
};

}

#endif /* FIFO_H */