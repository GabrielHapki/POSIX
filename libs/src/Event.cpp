#include "Event.hpp"

bool EventBase::hasDependent()
{
    if (depend != nullptr) {
        if (depend->state == Events::BLOCKED)
            return true;
        else
            return false;
    }
    return true;            
}

EventBase::EventBase(const Events::Compare pCmp, const Events::Times &pCfg):
    status(false),
    state(Events::IDLE),
    varCompare(pCmp),
    timeConfig(pCfg),
    depend(nullptr)
{

}

EventBase::~EventBase()
{

}

void EventBase::run()
{
    switch (state) {
    case Events::IDLE:
        if (compareLogic() && hasDependent()) {
            if (timeConfig.debounce != 0.f) {
                timer.start();
                state = Events::DEBOUNCE;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> DEBOUNCE" << std::endl;
#endif
            } else {
                state = Events::EXECUTE;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> EXECUTE" << std::endl;
#endif
            }
        }
        break;
    case Events::DEBOUNCE:
        if (compareLogic() && hasDependent()) {
            if (timer.get() >= timeConfig.debounce) {
                state = Events::EXECUTE;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> EXECUTE" << std::endl;
#endif
            }
        } else {
            state = Events::IDLE;
#ifdef GENERIC_EVENT_DEBUG
            std::cout << name << " -> IDLE" << std::endl;
#endif
        }
        break;
    case Events::EXECUTE:
        status = true;
        callback();
        timer.start();
        state = Events::COOLDOWN;
#ifdef GENERIC_EVENT_DEBUG
        std::cout << name << " -> COOLDOWN" << std::endl;
#endif
        break;
    case Events::COOLDOWN:
        {
        if (timer.get() >= timeConfig.cooldown) {
            if (depend != nullptr) {
                depend->reset();
                state = Events::BLOCKED;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> BLOCKED" << std::endl;
#endif
            } else {
                state = Events::IDLE;
#ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> IDLE" << std::endl;
#endif
            }
        }
        }
        break;
    case Events::BLOCKED:
        break;
    }
}

void EventBase::reset()
{
    if (state != Events::IDLE) {
        state = Events::IDLE;
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
    if ((state == Events::IDLE) && (depend->state == Events::IDLE)) {
        depend->state = Events::BLOCKED;
    }
}

Events::States EventBase::getState()
{
    return state;
}

EventList::EventList(const std::vector<EventBase*> eList) : eventList(eList)
{

}

EventList::~EventList()
{

}

void EventList::run()
{
    for (auto event : eventList) {
        event->run();
    }
}