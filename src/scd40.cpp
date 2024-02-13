#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>

#include "scd40.h"

SCD40::SCD40() {}

SCD40::~SCD40() {}

uint16_t SCD40::sensirion_common_bytes_to_uint16_t(const uint8_t* bytes) {
    return (uint16_t)bytes[0] << 8 | (uint16_t)bytes[1];
}

uint16_t SCD40::sensirion_i2c_add_command_to_buffer(uint8_t* buffer, uint16_t offset,
                                             uint16_t command) {
    buffer[offset++] = (uint8_t)((command & 0xFF00) >> 8);
    buffer[offset++] = (uint8_t)((command & 0x00FF) >> 0);
    return offset;
}

int8_t SCD40::sensirion_i2c_hal_write(uint8_t address, const uint8_t* data,
                               uint16_t count) {
    return i2c_write(i2c_dev, data, count, address);
}

int16_t SCD40::sensirion_i2c_write_data(uint8_t address, const uint8_t* data,
                                 uint16_t data_length) {
    return sensirion_i2c_hal_write(address, data, data_length);
}

int8_t SCD40::sensirion_i2c_hal_read(uint8_t address, uint8_t* data, uint16_t count) {
    return i2c_read(i2c_dev, data, count, address);
}

uint8_t SCD40::sensirion_i2c_generate_crc(const uint8_t* data, uint16_t count) {
    uint16_t current_byte;
    uint8_t crc = CRC8_INIT;
    uint8_t crc_bit;

    /* calculates 8-Bit checksum with given polynomial */
    for (current_byte = 0; current_byte < count; ++current_byte) {
        crc ^= (data[current_byte]);
        for (crc_bit = 8; crc_bit > 0; --crc_bit) {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

int8_t SCD40::sensirion_i2c_check_crc(const uint8_t* data, uint16_t count,
                               uint8_t checksum) {
    if (sensirion_i2c_generate_crc(data, count) != checksum)
        return CRC_ERROR;
    return NO_ERROR;
}

int16_t SCD40::sensirion_i2c_read_data_inplace(uint8_t address, uint8_t* buffer,
                                        uint16_t expected_data_length) {
    int16_t error;
    uint16_t i, j;
    uint16_t size = (expected_data_length / SENSIRION_WORD_SIZE) *
                    (SENSIRION_WORD_SIZE + CRC8_LEN);

    if (expected_data_length % SENSIRION_WORD_SIZE != 0) {
        return BYTE_NUM_ERROR;
    }

    error = sensirion_i2c_hal_read(address, buffer, size);
    if (error) {
        return error;
    }

    for (i = 0, j = 0; i < size; i += SENSIRION_WORD_SIZE + CRC8_LEN) {

        error = sensirion_i2c_check_crc(&buffer[i], SENSIRION_WORD_SIZE,
                                        buffer[i + SENSIRION_WORD_SIZE]);
        if (error) {
            return error;
        }
        buffer[j++] = buffer[i];
        buffer[j++] = buffer[i + 1];
    }

    return NO_ERROR;
}

void SCD40::sensirion_i2c_hal_sleep_usec(uint32_t useconds) {
    int32_t remaining = useconds;
    while (remaining > 0) {
        remaining = k_usleep(remaining);
    }
}

int16_t SCD40::scd4x_start_periodic_measurement() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x21B1);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return NO_ERROR;
}

int16_t SCD40::scd4x_read_measurement_ticks(uint16_t* co2, uint16_t* temperature,
                                     uint16_t* humidity) {
    int16_t error;
    uint8_t buffer[9];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0xEC05);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SCD4X_I2C_ADDRESS, &buffer[0], 6);
    if (error) {
        return error;
    }
    *co2 = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    *temperature = sensirion_common_bytes_to_uint16_t(&buffer[2]);
    *humidity = sensirion_common_bytes_to_uint16_t(&buffer[4]);
    return NO_ERROR;
}

int16_t SCD40::scd4x_read_measurement(uint16_t* co2, int32_t* temperature_m_deg_c,
                               int32_t* humidity_m_percent_rh) {
    int16_t error;
    uint16_t temperature;
    uint16_t humidity;

    error = scd4x_read_measurement_ticks(co2, &temperature, &humidity);
    if (error) {
        return error;
    }
    *temperature_m_deg_c = ((21875 * (int32_t)temperature) >> 13) - 45000;
    *humidity_m_percent_rh = ((12500 * (int32_t)humidity) >> 13);
    return NO_ERROR;
}

int16_t SCD40::scd4x_stop_periodic_measurement() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3F86);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(500000);
    return NO_ERROR;
}