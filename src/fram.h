#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>

#define FRAM_I2C_ADDRESS 0x50
#define FRAM_MAX_ADDRESS 0x1FFF

#define I2C_NODE DT_NODELABEL(i2c0)

static const struct device* i2c_fram = DEVICE_DT_GET(I2C_NODE);

class Fram {
private:
    uint16_t address;
    uint16_t add_to_buffer(uint8_t* buffer, uint16_t offset, uint16_t address);
    uint16_t combine_to_16b(uint8_t* buffer, uint16_t offset);
    uint8_t write_to_memory(uint16_t memory_address, uint8_t data);
    uint8_t write_multiple_bytes(uint16_t memory_address, uint8_t *data_buffer, uint8_t len);
    uint8_t read_from_current_address(uint8_t *buffer, uint8_t len);
    uint8_t read_from_address(uint16_t memory_address, uint8_t *buffer, uint8_t len);
public:
    Fram();
    ~Fram();
    uint8_t save_sensor_data(uint16_t co2, uint16_t temperature_deg_c, uint16_t humidity_percent_rh);
    uint8_t read_last_sensor_data(uint16_t& co2, uint16_t& temperature_deg_c, uint16_t& humidity_percent_rh);
};