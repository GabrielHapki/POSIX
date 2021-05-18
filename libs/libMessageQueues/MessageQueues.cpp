#include "MessageQueues.hpp"
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

MessageQueues::MessageQueues(key_t key) :
    messageQueueIdSnd(-1),
    messageQueueIdRcv(-1),
    mKey(key)
{
    if ((messageQueueIdSnd = msgget(mKey, IPC_CREAT | 0666 )) < 0) {
        throw std::runtime_error("msgget sender");
    }
    if ((messageQueueIdRcv = msgget(mKey, 0666 )) < 0) {
        throw std::runtime_error("msgget receiver");
    }
}

MessageQueues::~MessageQueues(void) {
}

bool MessageQueues::Send(const void * Buffer, size_t Size)
{
    if (msgsnd(messageQueueIdSnd, Buffer, Size, IPC_NOWAIT) < 0) {
        throw std::runtime_error("msgsnd");
        return false;
    }
    return true;
}

bool MessageQueues::Receive(void * Buffer, size_t Size, int32_t *RecLen)
{
    *RecLen = msgrcv(messageQueueIdRcv, Buffer, Size, 0, 0);
    if (*RecLen < 0) {
        throw std::runtime_error("msgrcv");
        return false;
    }
    return true;
}