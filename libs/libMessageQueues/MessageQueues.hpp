#ifndef MESSAGEQUEUES_H
#define MESSAGEQUEUES_H

#include <stdint.h>
#include <netinet/in.h>

class MessageQueues {
private:
    int messageQueueIdSnd;
    int messageQueueIdRcv;
    key_t mKey;

public:
    MessageQueues(key_t key);
    ~MessageQueues(void);
    bool Send(const void * Buffer, size_t Size);
    bool Receive(void * Buffer, size_t Size, int32_t *RecLen);
};

#endif /* MESSAGEQUEUES_H */