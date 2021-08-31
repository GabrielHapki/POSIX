#include "Events.hpp"
#include <unistd.h>
#include <stdio.h>

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