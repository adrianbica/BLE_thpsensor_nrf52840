#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "sdk_common.h"
#include "nordic_common.h"
#include "nrf.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "Characteristic.h"

#include "nrf_log.h"

class Service
{
protected:
    uint16_t    service_handle;         /**< Handle of service (as provided by the BLE stack). */
    uint16_t    conn_handle;            /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
private:
    virtual uint32_t IsIndicationEnabled(ble_gatts_char_handles_t, bool* p_indication_enabled) = 0;
    virtual void OnWrite(ble_evt_t const *p_ble_evt) = 0;
protected:
    void OnConnect(ble_evt_t const *p_ble_evt);
    void OnDisconnect(ble_evt_t const *p_ble_evt);
    void OnRwAuthorizeRequest(ble_evt_t const *p_ble_evt);

public:
    Service(void){};
    uint32_t Add(ble_uuid_t const *p_uuid);   
    uint32_t Add(ble_uuid128_t const *p_uuid);   
    uint32_t AddCharacteristic(Characteristic *p_characteristic, ble_gatts_char_handles_t *handles);
    //virtual uint32_t Init();
    uint16_t GetHandle(void) { return service_handle; }
    void OnBleEvent(ble_evt_t const *p_ble_evt);
};

#endif
