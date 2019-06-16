#ifndef __SERVO_MOTOR_SERVICE_H__
#define __SERVO_MOTOR_SERVICE_H__

#include "Service.h"
#include "AnalogCharacteristic.h"

class ServoMotorService;

#define CUSTOM_SERVOMOTOR_SERVICE_UUID_BASE         {0x8C, 0x99, 0x0E, 0xCC, 0x50, 0x65, 0x4A, 0xB3, \
                                                     0xB2, 0x4C, 0x04, 0x00, 0xCB, 0x0B, 0xB7, 0xB2}

/**@brief SERVOMOTOR SERVICE event type. */
enum ServoServiceEventType
{ 
    BLE_SERVOMOTOR_SERVICE_ANALOG_EVT_NOTIFICATION_ENABLED,  /**< Analog value notification enabled event. */
    BLE_SERVOMOTOR_SERVICE_ANALOG_EVT_NOTIFICATION_DISABLED, /**< Analog value notification disabled event. */
    BLE_SERVOMOTOR_SERVICE_ANALOG_EVT_WRITE, /**< Analog write event. */
};

/**@brief SERVO SERVICE Service event. */
struct ServoMotorServiceEvent
{
    ServoServiceEventType   evt_type;    /**< Type of event. */
    union {
        uint16_t        cccd_value;     /**< Holds decoded data in Notify and Indicate event handler. */
        ServiceAnalog   analog;         /**< Holds decoded data in Write event handler. */
    } params;
};

typedef void (*ServoMotorServiceEventHandler) (ServoMotorService *p_service, ServoMotorServiceEvent *p_evt);

class ServoMotorService: public Service
{
    ServoMotorServiceEventHandler   eventHandler;
    AnalogCharacteristic            *pServoMotorCharacteristic;
    ble_gatts_char_handles_t        analog_handles;

    uint32_t IsIndicationEnabled(ble_gatts_char_handles_t handles, bool* p_indication_enabled);
    void OnWrite(ble_evt_t const *p_ble_evt);
public:
    ServoMotorService(void);
    uint32_t Init(ServoMotorServiceEventHandler eh);

    uint32_t Set(ServiceAnalog* p_analog);
};

#endif

