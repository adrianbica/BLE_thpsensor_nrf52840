#ifndef __CHARACTERISTIC_H__
#define __CHARACTERISTIC_H__

#include <stdint.h>
#include <stdbool.h>

#include "sdk_common.h"
#include "nordic_common.h"
#include "nrf.h"
#include "ble.h"
#include "app_util.h"
#include "app_util_bds.h"

class Characteristic 
{
private:
protected:
    ble_add_char_params_t   params;
public:
    Characteristic(){};
    ble_add_char_params_t *GetParams(void);
    virtual uint16_t Encode(const void* service, uint8_t *buffer, uint16_t bufsize) = 0;
    virtual void InitParams(void) = 0;
};


#endif
