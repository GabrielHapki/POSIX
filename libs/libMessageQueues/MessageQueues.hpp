#ifndef MESSAGEQUEUES_H
#define MESSAGEQUEUES_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdint.h>
#include <netinet/in.h>

namespace MessageQueue {
    enum key_Type { SOUNDPLAYER };
}

class MsgQueuesBase {
protected:
    int messageQueueId;

public:
    MsgQueuesBase();
    virtual ~MsgQueuesBase(void);

    key_t getKey(MessageQueue::key_Type keyType);
};

class MsgQueuesSend : public MsgQueuesBase{
public:
    MsgQueuesSend(MessageQueue::key_Type keyType);
    ~MsgQueuesSend(void);
    bool send(const long Type, const void * Buffer, size_t Size);
};

class MsgQueuesReceive : public MsgQueuesBase{
public:
    MsgQueuesReceive(MessageQueue::key_Type keyType);
    ~MsgQueuesReceive(void);
    bool receive(const long Type, void * Buffer, size_t Size, ssize_t *RecLen);
};

#endif /* MESSAGEQUEUES_H */