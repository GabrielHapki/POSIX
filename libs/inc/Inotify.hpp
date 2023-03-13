#ifndef INOTIFY_H
#define INOTIFY_H

#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>

#include <string>

namespace posix{

class Inotify
{
    public:
        struct Event
        {
            uint32_t mask;	/* Watch mask.  */
            uint32_t cookie;	/* Cookie to synchronize two events.  */
            std::string name;	/* Name.  */
        };

        Inotify(const std::string &pathname, const uint32_t &mask);
        ~Inotify();

        Event getEvent();

    private:
        int fd;
        int wd;
        const size_t bufsize;
        inotify_event *event;
};

}

#endif /* INOTIFY_H */