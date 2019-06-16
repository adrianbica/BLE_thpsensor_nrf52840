#include "ServoMotorService.h"
#include "nrf_log.h"


uint16_t ServoMotorInitialValue = 2950;

ServoMotorService::ServoMotorService(void) :
    Service()
{
    pServoMotorCharacteristic = new AnalogCharacteristic(ServoMotorInitialValue);
}

uint32_t ServoMotorService::Init(ServoMotorServiceEventHandler eventHandler)
{
    uint32_t                err_code;
    const ble_uuid128_t     base_uuid128 = {CUSTOM_SERVOMOTOR_SERVICE_UUID_BASE};

    this->eventHandler = eventHandler;
    conn_handle = BLE_CONN_HANDLE_INVALID;

    err_code = Add(&base_uuid128);
    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Add Err: %d", err_code);
        return err_code;
    }

    pServoMotorCharacteristic->InitParams();
    err_code = AddCharacteristic(pServoMotorCharacteristic, &analog_handles);
    if (err_code != NRF_SUCCESS) 
        return err_code;
    return NRF_SUCCESS;
}

uint32_t ServoMotorService::IsIndicationEnabled(ble_gatts_char_handles_t handles, bool* p_indication_enabled)
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

void test_func(ServoMotorService* p_servo_service, ServoMotorServiceEvent* p_evt)
{
    NRF_LOG_INFO("Step 99\r\n");
}

void ServoMotorService::OnWrite(ble_evt_t const *p_ble_evt)
{
    if(p_ble_evt->evt.gatts_evt.params.write.handle == analog_handles.value_handle)
    {
        if(eventHandler != NULL)
        {
            ServoMotorServiceEvent evt;
            //(void)(evt);
            evt.evt_type = BLE_SERVOMOTOR_SERVICE_ANALOG_EVT_WRITE;
            /* YOUR_JOB: If evt.params.analog contains some fields that require initialization before decoding, do it here.
             * Following fileds require initialization before decoding:
             *     - field repeat count (field name ends with "_count"),
             *     - array length (field name ends with "_length"),
             *     - optional fields (field name ends with "_is_present"),
             *     - conditions (field like "bool cNN", where NN is a number)
             */
            pServoMotorCharacteristic->Decode(&evt.params.analog, p_ble_evt->evt.gatts_evt.params.write.data, p_ble_evt->evt.gatts_evt.params.write.len);
            eventHandler(this, &evt);
        }
    }
}

uint32_t ServoMotorService::Set(ServiceAnalog* p_analog)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[MAX_ANALOG_LEN];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = pServoMotorCharacteristic->Encode(p_analog, encoded_value, sizeof(encoded_value));
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(conn_handle, analog_handles.value_handle, &gatts_value);
}

