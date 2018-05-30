/**
 * @file batteryClient.c
 *
 * Sample client of the Battery API and Battery Admin API.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc.
 */

#include "legato.h"
#include "interfaces.h"


void percentageHandler
(
    uint8_t percentage,
    uint8_t triggerPercentage,
    bool isHigh,
    void *context
)
{
    LE_INFO(
        "Received a %s battery level event.  Current level is %d%% and trigger percentage is %d%% and context=0x%p",
        isHigh ? "high" : "low", percentage, triggerPercentage, context);
}

void chargeHandler
(
    ma_battery_ChargingStatus_t chargingStatus,
    void *context
)
{
    LE_INFO(
        "Received a charging status change event: %d and context=0x%p",
        chargingStatus,
        context);
}

void healthHandler
(
    ma_battery_HealthStatus_t healthStatus,
    void *context
)
{
    LE_INFO(
        "Received a health status change event: %d and context=0x%p",
        healthStatus,
        context);
}

COMPONENT_INIT
{
    ma_adminbattery_SetTechnology("LIPO", 2000 /* mAh */, 3700 /* mV */);

    char batteryType[256];
    uint16_t mAh;
    uint16_t voltage; // millivolts
    uint16_t percentremaining;
    uint16_t energyremaining;
    double currentvoltage, currenttemp;

    le_result_t result =
        ma_battery_GetTechnology(batteryType, sizeof(batteryType), &mAh, &voltage);
    if (result == LE_OK)
    {
        LE_INFO(
            " battery type = %s, capacity = %hd mAh,  voltage = %hd mV",
            batteryType, mAh, voltage);
    }
    else
    {
        LE_ERROR("Unable to get battery technology info (%s)", LE_RESULT_TXT(result));
    }

    ma_battery_HealthStatus_t health = ma_battery_GetHealthStatus();
    LE_INFO(" health = %d", health);

    ma_battery_ChargingStatus_t charge = ma_battery_GetChargingStatus();
    LE_INFO("charge = %d", charge);

    le_result_t result1 = ma_battery_GetVoltage(&currentvoltage);
    if (result1 == LE_OK)
    {
        LE_INFO(" current voltage = %lf in millivolts", currentvoltage);
    }

    le_result_t result2 = ma_battery_GetTemp(&currenttemp);
    if (result2 == LE_OK)
    {
        LE_INFO(" current temperature = %lf in Celcius", currenttemp);
    }

    le_result_t result3 = ma_battery_GetPercentRemaining(&percentremaining);
    if (result3 == LE_OK)
    {
        LE_INFO(" current percent = %d", percentremaining);
    }


    le_result_t result4 = ma_battery_GetChargeRemaining(&energyremaining);
    if (result4 == LE_OK)
    {
        LE_INFO(" current energy = %d", energyremaining);
    }

    ma_battery_AddLevelPercentageHandler(80, 90, percentageHandler, (void *)0xdeadbeef);

    ma_battery_AddChargingStatusChangeHandler(chargeHandler, NULL);

    ma_battery_AddHealthChangeHandler(healthHandler, NULL);
}
