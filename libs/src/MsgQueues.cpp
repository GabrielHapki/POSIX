#include "MsgQueues.hpp"
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>

#include <stdexcept>

#include <iostream>
#include <cerrno>
#include <cstring>
#include <clocale>

namespace posix{

MsgQueuesBase::MsgQueuesBase(const std::string &key, const classType &type) :
    mqfd(-1)
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;
    if (type == SENDER) {
        mqfd = mq_open(key.c_str(), O_WRONLY | O_CREAT, 0666, &attr);
    } else {
        mqfd = mq_open(key.c_str(), O_RDONLY | O_CREAT | O_NONBLOCK, 0666, &attr);
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

MsgQueuesBase::~MsgQueuesBase(void)
{
    if (mq_close(mqfd) < 0)
        std::cerr << "mq_close Error:" << std::to_string(errno) +  ", " << std::strerror(errno) << std::endl;
}

MsgQueuesSend::MsgQueuesSend(const std::string &key) :
    MsgQueuesBase(key, SENDER)
{

}

MsgQueuesSend::~MsgQueuesSend(void)
{

}

bool MsgQueuesSend::send(const void * buffer, const size_t size, const unsigned int priority)
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

MsgQueuesReceive::MsgQueuesReceive(const std::string &key, const bool &event) :
    MsgQueuesBase(key, RECEIVER)
{
    mKey = key;
    if (event)
        eventSetup();
}

MsgQueuesReceive::~MsgQueuesReceive()
{
    if (mq_unlink((const char *)mKey.c_str()) < -0)
        std::cerr << "mq_unlink Error:" << std::to_string(errno) +  ", " << std::strerror(errno) << std::endl;
}

bool MsgQueuesReceive::eventSetup()
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

bool MsgQueuesReceive::receive(void * buffer, const size_t size, ssize_t *recLen, unsigned int *priority)
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

}
