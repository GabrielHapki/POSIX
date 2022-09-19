#ifndef MESSAGEQUEUES_H
#define MESSAGEQUEUES_H

#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>
#include <sys/stat.h>

#include <stdexcept>

#include <iostream>
#include <cerrno>
#include <cstring>
#include <clocale>

namespace posix{

class MsgQueuesBase {
protected:
    mqd_t mqfd;
    std::string name;

public:
    MsgQueuesBase() :
        mqfd(-1)
    {

    }

    virtual ~MsgQueuesBase(void)
    {

    }
};

class MsgQueuesSend : public MsgQueuesBase{
public:
    MsgQueuesSend(const char *key)
    {
        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = 32;
        attr.mq_curmsgs = 0;
        mqfd = mq_open(key, O_WRONLY | O_CREAT, 0666, &attr);
        if (mqfd < 0) {
            throw("mq_open failed: " + errno);
        } else {
            auto ret = mq_getattr(mqfd, &attr);
            if (ret < 0) {
                throw("mq_getattr failed: " + errno);
            }
        }
    }

    ~MsgQueuesSend(void) override
    {
        if (mq_close(mqfd) == -1)
            perror("mq_close failure on mqfd");
    }

    bool send(const void * buffer, size_t size, const unsigned int priority)
    {
        if (mqfd > 0) {
            int status = mq_send(mqfd, (const char *)buffer, size, priority);
            if (status < 0) {
                perror("mq_send failure on mqfd");
            } else {
                return true;
            }
        }
        return false;        
    }
};

class MsgQueuesReceive : public MsgQueuesBase{
public:
    MsgQueuesReceive(const char *key)
    {
        name = key;
        struct mq_attr attr = {0};
        attr.mq_flags = 0;
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = 32;
        attr.mq_curmsgs = 0;
        mqfd = mq_open(key, O_RDONLY | O_CREAT | O_NONBLOCK, 0666, &attr);
        if (mqfd < 0) {
            throw("mq_open failed: " + errno);
        } else {
            auto ret = mq_getattr(mqfd, &attr);
            if (ret < 0) {
                throw("mq_getattr failed: " + errno);
            }
        }
    }

    ~MsgQueuesReceive(void) override
    {
        if (mq_close(mqfd) == -1)
            perror("mq_close failure on mqfd");
        if (mq_unlink(name.c_str()) == -1)
            perror("mq_unlink failure in test_ipc");
    }

    bool receive(void * buffer, size_t size, ssize_t *recLen, unsigned int *priority)
    {
        if (mqfd > 0) {
            *recLen = mq_receive(mqfd, (char *)buffer, size, priority);
            if (*recLen < 0) {
                perror("mq_receive failure on mqfd");
                std::cerr << "Error(" << errno << "): " << std::strerror(errno) << std::endl;
            } else {
                return true;
            }
        }
        return false;
    }
};

}

#endif /* MESSAGEQUEUES_H */