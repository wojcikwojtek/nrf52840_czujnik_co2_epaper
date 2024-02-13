#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>

#define SCD4X_I2C_ADDRESS 0x62 
#define CRC_ERROR 1
#define I2C_BUS_ERROR 2
#define I2C_NACK_ERROR 3
#define BYTE_NUM_ERROR 4

#define CRC8_POLYNOMIAL 0x31
#define CRC8_INIT 0xFF
#define CRC8_LEN 1

#define SENSIRION_COMMAND_SIZE 2
#define SENSIRION_WORD_SIZE 2
#define SENSIRION_NUM_WORDS(x) (sizeof(x) / SENSIRION_WORD_SIZE)
#define SENSIRION_MAX_BUFFER_WORDS 32
#define NO_ERROR 0
#define NOT_IMPLEMENTED_ERROR 31

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))
#endif

#define SENSIRION_COMMAND_SIZE 2
#define SENSIRION_WORD_SIZE 2
#define SENSIRION_NUM_WORDS(x) (sizeof(x) / SENSIRION_WORD_SIZE)
#define SENSIRION_MAX_BUFFER_WORDS 32

#define I2C_NODE DT_NODELABEL(i2c0)

static const struct device* i2c_dev = DEVICE_DT_GET(I2C_NODE);

class SCD40 {
private:
    uint16_t sensirion_common_bytes_to_uint16_t(const uint8_t* bytes);
    uint16_t sensirion_i2c_add_command_to_buffer(uint8_t* buffer, uint16_t offset, uint16_t command);
    int8_t sensirion_i2c_hal_write(uint8_t address, const uint8_t* data, uint16_t count);
    int16_t sensirion_i2c_write_data(uint8_t address, const uint8_t* data, uint16_t data_length);
    int8_t sensirion_i2c_hal_read(uint8_t address, uint8_t* data, uint16_t count);
    uint8_t sensirion_i2c_generate_crc(const uint8_t* data, uint16_t count);
    int8_t sensirion_i2c_check_crc(const uint8_t* data, uint16_t count, uint8_t checksum);
    int16_t sensirion_i2c_read_data_inplace(uint8_t address, uint8_t* buffer, uint16_t expected_data_length);
    void sensirion_i2c_hal_sleep_usec(uint32_t useconds);
public:
    SCD40();
    ~SCD40();
    int16_t scd4x_start_periodic_measurement();
    int16_t scd4x_read_measurement_ticks(uint16_t* co2, uint16_t* temperature, uint16_t* humidity);
    int16_t scd4x_read_measurement(uint16_t* co2, int32_t* temperature_m_deg_c, int32_t* humidity_m_percent_rh);
    int16_t scd4x_stop_periodic_measurement();
};