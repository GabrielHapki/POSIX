#include "Events.hpp"
#include <unistd.h>
#include <stdio.h>

std::map<std::string, Events::Times> eventConfig { //Sets the default values for the eventConfig map, these values will be replaced by the loadConfig()
    {Events::AC_CONNECT,        {900.f, 0.f}},
    {Events::AC_DISCONNECT,     {900.f, 0.f}},
    {Events::BATTERY_LOW,       {9.f, 60.f}},
    {Events::BATTERY_RESTORED,  {9.f, 60.f}},
    {Events::MOVE_COMBINED,     {0.f, 60.f}},
    {Events::PIR,               {0.08f, 9.f}},
    {Events::RESET,             {0.1f, 30.f}},// No Cut-off filter
    {Events::RINGING,           {0.f, 30.f}}, // No Cut-off filter
    {Events::RINGING5S,         {5.f, 30.f}},
    {Events::SHUTDOWN,          {3.f, 30.f}},
    {Events::TAMPER_CLOSE,      {3.f, 30.f}}, // Cut-off filter frequency 15k915 Hz, period 63us
    {Events::TAMPER_OPEN,       {0.f, 30.f}}, // Cut-off filter frequency 15k915 Hz, period 63us
    {Events::USER_ALARM,        {0.f, 30.f}},
};

class EventAcCon : public Event<bool>{
    private:
        SharedMem &mem;
    public:
        EventAcCon(bool &pVar, const Events::Compare pCmp, const bool pRef, const Events::Times &pCfg, const Events::Init pInit, const std::string pName,
            SharedMem &pMem) :
        Event(pVar, pCmp, pRef, pCfg, pInit, pName),
        mem(pMem) {}
        void callback();
};

void EventAcCon::callback()
{
    LOG_S(INFO) << "AC CONNECT event detected";
    auto eventConfig = mem.getEventConfig();
    mem.readEventConfig();
    time(&eventConfig->acRestoredTimestamp);
    mem.setEventConfig(); //Sending event: AC Restored
#ifdef GENERIC_EVENT_DEBUG
    printf("##### Ac Connected #####\n");
#endif
}

void sendBatteryRestored() {
    printf("# Battery restored #\n");
}

void sendLowBattery() {
    printf("# Low Battery #\n");
}

void sendAcConnected() {
    printf("# Ac Connected #\n");
}

void sendAcDisconnected() {
    printf("# Ac Disconnected #\n");
}

void sendShutdown() {
    printf("# Shutdown #\n");
}

int main(int argc, char** argv) {
    int i;
    bool j;

    EventAcCon acCon(acStatus, Events::MORE_THAN, 0, eventConfig[Events::AC_CONNECT], aInit, Events::AC_CONNECT, mem);

    Event<int> batteryRestored(i, MORE_THAN, 70, 1, false, (const void *)sendBatteryRestored);
    Event<int> lowBattery(i, LESS_THAN, 30, 1, true, (const void *)sendLowBattery);
    batteryRestored.setLink(&lowBattery);
    lowBattery.setLink(&batteryRestored);

    Event<bool> acConnect(j, MORE_THAN, 0, 1, false, (const void *)sendAcConnected);
    Event<bool> acDisconnect(j, LESS_THAN, 1, 1, true, (const void *)sendAcDisconnected);
    acConnect.setLink(&acDisconnect);
    acDisconnect.setLink(&acConnect);

    EventList events({&lowBattery, &batteryRestored, &acConnect, &acDisconnect});

    for (i=0; i<100; i+=10) {
        j = (i > 50) ? true : false;
        events.run();
        printf("Count = %d, bool = %d, ", i, j);
        printf("acConnect = %d, acDisconnect = %d, ", acConnect.get(), acDisconnect.get());
        printf("batteryRestored = %d, lowBattery = %d \n", batteryRestored.get(), lowBattery.get());
        usleep(500000);
    }
    for (i=100; i>0; i-=10) {
        j = (i > 50) ? true : false;
        events.run();
        printf("Count = %d, bool = %d, ", i, j);
        printf("acConnect = %d, acDisconnect = %d, ", acConnect.get(), acDisconnect.get());
        printf("batteryRestored = %d, lowBattery = %d \n", batteryRestored.get(), lowBattery.get());
        usleep(500000);
    }

    return 0;
}