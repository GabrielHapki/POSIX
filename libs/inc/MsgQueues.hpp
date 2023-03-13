#ifndef MESSAGEQUEUES_H
#define MESSAGEQUEUES_H

#include <mqueue.h>
#include<string>

namespace posix{

class MsgQueuesBase {
protected:
    mqd_t mqfd;

public:
    enum classType { SENDER, RECEIVER };

    MsgQueuesBase(const std::string &key, const classType &type);
    ~MsgQueuesBase(void);
};

class MsgQueuesSend : public MsgQueuesBase{
public:
    MsgQueuesSend(const std::string &key);
    ~MsgQueuesSend(void);

    bool send(const void * buffer, const size_t size, const unsigned int priority);
};

class MsgQueuesReceive : public MsgQueuesBase{
private:
    std::string mKey;

    bool eventSetup();

public:
    MsgQueuesReceive(const std::string &key, const bool &event);
    ~MsgQueuesReceive();
    bool receive(void * buffer, const size_t size, ssize_t *recLen, unsigned int *priority);
    static void eventThread(union sigval sv)
    {
        MsgQueuesReceive *mq = (MsgQueuesReceive *)sv.sival_ptr;
        mq->eventSetup();
        mq->event();
        pthread_exit(NULL);
    }
    virtual void event() {}
};

}

#endif /* MESSAGEQUEUES_H */
