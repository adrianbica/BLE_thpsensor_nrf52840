#ifndef __HUMIDITY_CHARACTERISTIC__
#define __HUMIDITY_CHARACTERISTIC__

#include "Characteristic.h"
#include "app_util_bds.h"
#include "../char_enc_dec.h"

/** @brief Humidity.
 *
 *  General information:
 *      - UUID:        0x2A6F
 *      - Type:        org.bluetooth.characteristic.humidity
 *      - Requirement: Mandatory
 */
struct SensingServiceHumidity
{
    /** @brief Humidity.
     *
     *  Unit is in percent with a resolution of 0.01 percent
     *  - Requirement:  Mandatory
     *  - Unit:         percentage
     *  - Format:       uint16
     *  - Decimal Exp.: -2
     */
    uint16_t humidity;
};

#define OPCODE_LENGTH 1 /**< Length of opcode inside THP Sensing Service packet. */
#define HANDLE_LENGTH 2 /**< Length of handle inside THP Sensing Service packet. */
#define MAX_HUMIDITY_LEN (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Humidity. */ 

class HumidityCharacteristic: public Characteristic
{
private:
    SensingServiceHumidity          initial_value;

public:
    HumidityCharacteristic(){};
    HumidityCharacteristic(uint16_t initial_value);
    uint16_t Encode(const void *data, uint8_t *buffer, uint16_t buffer_size);
    void InitParams(void);
};

#endif
