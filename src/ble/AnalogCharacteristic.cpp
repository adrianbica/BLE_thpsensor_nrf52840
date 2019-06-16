#include "AnalogCharacteristic.h"
#include "nrf_log.h"

AnalogCharacteristic::AnalogCharacteristic(uint16_t initial_value)
{
    this->initial_value.analog = initial_value;
}

uint16_t AnalogCharacteristic::Encode(const void* data, uint8_t *buffer, uint16_t buffer_size)
{
    ServiceAnalog*          service = (ServiceAnalog*)data;
    bds_char_enc_buffer_t   enc_buffer;

    bds_char_enc_buffer_init(&enc_buffer, buffer, buffer_size);
    bds_16bit_encode_to_buffer(&enc_buffer, service->analog);
    return bds_char_enc_buffer_done(&enc_buffer, buffer);
}

uint16_t AnalogCharacteristic::Decode(const void* data, const uint8_t *buffer, uint16_t buffer_size)
{
    ServiceAnalog*          service = (ServiceAnalog*)data;
    bds_char_dec_buffer_t   dec_buffer;

    bds_char_dec_buffer_init(&dec_buffer, buffer, buffer_size);
    service->analog = bds_16bit_decode_from_buffer(&dec_buffer);
    NRF_LOG_INFO("Analog: %d", service->analog);
    return bds_char_dec_buffer_done(&dec_buffer, buffer);
}

void AnalogCharacteristic::InitParams(void)
{
    uint8_t     analog_encoded_value[MAX_ANALOG_LEN];

    memset(&params, 0, sizeof(params));

    params.uuid                = 0x2A58; 
    params.max_len             = MAX_ANALOG_LEN;
    params.init_len            = Encode(&initial_value, analog_encoded_value, sizeof(analog_encoded_value));
    params.p_init_value        = analog_encoded_value; 
    params.char_props.read     = 1; 
    params.read_access         = SEC_OPEN; 
    params.char_props.write    = 1; 
    params.write_access        = SEC_OPEN; 
    // 1 for variable length and 0 for fixed length.
    params.is_var_len          = 1; 
}
