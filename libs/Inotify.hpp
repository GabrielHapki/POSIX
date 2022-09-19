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

        Inotify(const std::string &pathname, const uint32_t &mask) :
            fd(-1),
            wd(-1),
            bufsize(MAX_INPUT),
            event(NULL)
        {
            fd = inotify_init();
            if (fd < 0)
                perror("inotify_init");
            else {
                wd = inotify_add_watch(fd, pathname.c_str(), mask);
                event = new inotify_event[bufsize];
            }            
        }

        ~Inotify()
        {
            inotify_rm_watch(fd, wd);            
        }

        Event getEvent()
        {
            Event temp({0});
            auto length = read(fd, event, bufsize);
            if (length > 0) {
                if (event->wd == wd) {
                    temp.mask = event->mask;
                    temp.cookie = event->cookie;
                    if (event->len > 0) {
                        temp.name = std::string(event->name);
                    }
                }
            }
            return temp;            
        }

    private:
        int fd;
        int wd;
        const size_t bufsize;
        inotify_event *event;
};

}

#endif /* INOTIFY_H */