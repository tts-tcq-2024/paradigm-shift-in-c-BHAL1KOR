#include <assert.h>
#include "BatteryHealthChecker.h"
#include "BatteryHealthCheckerTest.h"

void TestBatteryHealthNormal() {
    assert(BatteryIsOk(25, 70, 0.7));
}

void TestBatteryHealthTemperature() {
    assert(!BatteryIsOk(50, 70, 0.7)); // Temperature too high
    assert(!BatteryIsOk(-1, 70, 0.7)); // Temperature too low
    assert(BatteryIsOk(1, 70, 0.7)); // Temperature warning low
    assert(BatteryIsOk(44, 70, 0.7)); // Temperature warning high
}

void TestBatteryHealthSoc() {
    assert(!BatteryIsOk(25, 85, 0.7)); // SOC too high
    assert(!BatteryIsOk(25, 15, 0.7)); // SOC too low
    assert(BatteryIsOk(25, 21, 0.7)); // SOC warning low
    assert(BatteryIsOk(25, 76, 0.7)); // SOC warning high
}

void TestBatteryHealthChargeRate() {
    assert(!BatteryIsOk(25, 70, 0.9)); // Charge rate too high
    assert(BatteryIsOk(25, 70, 0.1)); // Charge rate normal
    assert(BatteryIsOk(25, 70, 0.76)); // Charge rate warning high
}

void TestBatteryHealth() {
    TestBatteryHealthNormal();
    TestBatteryHealthTemperature();
    TestBatteryHealthSoc();
    TestBatteryHealthChargeRate();
}
