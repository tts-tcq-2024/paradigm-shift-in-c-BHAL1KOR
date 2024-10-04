#include <stdio.h>
#include "BatteryHealthChecker.h"
#include "WarningConfig.h"

#define TEMPERATURE_UPPER_LIMIT 45
#define TEMPERATURE_LOWER_LIMIT 0
#define SOC_UPPER_LIMIT 80
#define SOC_LOWER_LIMIT 20
#define CHARGE_RATE_UPPER_LIMIT 0.8

#define TEMPERATURE_WARNING_TOLERANCE (TEMPERATURE_UPPER_LIMIT * 0.05)
#define SOC_WARNING_TOLERANCE (SOC_UPPER_LIMIT * 0.05)
#define CHARGE_RATE_WARNING_TOLERANCE (CHARGE_RATE_UPPER_LIMIT * 0.05)

void PrintWarning(const char* parameter, const char* condition) {
    printf("Warning: %s %s\n", parameter, condition);
}

void PrintBreach(const char* parameter, const char* condition) {
    printf("%s out of range: %s!\n", parameter, condition);
}

int CheckLowerLimit(float value, float lowerLimit, float warningTolerance, const char* parameter, int warningEnabled) {
    if (value < lowerLimit) {
        PrintBreach(parameter, "Too low");
        return 0;
    }
    if (warningEnabled && value < lowerLimit + warningTolerance) {
        PrintWarning(parameter, "approaching lower limit");
    }
    return 1;
}

int CheckUpperLimit(float value, float upperLimit, float warningTolerance, const char* parameter, int warningEnabled) {
    if (value > upperLimit) {
        PrintBreach(parameter, "Too high");
        return 0;
    }
    if (warningEnabled && value > upperLimit - warningTolerance) {
        PrintWarning(parameter, "approaching upper limit");
    }
    return 1;
}

int IsWithinRange(float value, float lowerLimit, float upperLimit, float warningTolerance, const char* parameter, int warningEnabled) {
    return CheckLowerLimit(value, lowerLimit, warningTolerance, parameter, warningEnabled) &&
           CheckUpperLimit(value, upperLimit, warningTolerance, parameter, warningEnabled);
}

int TemperatureIsOk(float temperature) {
    return IsWithinRange(temperature, TEMPERATURE_LOWER_LIMIT, TEMPERATURE_UPPER_LIMIT, TEMPERATURE_WARNING_TOLERANCE, "Temperature", warningConfig.temperatureWarningEnabled);
}

int SocIsOk(float soc) {
    return IsWithinRange(soc, SOC_LOWER_LIMIT, SOC_UPPER_LIMIT, SOC_WARNING_TOLERANCE, "State of Charge", warningConfig.socWarningEnabled);
}

int ChargeRateIsOk(float chargeRate) {
    return CheckUpperLimit(chargeRate, CHARGE_RATE_UPPER_LIMIT, CHARGE_RATE_WARNING_TOLERANCE, "Charge Rate", warningConfig.chargeRateWarningEnabled);
}

int BatteryIsOk(float temperature, float soc, float chargeRate) {
    return TemperatureIsOk(temperature) && SocIsOk(soc) && ChargeRateIsOk(chargeRate);
}
