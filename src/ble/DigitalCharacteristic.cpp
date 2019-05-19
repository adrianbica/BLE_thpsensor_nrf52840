#include "DigitalCharacteristic.h"
#include "nrf_log.h"

DigitalCharacteristic::DigitalCharacteristic(uint8_t *initial_value)
{
    for (int i = 0; i < BLE_SERVICE_DIGITAL_DIGITAL_MAX_ITEMS; i++) {
        this->initial_value.digital[i] = *initial_value++;
    }
    this->initial_value.digital_count = BLE_SERVICE_DIGITAL_DIGITAL_MAX_ITEMS;
}

uint16_t DigitalCharacteristic::Encode(const void *data, uint8_t *buffer, uint16_t buffer_size)
{
    ServiceDigital          *service = (ServiceDigital*)data;
    bds_char_enc_buffer_t   enc_buffer;

    bds_char_enc_buffer_init(&enc_buffer, buffer, buffer_size);
    NRF_LOG_INFO("Limit: %d", service->digital_count);
    for (int i = 0; i < service->digital_count; i++)
    {
        bds_2bit_encode_to_buffer(&enc_buffer, service->digital[i]);
    }
    return bds_char_enc_buffer_done(&enc_buffer, buffer);
}


uint16_t DigitalCharacteristic::Decode(const void *data, const uint8_t *buffer, uint16_t buffer_size)
{
    ServiceDigital          *service = (ServiceDigital*)data;
    bds_char_dec_buffer_t   dec_buffer;

    bds_char_dec_buffer_init(&dec_buffer, buffer, buffer_size);
    NRF_LOG_INFO("Limit: %d", service->digital_count);
    for (int i = 0; i < service->digital_count; i++)
    {
        service->digital[i] = bds_2bit_decode_from_buffer(&dec_buffer);
        NRF_LOG_INFO("Digital[%d]: %d", i, service->digital[i]);
    }
    return bds_char_dec_buffer_done(&dec_buffer, buffer);
}

void DigitalCharacteristic::InitParams(void)
{
    uint8_t digital_encoded_value[MAX_DIGITAL_LEN];

    memset(&params, 0, sizeof(params));

    params.uuid                = 0x2A56; 
    params.max_len             = MAX_DIGITAL_LEN;
    params.init_len            = Encode(&initial_value, digital_encoded_value, sizeof(digital_encoded_value));
    params.p_init_value        = digital_encoded_value; 
    params.char_props.read     = 1; 
    params.read_access         = SEC_OPEN; 
    params.char_props.write    = 1; 
    params.write_access        = SEC_OPEN; 
    // 1 for variable length and 0 for fixed length.
    params.is_var_len          = 1; 
}

