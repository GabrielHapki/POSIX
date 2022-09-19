#include "Inotify.hpp"
#include <stdio.h>

int main(int argc, char** argv)
{
//    pdx::Inotify inotify("/home/gabriel/reccounter.log", IN_MODIFY);
    posix::Inotify inotify("/proc/", IN_ALL_EVENTS);
    posix::Inotify::Event event;

    while(1) {
        event = inotify.getEvent();
        printf("file %s, mask 0x%08X\n", event.name.c_str(), event.mask);
        usleep(10000);
    }

    return 0;
}