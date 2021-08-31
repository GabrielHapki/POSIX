#include "Events.hpp"

// Class EventBase

EventBase::EventBase() : state(WAIT_EVENT), depend(nullptr) {}

EventBase::~EventBase() {}

bool EventBase::getDepend()
{
    if (depend != nullptr) {
        return depend->get();
    }
    return true;
}

void EventBase::check()
{
    switch (state) {
    case WAIT_EVENT:
        if (moreOrLessThan() && getDepend()) {
            begin = std::chrono::steady_clock::now();
            state = COUNT_EVENT;
        }
        break;
    case COUNT_EVENT:
        if (moreOrLessThan() && getDepend()) {
            std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - begin;
            if (elapsed_seconds.count() > timeout) {
                state = STOP_EVENT;
                functionPointer();
                if (depend != nullptr) {
                    depend->reset();
                }
            }
        } else {
            state = WAIT_EVENT;
        }
        break;
    case STOP_EVENT:
        break;
    }
}

bool EventBase::get()
{
    if (state == STOP_EVENT)
        return true;
    return false;
}

void EventBase::reset() {
    state = WAIT_EVENT;
}

void EventBase::setLink(EventBase *vDep){
    this->depend = vDep;
}

// Class Event

EventList::EventList(const std::vector<EventBase*> eList)
{
    this->eventList = eList;
}

EventList::~EventList() {}

void EventList::run()
{
    for (auto event : eventList) {
        event->check();
    }
}
