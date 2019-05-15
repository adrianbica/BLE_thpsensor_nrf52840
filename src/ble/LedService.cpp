#include "LedService.h"
#include "nrf_log.h"

uint8_t LedInitialValue[BLE_LED_SERVICE_DIGITAL_DIGITAL_MAX_ITEMS] = {0};

LedService::LedService(void) :
    Service()
{
    pLedCharacteristic = new LedCharacteristic(LedInitialValue);
}

uint32_t LedService::Init(LedServiceEventHandler eventHandler)
{
    uint32_t        err_code;
    ble_uuid_t      ble_uuid;

    this->eventHandler = eventHandler;
    conn_handle = BLE_CONN_HANDLE_INVALID;
    BLE_UUID_BLE_ASSIGN(ble_uuid, 0x0002);

    err_code = Add(&ble_uuid);
    if (err_code != NRF_SUCCESS) return err_code;

    pLedCharacteristic->InitParams();
    err_code = AddCharacteristic(pLedCharacteristic, &digital_handles);
    if (err_code != NRF_SUCCESS) 
        return err_code;
    return NRF_SUCCESS;
}

uint32_t LedService::IsIndicationEnabled(ble_gatts_char_handles_t handles, bool* p_indication_enabled)
{
    uint32_t err_code;
    uint8_t  cccd_value_buf[BLE_CCCD_VALUE_LEN];
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = BLE_CCCD_VALUE_LEN;
    gatts_value.offset  = 0;
    gatts_value.p_value = cccd_value_buf;

    err_code = sd_ble_gatts_value_get(conn_handle, handles.cccd_handle, &gatts_value);
    if (err_code == NRF_SUCCESS) {
        *p_indication_enabled = ble_srv_is_indication_enabled(cccd_value_buf) | ble_srv_is_notification_enabled(cccd_value_buf);
    }
    if (err_code == BLE_ERROR_GATTS_SYS_ATTR_MISSING) {
        *p_indication_enabled = false;
        return NRF_SUCCESS;
    }
    return err_code;
}

void LedService::OnWrite(ble_evt_t const *p_ble_evt)
{
    if (p_ble_evt->evt.gatts_evt.params.write.handle == digital_handles.value_handle)
    {
        if(eventHandler != NULL)
        {
            LedServiceEvent evt;
            evt.evt_type = BLE_LED_SERVICE_DIGITAL_EVT_WRITE;

            /* If evt.params.digital contains some fields that require initialization before decoding, do it here.
             * Following fileds require initialization before decoding:
             *     - field repeat count (field name ends with "_count"),
             *     - array length (field name ends with "_length"),
             *     - optional fields (field name ends with "_is_present"),
             *     - conditions (field like "bool cNN", where NN is a number)
             */
            evt.params.digital.digital_count = BLE_LED_SERVICE_DIGITAL_DIGITAL_MAX_ITEMS;

            pLedCharacteristic->Decode(&evt.params.digital, p_ble_evt->evt.gatts_evt.params.write.data, p_ble_evt->evt.gatts_evt.params.write.len);
            eventHandler(this, &evt);
        }
    }
}

uint32_t LedService::Set(LedServiceDigital* p_digital)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[MAX_DIGITAL_LEN];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = pLedCharacteristic->Encode(p_digital, encoded_value, sizeof(encoded_value));
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(conn_handle, digital_handles.value_handle, &gatts_value);
}


