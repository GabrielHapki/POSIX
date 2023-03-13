#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

namespace posix{

class Process
{
    public:
        Process();
        ~Process();

        enum pid_t {
            MY_PID = 0,
            ANOTHER_PID,
        };

        pid_t store(const pid_t pid);
        pid_t get(const pid_t pid);
        bool isRunning(const pid_t pid);
};

}

#endif //PROCESS_H_INCLUDED