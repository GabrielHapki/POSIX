#ifndef MESSAGEQUEUES_H
#define MESSAGEQUEUES_H

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
//#include <sys/rt_syscall.h>
#include <mqueue.h>
#include <errno.h>

#include <string>

namespace MsgQueue {
    enum key { SOUNDPLAYER };
}

class MsgQueuesBase {
protected:
    mqd_t mqfd;
    std::string name;

    std::string getKey(const MsgQueue::key key);

public:
    MsgQueuesBase();
    virtual ~MsgQueuesBase(void);
};

class MsgQueuesSend : public MsgQueuesBase{
public:
    MsgQueuesSend(const MsgQueue::key key);
    ~MsgQueuesSend(void) override;
    bool send(const void * buffer, size_t size, const unsigned int priority);
};

class MsgQueuesReceive : public MsgQueuesBase{
public:
    MsgQueuesReceive(const MsgQueue::key key);
    ~MsgQueuesReceive(void) override;
    bool receive(void * buffer, size_t size, ssize_t *recLen, unsigned int *priority);
};

#endif /* MESSAGEQUEUES_H */