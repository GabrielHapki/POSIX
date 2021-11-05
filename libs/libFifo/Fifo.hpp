#ifndef FIFO_H
#define FIFO_H

#include <unistd.h>

class FifoBase {
protected:
    int fifoFd;
    bool Open(const char *pathname, const int flags);

public:
    FifoBase();
    ~FifoBase(void);
    bool Close();
};

class FifoClient : public FifoBase{
public:
    bool Open(const char *pathname);
    bool Write(const void *buf, size_t count, ssize_t *sendLen);
};

class FifoServer : public FifoBase{
public:
    bool Open(const char *pathname);
    bool Read(void *buf, size_t count, ssize_t *recvLen);
};

#endif /* FIFO_H */