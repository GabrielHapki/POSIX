#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <semaphore.h>

namespace posix{

class Semaphore {
    private:
        sem_t *sem;
    public:
        Semaphore(void);
        ~Semaphore(void);
        bool open(const char *name, int oflag, mode_t perms, unsigned int value);
        bool unlink(const char *name);
        bool close(void);
};

}

#endif /* SEMAPHORE_H */