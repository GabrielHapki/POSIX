#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <sys/types.h>

class Process
{
    public:
        Process() {}
        ~Process() {}

        enum Pid {
            MY_PID = 0,
            ANOTHER_PID,
        };

        static pid_t store(const Pid pid);
        static pid_t get(const Pid pid);
        static bool isRunning(const Pid pid);
};

#endif //PROCESS_H_INCLUDED