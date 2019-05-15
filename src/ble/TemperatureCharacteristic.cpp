#include "TemperatureCharacteristic.h"

TemperatureCharacteristic::TemperatureCharacteristic(int16_t initial_value):Characteristic()
{
    this->initial_value.temperature = initial_value;
}

uint16_t TemperatureCharacteristic::Encode(const void *data, uint8_t *buffer, uint16_t buffer_size)
{
    SensingServiceTemperature  *service = (SensingServiceTemperature*)data;

    bds_char_enc_buffer_t enc_buffer;
    bds_char_enc_buffer_init(&enc_buffer, buffer, buffer_size);
    bds_16bit_encode_to_buffer(&enc_buffer, service->temperature);
    return bds_char_enc_buffer_done(&enc_buffer, buffer);
}

void TemperatureCharacteristic::InitParams(void)
{
    uint8_t temperature_encoded_value[MAX_TEMPERATURE_LEN];

    memset(&params, 0, sizeof(params));

    params.uuid                = 0x2A6E; 
    params.max_len             = MAX_TEMPERATURE_LEN;
    params.init_len            = Encode(&initial_value, temperature_encoded_value, sizeof(temperature_encoded_value));
    params.p_init_value        = temperature_encoded_value; 
    params.char_props.notify   = 1; 
    params.char_props.read     = 1; 
    params.read_access         = SEC_OPEN; 
    params.cccd_write_access   = SEC_OPEN;
    // 1 for variable length and 0 for fixed length.
    params.is_var_len          = 1; 
}

