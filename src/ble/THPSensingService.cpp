#include "THPSensingService.h"
#include "nrf_log.h"

THPSensingService::THPSensingService(void) :
    Service()
{
    pHumidCharacteristic = new HumidityCharacteristic();
    pPressureCharacteristic = new PressureCharacteristic();
    pTemperatureCharacteristic = new TemperatureCharacteristic(0);
}

uint32_t THPSensingService::Init(SensingServiceEventHandler eventHandler)
{
    uint32_t        err_code;
    const ble_uuid128_t base_uuid128 = {CUSTOM_THP_SERVICE_UUID_BASE};

    this->eventHandler = eventHandler;
    conn_handle = BLE_CONN_HANDLE_INVALID;

    err_code = Add(&base_uuid128);
    if (err_code != NRF_SUCCESS) return err_code;

    pHumidCharacteristic->InitParams();
    err_code = AddCharacteristic(pHumidCharacteristic, &humidity_handles);
    if (err_code != NRF_SUCCESS) 
        return err_code;

    pPressureCharacteristic->InitParams();
    err_code = AddCharacteristic(pPressureCharacteristic, &pressure_handles);
    if (err_code != NRF_SUCCESS)
        return err_code;

    pTemperatureCharacteristic->InitParams();
    err_code = AddCharacteristic(pTemperatureCharacteristic, &temperature_handles);
    if (err_code != NRF_SUCCESS)
        return err_code;
    return NRF_SUCCESS;
}

uint32_t THPSensingService::IsIndicationEnabled(ble_gatts_char_handles_t handles, bool *p_indication_enabled)
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

uint32_t THPSensingService::Set(SensingServiceHumidity *p_humidity)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[MAX_HUMIDITY_LEN];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = pHumidCharacteristic->Encode(p_humidity, encoded_value, sizeof(encoded_value));
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(conn_handle, humidity_handles.value_handle, &gatts_value);
}

uint32_t THPSensingService::Set(SensingServicePressure *p_pressure)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[MAX_HUMIDITY_LEN];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = pPressureCharacteristic->Encode(p_pressure, encoded_value, sizeof(encoded_value));
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(conn_handle, pressure_handles.value_handle, &gatts_value);
}

uint32_t THPSensingService::Set(SensingServiceTemperature *p_temperature)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[MAX_HUMIDITY_LEN];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = pTemperatureCharacteristic->Encode(p_temperature, encoded_value, sizeof(encoded_value));
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(conn_handle, temperature_handles.value_handle, &gatts_value);
}

uint32_t THPSensingService::Send(SensingServiceHumidity *p_humidity)
{
    uint32_t err_code = NRF_SUCCESS;

    if (service_handle != BLE_CONN_HANDLE_INVALID) {
        ble_gatts_hvx_params_t hvx_params;
        uint8_t encoded_value[MAX_HUMIDITY_LEN];
        uint16_t hvx_len;

        // Initialize value struct.
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_len           = pHumidCharacteristic->Encode(p_humidity, encoded_value, sizeof(encoded_value));
        hvx_params.handle = humidity_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.p_len  = &hvx_len;
        hvx_params.offset = 0;
        hvx_params.p_data = encoded_value;

        err_code = sd_ble_gatts_hvx(conn_handle, &hvx_params);
    } else {
        err_code = NRF_ERROR_INVALID_STATE;
    }
    return err_code;
}

uint32_t THPSensingService::Send(SensingServicePressure *p_pressure)
{
    uint32_t err_code = NRF_SUCCESS;

    if (service_handle != BLE_CONN_HANDLE_INVALID) {
        ble_gatts_hvx_params_t hvx_params;
        uint8_t encoded_value[MAX_HUMIDITY_LEN];
        uint16_t hvx_len;

        // Initialize value struct.
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_len           = pPressureCharacteristic->Encode(p_pressure, encoded_value, sizeof(encoded_value));
        hvx_params.handle = pressure_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.p_len  = &hvx_len;
        hvx_params.offset = 0;
        hvx_params.p_data = encoded_value;

        err_code = sd_ble_gatts_hvx(conn_handle, &hvx_params);
    } else {
        err_code = NRF_ERROR_INVALID_STATE;
    }
    return err_code;
}

