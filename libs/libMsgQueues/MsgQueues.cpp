#include "MsgQueues.hpp"
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

namespace posix{

MsgQueuesBase::MsgQueuesBase() :
    mqfd(-1)
{
    std::cout << "MessageQueuesBase constructor" << std::endl;
}

MsgQueuesBase::~MsgQueuesBase(void)
{
    std::cout << "MessageQueuesBase destructor" << std::endl;
}

std::string MsgQueuesBase::getKey(const MsgQueue::key key)
{
    switch(key) {
    case MsgQueue::SOUNDPLAYER: return "/SoundPlayer_MQ";
        break;
    default: return "fail";
        break;
    }
}

MsgQueuesSend::MsgQueuesSend(const MsgQueue::key key)
{
    name = getKey(key);
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;
    mqfd = mq_open(name.c_str(), O_WRONLY | O_CREAT, 0666, &attr);
    if (mqfd == -1) {
        perror("mq_open failure from main");
    } else {
        std::cout << "MsgQueuesSend constructor mqfd = " << mqfd << std::endl;
        mq_getattr(mqfd, &attr);
    }
}

MsgQueuesSend::~MsgQueuesSend(void)
{
    std::cout << "MsgQueuesSend destructor" << std::endl;
    if (mq_close(mqfd) == -1)
        perror("mq_close failure on mqfd");
}

bool MsgQueuesSend::send(const void * buffer, size_t size, const unsigned int priority)
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

MsgQueuesReceive::MsgQueuesReceive(const MsgQueue::key key)
{
    name = getKey(key);
    struct mq_attr attr = {0};
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;
    mqfd = mq_open(name.c_str(), O_RDONLY | O_CREAT | O_NONBLOCK, 0666, &attr);
    if (mqfd == -1) {
        perror("mq_open failure from main");
    } else {
        std::cout << "MsgQueuesReceive constructor mqfd = " << mqfd << std::endl;
        mq_getattr(mqfd, &attr);
    }
}

MsgQueuesReceive::~MsgQueuesReceive(void)
{
    std::cout << "MsgQueuesReceive destructor" << std::endl;
    if (mq_close(mqfd) == -1)
        perror("mq_close failure on mqfd");
    if (mq_unlink(name.c_str()) == -1)
        perror("mq_unlink failure in test_ipc");
}

bool MsgQueuesReceive::receive(void * buffer, size_t size, ssize_t *recLen, unsigned int *priority)
{
    if (mqfd > 0) {
        *recLen = mq_receive(mqfd, (char *)buffer, size, priority);
        if (*recLen < 0) {
            //perror("mq_receive failure on mqfd");
        } else {
            return true;
        }
    }
    return false;
}

}