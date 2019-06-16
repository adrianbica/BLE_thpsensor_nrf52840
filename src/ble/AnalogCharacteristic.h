#ifndef __ANALOG_CHARACTERISTIC_H__
#define __ANALOG_CHARACTERISTIC_H__

#include "Characteristic.h"
#include "app_util_bds.h"
#include "../char_enc_dec.h"

#ifndef BLE_SERVICE_ANALOG_MAX_ITEMS
#define BLE_SERVICE_ANALOG_MAX_ITEMS 1
#endif

/** @brief Analog.
 *
 *  General information:
 *      - UUID:        0x2A58
 *      - Type:        org.bluetooth.characteristic.analog
 *      - Requirement: Mandatory
 *
 *  The Analog characteristic is used to read or write the value of one of the IO Module's analog signals.
 */
struct ServiceAnalog
{
    /** @brief Analog.
     *
     *  - Requirement:  Mandatory
     *  - Format:       uint16
     */
    uint16_t analog;

};

#define OPCODE_LENGTH 1 /**< Length of opcode inside SERVO SERVICE packet. */
#define HANDLE_LENGTH 2 /**< Length of handle inside SERVO SERVICE packet. */
#define MAX_ANALOG_LEN (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Analog. */ 

class AnalogCharacteristic: public Characteristic
{
private:
    ServiceAnalog           initial_value;

public:
    AnalogCharacteristic(){};
    AnalogCharacteristic(uint16_t initial_value);
    uint16_t Encode(const void* data, uint8_t *buffer, uint16_t buffer_size);
    uint16_t Decode(const void* data, const uint8_t *buffer, uint16_t buffer_size);
    void InitParams(void);
};

#endif