uint32_t THPSensingService::Send(SensingServiceTemperature *p_temperature)
{
    uint32_t err_code = NRF_SUCCESS;

    if (service_handle != BLE_CONN_HANDLE_INVALID) {
        ble_gatts_hvx_params_t hvx_params;
        uint8_t encoded_value[MAX_HUMIDITY_LEN];
        uint16_t hvx_len;

        // Initialize value struct.
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_len           = pHumidCharacteristic->Encode(p_temperature, encoded_value, sizeof(encoded_value));
        hvx_params.handle = temperature_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.p_len  = &hvx_len;
        hvx_params.offset = 0;
        hvx_params.p_data = encoded_value;

        err_code = sd_ble_gatts_hvx(conn_handle, &hvx_params);
    } else {
        err_code = NRF_ERROR_INVALID_STATE;
    }
    return err_code;
}

uint32_t THPSensingService::Update(SensingServiceHumidity *p_humidity)
{
    bool indication_enabled = false;

    IsIndicationEnabled(humidity_handles, &indication_enabled);

    if (indication_enabled) {
        return Send(p_humidity);
    } else {
        return Set(p_humidity);
    }
}

uint32_t THPSensingService::Update(SensingServicePressure *p_pressure)
{
    bool indication_enabled = false;

    IsIndicationEnabled(pressure_handles, &indication_enabled);

    if (indication_enabled) {
        return Send(p_pressure);
    } else {
        return Set(p_pressure);
    }
}
uint32_t THPSensingService::Update(SensingServiceTemperature *p_temperature)
{
    bool indication_enabled = false;

    IsIndicationEnabled(temperature_handles, &indication_enabled);

    if (indication_enabled) {
        return Send(p_temperature);
    } else {
        return Set(p_temperature);
    }
}

void THPSensingService::OnWrite(ble_evt_t const *p_ble_evt)
{
    if(p_ble_evt->evt.gatts_evt.params.write.handle == humidity_handles.cccd_handle)
    {
        if(eventHandler != NULL)
        {
            SensingServiceEvent evt;
            evt.evt_type = BLE_THP_SENSING_SERVICE_HUMIDITY_EVT_CCCD_WRITE;
            bds_uint16_decode(p_ble_evt->evt.gatts_evt.params.write.len, p_ble_evt->evt.gatts_evt.params.write.data, &(evt.params.cccd_value));
            eventHandler(this, &evt);
        }
    } 
    if(p_ble_evt->evt.gatts_evt.params.write.handle == pressure_handles.cccd_handle)
    {
        if(eventHandler != NULL)
        {
            SensingServiceEvent evt;
            evt.evt_type = BLE_THP_SENSING_SERVICE_PRESSURE_EVT_CCCD_WRITE;
            bds_uint16_decode(p_ble_evt->evt.gatts_evt.params.write.len, p_ble_evt->evt.gatts_evt.params.write.data, &(evt.params.cccd_value));
            eventHandler(this, &evt);
        }
    } 
    if(p_ble_evt->evt.gatts_evt.params.write.handle == temperature_handles.cccd_handle)
    {
        if(eventHandler != NULL)
        {
            SensingServiceEvent evt;
            evt.evt_type = BLE_THP_SENSING_SERVICE_TEMPERATURE_EVT_CCCD_WRITE;
            bds_uint16_decode(p_ble_evt->evt.gatts_evt.params.write.len, p_ble_evt->evt.gatts_evt.params.write.data, &(evt.params.cccd_value));
            eventHandler(this, &evt);
        }
    } 
}


