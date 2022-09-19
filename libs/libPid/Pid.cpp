#include "Pid.hpp"

#include <sys/stat.h>
#include <unistd.h>

#include <string>

pid_t Process::store(const Pid pid)
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

pid_t Process::get(const Pid pid)
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

bool Process::isRunning(const Pid pid)
{
    struct stat statBuf;
    std::string pathName = "/proc/" + std::to_string(get(pid));

    if (stat(pathName.c_str(), &statBuf) == 0)
        return true;

    return false;
}