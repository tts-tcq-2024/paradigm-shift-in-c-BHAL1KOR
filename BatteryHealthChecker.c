#include <stdio.h>
#include "BatteryHealthChecker.h"

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

int CheckLowerLimit(float value, float lowerLimit, float warningTolerance, const char* parameter) {
    if (value < lowerLimit) {
        printf("%s out of range: Too low!\n", parameter);
        return 0;
    }
    if (value < lowerLimit + warningTolerance) {
        PrintWarning(parameter, "approaching lower limit");
    }
    return 1;
}

int CheckUpperLimit(float value, float upperLimit, float warningTolerance, const char* parameter) {
    if (value > upperLimit) {
        printf("%s out of range: Too high!\n", parameter);
        return 0;
    }
    if (value > upperLimit - warningTolerance) {
        PrintWarning(parameter, "approaching upper limit");
    }
    return 1;
}

int IsWithinRange(float value, float lowerLimit, float upperLimit, float warningTolerance, const char* parameter) {
    return CheckLowerLimit(value, lowerLimit, warningTolerance, parameter) &&
           CheckUpperLimit(value, upperLimit, warningTolerance, parameter);
}

int TemperatureIsOk(float temperature) {
    return IsWithinRange(temperature, TEMPERATURE_LOWER_LIMIT, TEMPERATURE_UPPER_LIMIT, TEMPERATURE_WARNING_TOLERANCE, "Temperature");
}

int SocIsOk(float soc) {
    return IsWithinRange(soc, SOC_LOWER_LIMIT, SOC_UPPER_LIMIT, SOC_WARNING_TOLERANCE, "State of Charge");
}

int ChargeRateIsOk(float chargeRate) {
    return IsWithinRange(chargeRate, 0, CHARGE_RATE_UPPER_LIMIT, CHARGE_RATE_WARNING_TOLERANCE, "Charge Rate");
}

int BatteryIsOk(float temperature, float soc, float chargeRate) {
    return TemperatureIsOk(temperature) && SocIsOk(soc) && ChargeRateIsOk(chargeRate);
}
