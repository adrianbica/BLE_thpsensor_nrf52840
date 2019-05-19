#ifndef __RELAY_SERVICE_H__
#define __RELAY_SERVICE_H__

#include "Service.h"
#include "DigitalCharacteristic.h"

class RelayService;

/**@brief Relay service event type. */
enum RelayServiceEventType
{ 
    BLE_RELAY_SERVICE_DIGITAL_EVT_NOTIFICATION_ENABLED,  /**< Digital value notification enabled event. */
    BLE_RELAY_SERVICE_DIGITAL_EVT_NOTIFICATION_DISABLED, /**< Digital value notification disabled event. */
    BLE_RELAY_SERVICE_DIGITAL_EVT_WRITE, /**< Digital write event. */
};

/**@brief LED Service event. */
struct RelayServiceEvent
{
    RelayServiceEventType      evt_type;    /**< Type of event. */
    union
    {
        uint16_t            cccd_value;     /**< Holds decoded data in Notify and Indicate event handler. */
        ServiceDigital      digital;        /**< Holds decoded data in Write event handler. */
    } params;
};

typedef void (*RelayServiceEventHandler) (RelayService *p_service, RelayServiceEvent *p_evt);

class RelayService: public Service
{
private:
    RelayServiceEventHandler    eventHandler;
    DigitalCharacteristic       *pRelayCharacteristic;
    ble_gatts_char_handles_t    digital_handles; /**< Handles related to the Digital characteristic. */

    uint32_t IsIndicationEnabled(ble_gatts_char_handles_t, bool* p_indication_enabled);
    void OnWrite(ble_evt_t const *p_ble_evt);
public:
    RelayService(void);
    uint32_t Init(RelayServiceEventHandler eh);

    uint32_t Set(ServiceDigital* p_digital);
};

#endif
