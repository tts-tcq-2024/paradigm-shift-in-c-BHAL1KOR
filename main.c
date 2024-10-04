#include <stdio.h>
#include "BatteryHealthChecker.h"
#include "BatteryHealthCheckerTest.h"
int main() {
    TestTemperature();
    TestSoc();
    TestChargeRate();
    TestBattery();
    return 0;
}
