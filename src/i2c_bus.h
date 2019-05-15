#ifndef __I2C_BUD_H__
#define __I2C_BUD_H__

#include "app_error.h"

class i2c_bus
{
private:
    uint8_t     i2c_dev_addr;
    volatile bool m_xfer_done;
    // private default constructor(prohibits creation from outside)
    i2c_bus(void);
    // private copy constructor (prohibits copy creation)
    i2c_bus(const i2c_bus&);
    ~i2c_bus(void);
public:
    static i2c_bus& GetInstance(void);
    void init(uint8_t dev_addr);
    ret_code_t write(uint8_t *buffer, uint8_t len);
    ret_code_t read(uint8_t *buffer, uint8_t len);
    bool transfer_ready(void) {return m_xfer_done;};
    void complete_transfer(void) {m_xfer_done = true;};
};
#endif

