#ifndef MESSAGEQUEUES_H
#define MESSAGEQUEUES_H

#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
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
    enum classType { SENDER, RECEIVER };

    MsgQueuesBase(const char *key, classType type) :
        mqfd(-1)
    {
        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = 32;
        attr.mq_curmsgs = 0;
        if (type == SENDER) {
            mqfd = mq_open(key, O_WRONLY | O_CREAT, 0666, &attr);
        } else {
            mqfd = mq_open(key, O_RDONLY | O_CREAT | O_NONBLOCK, 0666, &attr);
        }
        if (mqfd < 0) {
            std::runtime_error("mq_open error: " + std::to_string(errno) + ", " + std::strerror(errno));
        } else {
            auto ret = mq_getattr(mqfd, &attr);
            if (ret < 0) {
                throw std::runtime_error("mq_getattr error: "  + std::to_string(errno) + ", " + std::strerror(errno));
            }
        }
    }

    ~MsgQueuesBase(void)
    {
        if (mq_close(mqfd) < 0)
            std::cerr << "mq_close Error:" << std::to_string(errno) +  ", " << std::strerror(errno) << std::endl;
    }
};

class MsgQueuesSend : public MsgQueuesBase{
public:
    MsgQueuesSend(const char *key) :
        MsgQueuesBase(key, SENDER)
    {

    }

    ~MsgQueuesSend(void)
    {

    }

    bool send(const void * buffer, size_t size, const unsigned int priority)
    {
        if (mqfd > 0) {
            int status = mq_send(mqfd, (const char *)buffer, size, priority);
            if (status < 0) {
                std::cerr << "mq_send Error:" << std::to_string(errno) +  ", " << std::strerror(errno) << std::endl;
            } else {
                return true;
            }
        }
        return false;        
    }
};

class MsgQueuesReceive : public MsgQueuesBase{
private:
    bool eventSetup()
    {
        struct sigevent sev;
        sev.sigev_notify = SIGEV_THREAD;
        sev.sigev_notify_function = this->eventThread;
        sev.sigev_notify_attributes = NULL;
        sev.sigev_value.sival_ptr = this;

        if (mq_notify(mqfd, &sev) < 0) {
            std::cerr << "mq_notify Error:" << std::to_string(errno) +  ", " << std::strerror(errno) << std::endl;
            return false;
        }

        return true;
    }

public:
    MsgQueuesReceive(const char *key, const bool event) :
        MsgQueuesBase(key, RECEIVER)
    {
        if (event)
            eventSetup();
    }

    ~MsgQueuesReceive()
    {
        if (mq_unlink((const char *)name.c_str()) < -0)
            std::cerr << "mq_unlink Error:" << std::to_string(errno) +  ", " << std::strerror(errno) << std::endl;
    }

    bool receive(void * buffer, size_t size, ssize_t *recLen, unsigned int *priority)
    {
        if (mqfd > 0) {
            *recLen = mq_receive(mqfd, (char *)buffer, size, priority);
            if (*recLen < 0) {
                std::cerr << "mq_receive Error:" << std::to_string(errno) +  ", " << std::strerror(errno) << std::endl;
            } else {
                return true;
            }
        }
        return false;
    }

    static void eventThread(union sigval sv)
    {
        posix::MsgQueuesReceive *mq = (posix::MsgQueuesReceive *)sv.sival_ptr;
        mq->eventSetup();
        mq->event();
        pthread_exit(NULL);
    }

    virtual void event()
    {

    }
};

}

#endif /* MESSAGEQUEUES_H */
