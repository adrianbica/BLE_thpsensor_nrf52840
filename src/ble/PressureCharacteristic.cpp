#include "PressureCharacteristic.h"

PressureCharacteristic::PressureCharacteristic(uint32_t initial_value)
{
    this->initial_value.pressure = initial_value;
}

uint16_t PressureCharacteristic::Encode(const void *data, uint8_t *buffer, uint16_t buffer_size)
{
    SensingServicePressure  *service = (SensingServicePressure*)data;

    bds_char_enc_buffer_t enc_buffer;
    bds_char_enc_buffer_init(&enc_buffer, buffer, buffer_size);
    bds_32bit_encode_to_buffer(&enc_buffer, service->pressure);
    return bds_char_enc_buffer_done(&enc_buffer, buffer);

}

void PressureCharacteristic::InitParams(void)
{
    uint8_t pressure_encoded_value[MAX_PRESSURE_LEN];

    memset(&params, 0, sizeof(params));

    params.uuid                = 0x2A6D; 
    params.max_len             = MAX_PRESSURE_LEN;
    params.init_len            = Encode(&initial_value, pressure_encoded_value, sizeof(pressure_encoded_value));
    params.p_init_value        = pressure_encoded_value; 
    params.char_props.notify   = 1; 
    params.char_props.read     = 1; 
    params.read_access         = SEC_OPEN; 
    params.cccd_write_access   = SEC_OPEN;
    // 1 for variable length and 0 for fixed length.
    params.is_var_len          = 1; 
}

