#include "Semaphore.hpp"
#include <semaphore.h>

namespace posix{

Semaphore::Semaphore(void)
{

}

Semaphore::~Semaphore(void)
{

}

bool Semaphore::open(const char *name, int oflag, mode_t perms, unsigned int value)
{
    sem = sem_open(name, oflag, perms, value);
    return true;
}

bool Semaphore::unlink(const char *name)
{
    sem_unlink(name);
    return true;
}

bool Semaphore::close(void)
{
    sem_close(sem);
    return true;
}

}
