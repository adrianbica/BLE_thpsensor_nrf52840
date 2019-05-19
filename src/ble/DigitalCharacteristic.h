#ifndef __DIGITAL_CHARACTERISTIC_H__
#define __DIGITAL_CHARACTERISTIC_H__

#include "Characteristic.h"
#include "app_util_bds.h"
#include "../char_enc_dec.h"

#ifndef BLE_SERVICE_DIGITAL_DIGITAL_MAX_ITEMS
#define BLE_SERVICE_DIGITAL_DIGITAL_MAX_ITEMS 4
#endif



struct ServiceDigital
{
    /** @brief Digital.
     *
     *  The Digital characteristic is an array of n 2-bit values in a bit field
     *  - Requirement:  Mandatory
     *  - Format:       2bit
     *
     */
    uint8_t digital[BLE_SERVICE_DIGITAL_DIGITAL_MAX_ITEMS];

    /** @brief Number of elements in field "digital".
     */
    uint16_t digital_count;

};

#define OPCODE_LENGTH 1 /**< Length of opcode inside LED service packet. */
#define HANDLE_LENGTH 2 /**< Length of handle inside LED service packet. */

/* TODO: Consider changing the max values if encoded data for characteristic/descriptor is smaller */ 
#define MAX_DIGITAL_LEN (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Digital. */ 



class DigitalCharacteristic: public Characteristic
{
protected:
    ServiceDigital      initial_value;
public:
    DigitalCharacteristic(void){};
    DigitalCharacteristic(uint8_t *initial_value);
    uint16_t Encode(const void* data, uint8_t *buffer, uint16_t buffer_size);
    uint16_t Decode(const void* data, const uint8_t *buffer, uint16_t buffer_size);
    void InitParams(void);
};

#endif
