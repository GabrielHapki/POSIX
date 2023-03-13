#include "Pid.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

namespace posix{

Process::Process()
{

}

Process::~Process()
{

}

Process::pid_t Process::store(const Process::pid_t pid)
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

    return MY_PID;
}

Process::pid_t Process::get(const Process::pid_t pid)
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

    return MY_PID;
}

bool Process::isRunning(const Process::pid_t pid)
{
    struct stat statBuf;
    std::string pathName = "/proc/" + std::to_string(get(pid));

    if (stat(pathName.c_str(), &statBuf) == 0)
        return true;

    return false;
}

}
