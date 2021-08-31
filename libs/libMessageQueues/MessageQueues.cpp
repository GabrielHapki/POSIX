#include "MessageQueues.hpp"
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MSGSZ     128

struct msgBuff {
    long    type;
    char    data[MSGSZ];
};

MsgQueuesBase::MsgQueuesBase() :
    messageQueueId(-1)
{
    std::cout << "MessageQueuesBase constructor" << std::endl;
}

MsgQueuesBase::~MsgQueuesBase(void)
{
    std::cout << "MessageQueuesBase destructor" << std::endl;
}

key_t MsgQueuesBase::getKey(MessageQueue::key_Type keyType)
{
    std::string pathName;

    switch(keyType) {
    case MessageQueue::SOUNDPLAYER: pathName = "SoundPlayer";
        break;
    }

    return ftok(pathName.c_str(), 'a');
}

MsgQueuesSend::MsgQueuesSend(MessageQueue::key_Type keyType)
{
    key_t mKey = getKey(keyType);
    if ((messageQueueId = msgget(mKey, IPC_CREAT | 0666 )) < 0) {
        std::cout << "MsgQueuesSend Error: Cannot get an Id!" << std::endl;
    }
    struct msqid_ds msg_settings;
    int ret = msgctl(messageQueueId, IPC_STAT, &msg_settings);
    msg_settings.msg_qbytes = 32767;
    msgctl( messageQueueId, IPC_SET, &msg_settings);
}

MsgQueuesSend::~MsgQueuesSend(void)
{
    std::cout << "MsgQueuesSend destructor" << std::endl;
}

bool MsgQueuesSend::send(const long Type, const void * Buffer, size_t Size)
{
    errno = 0;
    msgBuff sbuf;
    sbuf.type = Type;
    if (Size <= MSGSZ) {
        memcpy(sbuf.data, Buffer, Size);
        if (msgsnd(messageQueueId, &sbuf, Size+sizeof(Type), IPC_NOWAIT) < 0) {
            if(errno != 0) {
                printf("MsgQueuesSend::Send Error: %d!\n", errno);
            }
            return false;
        }
        std::cout << "MsgQueuesSend sending " << Size+sizeof(long) << " bytes" << std::endl;
        return true;
    }
    return false;
}

MsgQueuesReceive::MsgQueuesReceive(MessageQueue::key_Type keyType)
{
    key_t mKey = getKey(keyType);
    std::cout << "MsgQueuesReceive constructor, key = " << static_cast<unsigned int>(mKey) << std::endl;
    if ((messageQueueId = msgget(mKey, 0666 )) < 0) {
        std::cout << "MsgQueuesReceive Error: Cannot get an Id!" << std::endl;
    }
}

MsgQueuesReceive::~MsgQueuesReceive(void)
{
    std::cout << "MsgQueuesReceive destructor" << std::endl;
}

bool MsgQueuesReceive::receive(const long Type, void * Buffer, size_t Size, ssize_t *RecLen)
{
    errno = 0;
    msgBuff rbuf;
    *RecLen = msgrcv(messageQueueId, &rbuf, Size+sizeof(Type), Type, IPC_NOWAIT);
    if (*RecLen < 0) {
        if(errno != 0) {
//            printf("MsgQueuesReceive::Receive Error: %d!\n", errno);
        }
        return false;
    }
    memcpy(Buffer, rbuf.data, Size);
    std::cout << "MsgQueuesReceive receiving " << *RecLen << " bytes" << std::endl;
    return true;
}