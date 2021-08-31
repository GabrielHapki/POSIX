#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <iostream>
#include <chrono>
#include <vector>

enum EventType { MORE_THAN, LESS_THAN};

class EventBase {
    protected:
        enum States { WAIT_EVENT, COUNT_EVENT, STOP_EVENT };
        States state;

        std::chrono::time_point<std::chrono::steady_clock> begin;
        std::chrono::time_point<std::chrono::steady_clock> end;

        EventType eventType;
        double timeout;
        void (*functionPointer)();
        EventBase *depend;

        bool getDepend();

    public:
        EventBase();
        ~EventBase();

        virtual bool moreOrLessThan() = 0;
        void check();
        bool get();
        void reset();
        void setLink(EventBase *vDep);
};

template <typename T1>
class Event : public EventBase {
    private:
        T1 &value;
        const T1 limit;

    public:
        Event(T1 &variable, const EventType eType, const T1 uLimit, const double uTimeout, const bool init, const void *fPointer) :
        value(variable), limit(uLimit)
        {
            eventType = eType;
            timeout = uTimeout;
            if (init)
                state = STOP_EVENT;
            functionPointer = (void (*)())fPointer;
        }
        ~Event() {}

        bool moreOrLessThan() override
        {
            switch(eventType) {
            case MORE_THAN:
                if (value > limit)
                    return true;
                break;
            case LESS_THAN:
                if (value < limit)
                    return true;
                break;
            }
            return false;
        }
};

class EventList {
    private:
        std::vector<EventBase*> eventList;

    public:
        EventList(const std::vector<EventBase*> eList);
        ~EventList();

    void run();
};

#endif /* EVENTS_H */