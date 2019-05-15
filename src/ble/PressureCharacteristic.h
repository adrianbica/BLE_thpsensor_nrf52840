#ifndef __PRESSURE_CHARACTERISTIC__
#define __PRESSURE_CHARACTERISTIC__

#include "Characteristic.h"
#include "app_util_bds.h"
#include "../char_enc_dec.h"

/** @brief Pressure.
 *
 *  General information:
 *      - UUID:        0x2A6D
 *      - Type:        org.bluetooth.characteristic.pressure
 *      - Requirement: Mandatory
 */
struct SensingServicePressure
{
    /** @brief Pressure.
     *
     *  Unit is in pascals with a resolution of 0.1 Pa
     *  - Requirement:  Mandatory
     *  - Unit:         pressure (pascal)
     *  - Format:       uint32
     *  - Decimal Exp.: -1
     */
    uint32_t pressure;

};

#define OPCODE_LENGTH 1 /**< Length of opcode inside THP Sensing Service packet. */
#define HANDLE_LENGTH 2 /**< Length of handle inside THP Sensing Service packet. */
#define MAX_PRESSURE_LEN (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Pressure. */ 

class PressureCharacteristic: public Characteristic
{
private:
    SensingServicePressure          initial_value;

public:
    PressureCharacteristic(){};
    PressureCharacteristic(uint32_t initial_value);
    uint16_t Encode(const void *data, uint8_t *buffer, uint16_t buffer_size);
    void InitParams(void);
};

#endif

