#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

namespace posix{

class Process
{
    public:
        Process()
        {

        }

        ~Process()
        {

        }

        enum Pid {
            MY_PID = 0,
            ANOTHER_PID,
        };

        static pid_t store(const Pid pid)
        {
            // auto mem = SharedMem::instance();
            // auto pidStatus = mem.getProcessStatus();
            // mem.readProcessStatus();

            //get the pid from the current process calling this function
            auto tmp = getpid();

            // switch(pid) {
            // case MY_PID: pidStatus->camd = tmp;
            //     break;
            // case ANOTHER_PID: pidStatus->cloud = tmp;
            //     break;
            // default:
            //     return 0;
            // }
            // mem.setProcessStatus();

            return tmp;
        }

        static pid_t get(const Pid pid)
        {
            // auto mem = SharedMem::instance();
            // auto pidStatus = mem.getProcessStatus();
            // mem.readProcessStatus();

            // switch(pid) {
            // case MY_PID:
            //     return pidStatus->camd;
            //     break;
            // case ANOTHER_PID:
            //     return pidStatus->cloud;
            //     break;
            // default:
            //     return 0;
            // }

            return 0;
        }

        static bool isRunning(const Pid pid)
        {
            struct stat statBuf;
            std::string pathName = "/proc/" + std::to_string(get(pid));

            if (stat(pathName.c_str(), &statBuf) == 0)
                return true;

            return false;
        }
};

}

#endif //PROCESS_H_INCLUDED