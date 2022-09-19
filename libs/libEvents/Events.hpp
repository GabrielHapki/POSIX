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
        bool hasDependent();

    public:
        EventBase(const Events::Compare pCmp, const Events::Times &pCfg);
        ~EventBase();

        /**
         * This function is an event state machine.
         */
        void run();

        /**
         * This function resets the state machine.
         */
        void reset();

        /**
         * This function returns if an event happened and reset the flag.
         */
        bool event();

        /**
         * This function sets another event as a dependent.
         * It is necessary to define mutual dependence.
         */
        void setDependent(EventBase *vDep);

        /**
         * This function returns the current state.
         */
        Events::States getState();

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
        ~Event() {}

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
        EventList(const std::vector<EventBase*> eList);
        ~EventList();

    /**
     * This function executes all events in the list.
     */
    void run();
};

#endif /* EVENTS_H */