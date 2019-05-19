#ifndef __LED_SERVICE_H__
#define __LED_SERVICE_H__

#include "Service.h"
#include "DigitalCharacteristic.h"


class LedService;

/**@brief LED service event type. */
enum LedServiceEventType
{ 
    BLE_LED_SERVICE_DIGITAL_EVT_NOTIFICATION_ENABLED,  /**< Digital value notification enabled event. */
    BLE_LED_SERVICE_DIGITAL_EVT_NOTIFICATION_DISABLED, /**< Digital value notification disabled event. */
    BLE_LED_SERVICE_DIGITAL_EVT_WRITE, /**< Digital write event. */
};

/**@brief LED Service event. */
struct LedServiceEvent
{
    LedServiceEventType      evt_type;    /**< Type of event. */
    union
    {
        uint16_t            cccd_value;     /**< Holds decoded data in Notify and Indicate event handler. */
        ServiceDigital      digital;        /**< Holds decoded data in Write event handler. */
    } params;
};

typedef void (*LedServiceEventHandler) (LedService *p_service, LedServiceEvent *p_evt);

class LedService: public Service
{
private:
    LedServiceEventHandler      eventHandler;
    DigitalCharacteristic       *pLedCharacteristic;
    ble_gatts_char_handles_t    digital_handles; /**< Handles related to the Digital characteristic. */

    uint32_t IsIndicationEnabled(ble_gatts_char_handles_t, bool* p_indication_enabled);
    void OnWrite(ble_evt_t const *p_ble_evt);
public:
    LedService(void);
    uint32_t Init(LedServiceEventHandler eh);

    uint32_t Set(ServiceDigital* p_digital);
};
#endif

