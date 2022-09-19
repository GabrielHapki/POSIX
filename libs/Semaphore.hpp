#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <semaphore.h>

namespace posix{

class Semaphore {
    private:
        sem_t *sem;
    public:
        Semaphore(void)
        {

        }

        ~Semaphore(void)
        {

        }

        bool open(const char *name, int oflag, mode_t perms, unsigned int value)
        {
            sem = sem_open(name, oflag, perms, value);
            return true;
        }

        bool unlink(const char *name)
        {
            sem_unlink(name);
            return true;
        }

        bool close(void)
        {
            sem_close(sem);
            return true;
        }
};

}

#endif /* SEMAPHORE_H */