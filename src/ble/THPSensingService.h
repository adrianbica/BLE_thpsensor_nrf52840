#ifndef __THP_SENSING_SERVICE_CPP_H__
#define __THP_SENSING_SERVICE_CPP_H__

#include "Service.h"
#include "HumidityCharacteristic.h"
#include "PressureCharacteristic.h"
#include "TemperatureCharacteristic.h"

class THPSensingService;

enum SensingServiceEventType 
{
    BLE_THP_SENSING_SERVICE_HUMIDITY_EVT_NOTIFICATION_ENABLED,      /* Humidity value notification enabled event. */
    BLE_THP_SENSING_SERVICE_HUMIDITY_EVT_NOTIFICATION_DISABLED,     /* Humidity value notification disabled event. */
    BLE_THP_SENSING_SERVICE_HUMIDITY_EVT_CCCD_WRITE,                /* Humidity CCCD write event. */
    BLE_THP_SENSING_SERVICE_PRESSURE_EVT_NOTIFICATION_ENABLED,      /* Pressure value notification enabled event. */
    BLE_THP_SENSING_SERVICE_PRESSURE_EVT_NOTIFICATION_DISABLED,     /* Pressure value notification disabled event. */
    BLE_THP_SENSING_SERVICE_PRESSURE_EVT_CCCD_WRITE,                /* Pressure CCCD write event. */
    BLE_THP_SENSING_SERVICE_TEMPERATURE_EVT_NOTIFICATION_ENABLED,   /* Temperature value notification enabled event. */
    BLE_THP_SENSING_SERVICE_TEMPERATURE_EVT_NOTIFICATION_DISABLED,  /* Temperature value notification disabled event. */
    BLE_THP_SENSING_SERVICE_TEMPERATURE_EVT_CCCD_WRITE,             /* Temperature CCCD write event. */
};

/**@brief THP Sensing Service Service event. */
struct SensingServiceEvent
{
    SensingServiceEventType      evt_type;    /**< Type of event. */
    union
    {
        uint16_t cccd_value; /**< Holds decoded data in Notify and Indicate event handler. */
    } params;
};

typedef void (*SensingServiceEventHandler) (THPSensingService *p_service, SensingServiceEvent *p_evt);

class THPSensingService: public Service
{
private:
    SensingServiceEventHandler       eventHandler;
    HumidityCharacteristic           *pHumidCharacteristic;
    PressureCharacteristic           *pPressureCharacteristic;
    TemperatureCharacteristic        *pTemperatureCharacteristic;
    ble_gatts_char_handles_t         humidity_handles;       /**< Handles related to the Humidity characteristic. */
    ble_gatts_char_handles_t         pressure_handles;       /**< Handles related to the Pressure characteristic. */
    ble_gatts_char_handles_t         temperature_handles;    /**< Handles related to the Temperature characteristic. */

    uint32_t IsIndicationEnabled(ble_gatts_char_handles_t, bool* p_indication_enabled);
    //void OnConnect(ble_evt_t const *p_ble_evt);
    //void OnDisconnect(ble_evt_t const *p_ble_evt);
    void OnWrite(ble_evt_t const *p_ble_evt);
    //void OnRwAuthorizeRequest(ble_evt_t const *p_ble_evt);
public:
    THPSensingService(void);
    uint32_t Init(SensingServiceEventHandler eh);
    //uint16_t GetHandle(void);
    //uint32_t Add(ble_uuid_t const *p_uuid);   

    uint32_t Set(SensingServiceHumidity* p_humidity);
    uint32_t Send(SensingServiceHumidity* p_humidity);
    uint32_t Update(SensingServiceHumidity* p_humidity);
    uint32_t Set(SensingServicePressure* p_pressure);
    uint32_t Send(SensingServicePressure* p_pressure);
    uint32_t Update(SensingServicePressure* p_pressure);
    uint32_t Set(SensingServiceTemperature* p_temperature);
    uint32_t Send(SensingServiceTemperature* p_temperature);
    uint32_t Update(SensingServiceTemperature* p_temperature);
    //uint32_t AddCharacteristic(Characteristic *p_characteristic, ble_gatts_char_handles_t *handles);
    //void OnBleEvent(ble_evt_t const * p_ble_evt);
};

#endif

