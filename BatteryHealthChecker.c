#include <stdio.h>
#include "BatteryHealthChecker.h"
#include "WarningConfig.h"

#define TEMPERATURE_UPPER_LIMIT 45
#define TEMPERATURE_LOWER_LIMIT 0
#define SOC_UPPER_LIMIT 80
#define SOC_LOWER_LIMIT 20
#define CHARGE_RATE_UPPER_LIMIT 0.8
#define WARNING_TOLERANCE_PERCENTAGE 0.05
#define TEMPERATURE_WARNING_TOLERANCE (TEMPERATURE_UPPER_LIMIT * WARNING_TOLERANCE_PERCENTAGE)
#define SOC_WARNING_TOLERANCE (SOC_UPPER_LIMIT * WARNING_TOLERANCE_PERCENTAGE)
#define CHARGE_RATE_WARNING_TOLERANCE (CHARGE_RATE_UPPER_LIMIT * WARNING_TOLERANCE_PERCENTAGE)

void PrintWarning(const char* parameter, const char* condition) {
    printf("Warning: %s %s\n", parameter, condition);
}

void PrintBreach(const char* parameter, const char* condition) {
    printf("%s out of range: %s!\n", parameter, condition);
}

int IsBelowLowerLimit(float value, float lowerLimit) {
    return value < lowerLimit;
}

int IsAboveUpperLimit(float value, float upperLimit) {
    return value > upperLimit;
}

int IsApproachingLowerLimit(float value, float lowerLimit, float warningTolerance) {
    return value < lowerLimit + warningTolerance;
}

int IsApproachingUpperLimit(float value, float upperLimit, float warningTolerance) {
    return value > upperLimit - warningTolerance;
}

void CheckLowerLimitBreach(float value, float lowerLimit, const char* parameter) {
    if (IsBelowLowerLimit(value, lowerLimit)) {
        PrintBreach(parameter, "Too low");
    }
}

void CheckLowerLimitWarning(float value, float lowerLimit, float warningTolerance, const char* parameter) {
    if (IsApproachingLowerLimit(value, lowerLimit, warningTolerance)) {
        PrintWarning(parameter, "approaching lower limit");
    }
}

int CheckLowerLimit(float value, float lowerLimit, float warningTolerance, const char* parameter, int warningEnabled) {
    CheckLowerLimitBreach(value, lowerLimit, parameter);
    if (IsBelowLowerLimit(value, lowerLimit)) {
        return 0;
    }
    if (warningEnabled) {
        CheckLowerLimitWarning(value, lowerLimit, warningTolerance, parameter);
    }
    return 1;
}

void CheckUpperLimitBreach(float value, float upperLimit, const char* parameter) {
    if (IsAboveUpperLimit(value, upperLimit)) {
        PrintBreach(parameter, "Too high");
    }
}

void CheckUpperLimitWarning(float value, float upperLimit, float warningTolerance, const char* parameter) {
    if (IsApproachingUpperLimit(value, upperLimit, warningTolerance)) {
        PrintWarning(parameter, "approaching upper limit");
    }
}

int CheckUpperLimit(float value, float upperLimit, float warningTolerance, const char* parameter, int warningEnabled) {
    CheckUpperLimitBreach(value, upperLimit, parameter);
    if (IsAboveUpperLimit(value, upperLimit)) {
        return 0;
    }
    if (warningEnabled) {
        CheckUpperLimitWarning(value, upperLimit, warningTolerance, parameter);
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
