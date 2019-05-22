#include "Service.h"

void Service::OnConnect(ble_evt_t const *p_ble_evt)
{
    conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

void Service::OnDisconnect(ble_evt_t const *p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    conn_handle = BLE_CONN_HANDLE_INVALID;
}

void Service::OnRwAuthorizeRequest(ble_evt_t const *p_ble_evt)
{
    ble_gatts_evt_rw_authorize_request_t const * p_auth_req = &(p_ble_evt->evt.gatts_evt.params.authorize_request);
    if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
    {
        if ((p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op
                != BLE_GATTS_OP_PREP_WRITE_REQ)
            && (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op
                != BLE_GATTS_OP_EXEC_WRITE_REQ_NOW)
            && (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op
                != BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL)
           )
        {

        }
    }
}


uint32_t Service::Add(ble_uuid_t const *p_uuid)
{
    return sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, p_uuid, &service_handle);
}

uint32_t Service::Add(ble_uuid128_t const *p_uuid)
{
    uint32_t    err_code;
    uint8_t     uuid_type;
    ble_uuid_t  ble_uuid;

    //ble_uuid128_t base_uuid = {CUSTOM_SERVICE_UUID_BASE};

    err_code =  sd_ble_uuid_vs_add(p_uuid, &uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_INFO("Add Err 1: %d", err_code);
        return err_code;
    }
    ble_uuid.type = uuid_type;
    ble_uuid.uuid = 0x0000;
    return sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &service_handle);
}

uint32_t Service::AddCharacteristic(Characteristic *p_characteristic, ble_gatts_char_handles_t *handles)
{
    return characteristic_add(service_handle, p_characteristic->GetParams(), handles);
}

void Service::OnBleEvent(ble_evt_t const *p_ble_evt)
{
    switch (p_ble_evt->header.evt_id) {
    case BLE_GAP_EVT_CONNECTED:
        OnConnect(p_ble_evt);
        break;
    case BLE_GAP_EVT_DISCONNECTED:
        OnDisconnect(p_ble_evt);
        break;
    case BLE_GATTS_EVT_WRITE:
        OnWrite(p_ble_evt);
        break;
    case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
        OnRwAuthorizeRequest(p_ble_evt);
        break;
    default:
        //No implementation needed.
        break;
    }
}

