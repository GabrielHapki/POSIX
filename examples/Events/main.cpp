#include "Events.hpp"
#include <unistd.h>
#include <stdio.h>
#include <map>

std::map<std::string, Events::Times> eventConfig { //Sets the default values for the eventConfig map, these values will be replaced by the loadConfig()
    {Events::AC_CONNECT,        {3.f, 0.f}},
    {Events::AC_DISCONNECT,     {3.f, 0.f}},
    {Events::BATTERY_LOW,       {3.f, 9.f}},
    {Events::BATTERY_RESTORED,  {3.f, 9.f}},
};

class EventAcCon : public Event<bool>{
    public:
        EventAcCon(bool &pVar, const Events::Compare pCmp, const bool pRef, const Events::Times &pCfg, const Events::States pInit, const std::string pName) :
        Event(pVar, pCmp, pRef, pCfg, pInit, pName) {}
        void callback()
        {
            printf("# Ac Connected #\n");
        }
};

class EventAcDis : public Event<bool>{
    public:
        EventAcDis(bool &pVar, const Events::Compare pCmp, const bool pRef, const Events::Times &pCfg, const Events::States pInit, const std::string pName) :
        Event(pVar, pCmp, pRef, pCfg, pInit, pName) {}
        void callback()
        {
            printf("# Ac Disconnected #\n");
        }
};

class EventBatLow : public Event<int>{
    public:
        EventBatLow(int &pVar, const Events::Compare pCmp, const int pRef, const Events::Times &pCfg, const Events::States pInit, const std::string pName) :
        Event(pVar, pCmp, pRef, pCfg, pInit, pName) {}
        void callback()
        {
            printf("# Low Battery #\n");
        }
};

class EventBatRes : public Event<int>{
    public:
        EventBatRes(int &pVar, const Events::Compare pCmp, const int pRef, const Events::Times &pCfg, const Events::States pInit, const std::string pName) :
        Event(pVar, pCmp, pRef, pCfg, pInit, pName) {}
        void callback()
        {
            printf("# Battery restored #\n");
        }
};

void sendShutdown() {
    printf("# Shutdown #\n");
}

int main(int argc, char** argv) {
    Events::States aInit = Events::IDLE, bInit = Events::IDLE;

    bool acStatus = false; // = adcCtrl.getAc();
    if (acStatus) {             // if AC Connected
        aInit = Events::BLOCKED;  // then set AcDisconnect as idle, and set AcConnect as block
    } else {                    // if AC Disconnected
        bInit = Events::BLOCKED;  // then set AcConnect as idle, and set AcDisconnect as block
    }
    EventAcCon acCon(acStatus, Events::MORE_THAN, 0, eventConfig[Events::AC_CONNECT], aInit, Events::AC_CONNECT);
    EventAcDis acDis(acStatus, Events::LESS_THAN, 1, eventConfig[Events::AC_DISCONNECT], bInit, Events::AC_DISCONNECT);
    acCon.setDependent(&acDis);
    acDis.setDependent(&acCon);

    aInit = bInit = Events::IDLE;

    int batLvl = 0; //adcCtrl.getBatteryPercent();
    if (batLvl > 30) {          // if battery level more than 30%
        aInit = Events::BLOCKED;  // then set batLow as idle, and set batRes as block
    } else {                    // if AC Disconnected
        bInit = Events::BLOCKED;  // then set batRes as idle, and set batLow as block
    }
    EventBatRes batRes(batLvl, Events::MORE_THAN, 70, eventConfig[Events::BATTERY_RESTORED], aInit, Events::BATTERY_RESTORED);
    EventBatLow batLow(batLvl, Events::LESS_THAN, 30, eventConfig[Events::BATTERY_LOW], bInit, Events::BATTERY_LOW);
    batRes.setDependent(&batLow);
    batLow.setDependent(&batRes);

    EventList events({&acCon, &acDis, &batLow, &batRes});

    for (batLvl=0; batLvl<100; batLvl+=3) {
        acStatus = (batLvl > 50) ? true : false;
        events.run();
        printf("Count = %d, bool = %d, ", batLvl, acStatus);
        printf("acConnect = %d, acDisconnect = %d, ", acCon.getState(), acDis.getState());
        printf("batteryRestored = %d, lowBattery = %d \n", batRes.getState(), batLow.getState());
        usleep(500000);
    }
    for (batLvl=100; batLvl>0; batLvl-=3) {
        acStatus = (batLvl > 50) ? true : false;
        events.run();
        printf("Count = %d, bool = %d, ", batLvl, acStatus);
        printf("acConnect = %d, acDisconnect = %d, ", acCon.getState(), acDis.getState());
        printf("batteryRestored = %d, lowBattery = %d \n", batRes.getState(), batLow.getState());
        usleep(500000);
    }

    return 0;
}