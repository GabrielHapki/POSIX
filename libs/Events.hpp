#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <Timer.hpp>

//#define GENERIC_EVENT_DEBUG

namespace Events
{
    const std::string AC_CONNECT =          "ac_connect";
    const std::string AC_DISCONNECT =       "ac_disconnect";
    const std::string BATTERY_LOW =         "battery_low";
    const std::string BATTERY_RESTORED =    "battery_restored";

    enum Compare { MORE_THAN, LESS_THAN };

    struct Times {
        float debounce;
        float cooldown;
    };

    enum States { IDLE, DEBOUNCE, EXECUTE, COOLDOWN, BLOCKED };
}

/**
 * Generic class implementation of an event.
 * An event happens when an input changes.
 * A debounce is used to check if the input is stable after some amount of time.
 * A cooldown defines a period where the event will not be checked.
 * The event can be a single object or can be a pair of mutual dependents objects.
 */
class EventBase {
    protected:
        bool status;
        Events::States state;
        const Events::Compare varCompare;
        const Events::Times &timeConfig;
        EventBase *depend;
        posix::Timer timer;
#ifdef GENERIC_EVENT_DEBUG
        std::string name;
#endif

        /**
         * This function checks if the reference variable is more/less than the referece value.
         *
         * @return A boolean value (true = valid comparasion, false = invalid comparasion).
         */
        virtual bool compareLogic() = 0;

        /**
         * This function returns if there is a dependent event.
         *
         * @return A boolean value (true = has dependent, false = have not).
         */
        bool hasDependent()
        {
            if (depend != nullptr) {
                if (depend->state == Events::BLOCKED)
                    return true;
                else
                    return false;
            }
            return true;            
        }

    public:
        EventBase(const Events::Compare pCmp, const Events::Times &pCfg):
            state(Events::IDLE),
            status(false),
            varCompare(pCmp),
            timeConfig(pCfg),
            depend(nullptr)
        {

        }

        ~EventBase()
        {

        }

        /**
         * This function is an event state machine.
         */
        void run()
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

        /**
         * This function resets the state machine.
         */
        void reset()
        {
            if (state != Events::IDLE) {
                state = Events::IDLE;
        #ifdef GENERIC_EVENT_DEBUG
                std::cout << name << " -> IDLE (by reset)" << std::endl;
        #endif
            }            
        }

        /**
         * This function returns if an event happened and reset the flag.
         */
        bool event()
        {
            if (status) {
                bool tmp = status;
                status = false;
                return tmp;
            }
            return false;
        }

        /**
         * This function sets another event as a dependent.
         * It is necessary to define mutual dependence.
         */
        void setDependent(EventBase *vDep)
        {
            depend = vDep;
            if ((state == Events::IDLE) && (depend->state == Events::IDLE)) {
                depend->state = Events::BLOCKED;
            }
        }

        /**
         * This function returns the current state.
         */
        Events::States getState()
        {
            return state;
        }

        /**
         * This function will be called in the EXECUTE state.
         * It can be overridden when the class Event is inherited to add specific code.
         */
        virtual void callback() {}
};

/**
 * This extends the class EventBase to support multiple types through Template.
 */
template <typename T1>
class Event : public EventBase {
    private:
        T1 &variable;
        const T1 reference;

    public:
        /**
         * Event constructor.
         *
         * @param pVar A reference variable to be compared.
         * @param pCmp Comparison type (more/less than).
         * @param pRef Reference value to be compared.
         * @param pCfg Configuration struct for debounce and cooldown.
         * @param fPtr Function pointer to be called after the debouncing.
         * @param pInit Initial value (idle/block).
         * @param pName Name of the object (only for debug).
         */
        Event(T1 &pVar, const Events::Compare pCmp, const T1 pRef, const Events::Times &pCfg, const Events::States pInit, const std::string pName) :
            EventBase(pCmp, pCfg),
            variable(pVar), reference(pRef)
        {
            if (pInit != Events::IDLE)
                state = Events::BLOCKED;
#ifdef GENERIC_EVENT_DEBUG
            name = pName;
            std::cout << std::endl;
            std::cout << name << " Init = " << (pInit == Events::IDLE ? "IDLE" : "BLOCK") << std::endl;
            std::cout << name << " Debounce = " << std::fixed << std::setprecision(3) << timeConfig.debounce << std::endl;
            std::cout << name << " Cooldown = " << std::fixed << std::setprecision(3) << timeConfig.cooldown << std::endl;
#endif
        }
        
        virtual ~Event() {}

        /**
         * This function checks if the reference variable is more/less than the referece value.
         *
         * @return A boolean value (true = valid comparasion, false = invalid comparasion).
         */
        bool compareLogic() override
        {
            switch(varCompare) {
            case Events::MORE_THAN:
                if (variable > reference)
                    return true;
                break;
            case Events::LESS_THAN:
                if (variable < reference)
                    return true;
                break;
            }
            return false;
        }
};

/**
 * This class is a list of events to be executed inside a loop.
 */
class EventList {
    private:
        std::vector<EventBase*> eventList;

    public:
        /**
         * EventList constructor
         *
         * @param eList List of events pointers
         */
        EventList(const std::vector<EventBase*> eList)
        {

        }

        ~EventList()
        {

        }

    /**
     * This function executes all events in the list.
     */
    void run()
    {
        for (auto event : eventList) {
            event->run();
        }
    }
};

#endif /* EVENTS_H */