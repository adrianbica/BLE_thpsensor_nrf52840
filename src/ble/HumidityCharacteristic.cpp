#include "HumidityCharacteristic.h"

HumidityCharacteristic::HumidityCharacteristic(uint16_t initial_value)
{
    this->initial_value.humidity = initial_value;
}

uint16_t HumidityCharacteristic::Encode(const void *data, uint8_t *buffer, uint16_t buffer_size)
{
    SensingServiceHumidity  *service = (SensingServiceHumidity*)data;

    bds_char_enc_buffer_t enc_buffer;
    bds_char_enc_buffer_init(&enc_buffer, buffer, buffer_size);
    bds_16bit_encode_to_buffer(&enc_buffer, service->humidity);
    return bds_char_enc_buffer_done(&enc_buffer, buffer);

}

void HumidityCharacteristic::InitParams(void)
{
    uint8_t humidity_encoded_value[MAX_HUMIDITY_LEN];

    memset(&params, 0, sizeof(params));

    params.uuid                = 0x2A6F; 
    params.max_len             = MAX_HUMIDITY_LEN;
    params.init_len            = Encode(&initial_value, humidity_encoded_value, sizeof(humidity_encoded_value));
    params.p_init_value        = humidity_encoded_value; 
    params.char_props.notify   = 1; 
    params.char_props.read     = 1; 
    params.read_access         = SEC_OPEN; 
    params.cccd_write_access   = SEC_OPEN;
    // 1 for variable length and 0 for fixed length.
    params.is_var_len          = 1; 
}


