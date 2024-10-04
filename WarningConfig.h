#ifndef WARNING_CONFIG_H
#define WARNING_CONFIG_H

typedef enum {
    DISABLE = 0,
    ENABLE = 1
} WarningState;

typedef struct {
    WarningState temperatureWarningEnabled;
    WarningState socWarningEnabled;
    WarningState chargeRateWarningEnabled;
} WarningConfig;

extern WarningConfig warningConfig;

#endif // WARNING_CONFIG_H
