#include "Events.hpp"

// Class EventBase

EventBase::EventBase(const Events::Compare pCmp, const Events::Times &pCfg) :
    state(IDLE),
    status(false),
    varCompare(pCmp),
    timeConfig(pCfg),
    depend(nullptr) {}

EventBase::~EventBase() {}

bool EventBase::hasDependent()
{
    if (depend != nullptr) {
        if (depend->state == BLOCKED)
            return true;
        else
            return false;
    }
    return true;
}

void EventBase::run()
{
    switch (state) {
    case IDLE:
        if (compareLogic() && hasDependent()) {
            if (timeConfig.debounce != 0.f) {
                timer.start();
                state = DEBOUNCE;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> DEBOUNCE" << std::endl;
#endif
            } else {
                state = EXECUTE;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> EXECUTE" << std::endl;
#endif
            }
        }
        break;
    case DEBOUNCE:
        if (compareLogic() && hasDependent()) {
            if (timer.getTime() >= timeConfig.debounce) {
                state = EXECUTE;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> EXECUTE" << std::endl;
#endif
            }
        } else {
            state = IDLE;
#ifdef GENERIC_EVENT_DEBUG
            std::cout << name << " -> IDLE" << std::endl;
#endif
        }
        break;
    case EXECUTE:
        status = true;
        callback();
        timer.start();
        state = COOLDOWN;
#ifdef GENERIC_EVENT_DEBUG
        std::cout << name << " -> COOLDOWN" << std::endl;
#endif
        break;
    case COOLDOWN:
        {
        if (timer.getTime() >= timeConfig.cooldown) {
            if (depend != nullptr) {
                depend->reset();
                state = BLOCKED;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> BLOCKED" << std::endl;
#endif
            } else {
                state = IDLE;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> IDLE" << std::endl;
#endif
            }
        }
        }
        break;
    case BLOCKED:
        break;
    }
}

void EventBase::reset()
{
    if (state != IDLE) {
        state = IDLE;
#ifdef GENERIC_EVENT_DEBUG
        std::cout << name << " -> IDLE (by reset)" << std::endl;
#endif
    }
}

bool EventBase::event()
{
    if (status) {
        bool tmp = status;
        status = false;
        return tmp;
    }
    return false;
}

void EventBase::setDependent(EventBase *vDep)
{
    depend = vDep;
    if ((state == IDLE) && (depend->state == IDLE)) {
        depend->state = BLOCKED;
    }
}

// Class Event

EventList::EventList(const std::vector<EventBase*> eList) : eventList(eList) { }

EventList::~EventList() {}

void EventList::run()
{
    for (auto event : eventList) {
        event->run();
    }
}
