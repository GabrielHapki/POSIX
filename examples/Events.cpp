#include "Event.hpp"
#include <unistd.h>
#include <stdio.h>
#include <map>

std::map<std::string, Events::Times> eventConfig { //Sets the default values for the eventConfig map, these values will be replaced by the loadConfig()
    {Events::BATTERY_LOW,       {3.f, 3.f}},
    {Events::BATTERY_RESTORED,  {3.f, 3.f}},
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

int main(int argc, char** argv) {
    int batLvl = 0; //adcCtrl.getBatteryPercent();
    EventBatRes batRes(batLvl, Events::MORE_THAN, 70, eventConfig[Events::BATTERY_RESTORED], Events::IDLE, Events::BATTERY_RESTORED);
    EventBatLow batLow(batLvl, Events::LESS_THAN, 30, eventConfig[Events::BATTERY_LOW], Events::BLOCKED, Events::BATTERY_LOW);
    batRes.setDependent(&batLow);
    batLow.setDependent(&batRes);

    EventList events({&batLow, &batRes});

    printf("\n");
    for (batLvl=66; batLvl<100; batLvl+=5) {
        events.run();
        printf("batLvl = %d, batteryRestored = %d, lowBattery = %d \n", batLvl, batRes.getState(), batLow.getState());
        sleep(1);
    }
    for (batLvl=70; batLvl>0; batLvl-=5) {
        events.run();
        printf("batLvl = %d, batteryRestored = %d, lowBattery = %d \n", batLvl, batRes.getState(), batLow.getState());
        sleep(1);
    }

    return 0;
}