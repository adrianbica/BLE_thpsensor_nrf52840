#ifndef __TEMPERATURE_CHARACTERISTIC__
#define __TEMPERATURE_CHARACTERISTIC__

#include "Characteristic.h"
#include "app_util_bds.h"
#include "../char_enc_dec.h"

/** @brief Temperature.
 *
 *  General information:
 *      - UUID:        0x2A6E
 *      - Type:        org.bluetooth.characteristic.temperature
 *      - Requirement: Mandatory
 */
struct SensingServiceTemperature
{
    /** @brief Temperature.
     *
     *  Unit is in degrees Celsius with a resolution of 0.01 degrees Celsius
     *  - Requirement:  Mandatory
     *  - Unit:         Celsius temperature (degree Celsius)
     *  - Format:       sint16
     *  - Decimal Exp.: -2
     */
    int16_t temperature;

};

#define OPCODE_LENGTH 1 /**< Length of opcode inside THP Sensing Service packet. */
#define HANDLE_LENGTH 2 /**< Length of handle inside THP Sensing Service packet. */
#define MAX_TEMPERATURE_LEN (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Temperature. */ 

class TemperatureCharacteristic: public Characteristic
{
private:
    SensingServiceTemperature          initial_value;

public:
    TemperatureCharacteristic();
    TemperatureCharacteristic(int16_t initial_value);
    uint16_t Encode(const void *data, uint8_t *buffer, uint16_t buffer_size);
    void InitParams(void);
};

#endif

