#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>

#include "fram.h"

Fram::Fram() {
    address = 0x0;
}

Fram::~Fram() {}

uint16_t Fram::add_to_buffer(uint8_t* buffer, uint16_t offset, uint16_t address) {
    buffer[offset] = (uint8_t)((address & 0xFF00) >> 8);
    buffer[offset+1] = (uint8_t)((address & 0x00FF) >> 0);
    return offset + 2;
}

uint16_t Fram::combine_to_16b(uint8_t* buffer, uint16_t offset) {
    return (uint16_t)buffer[offset] << 8 | (uint16_t)buffer[offset+1];
}

uint8_t Fram::write_to_memory(uint16_t memory_address, uint8_t data) {
    uint8_t ret;
    uint8_t buffer[3];
    uint16_t offset = 0;
    add_to_buffer(&buffer[0], offset, memory_address);
    buffer[2] = data;
    ret = i2c_write(i2c_fram, (uint8_t *)buffer, 3, FRAM_I2C_ADDRESS);
    return ret;
}

uint8_t Fram::write_multiple_bytes(uint16_t memory_address, uint8_t *data_buffer, uint8_t len) {
    uint8_t ret;
    uint8_t* buffer = new uint8_t[len+2];
    uint16_t offset = 0;
    offset = add_to_buffer(&buffer[0], offset, memory_address);
    for(int i = 0; i < len; i++) {
        buffer[offset] = data_buffer[i];
        offset++;
    }
    ret = i2c_write(i2c_fram, (uint8_t *)buffer, len+2, FRAM_I2C_ADDRESS);
    delete[] buffer;
    return ret;
}

uint8_t Fram::read_from_current_address(uint8_t *buffer, uint8_t len) {
    uint8_t ret;
    ret = i2c_read(i2c_fram, (uint8_t*)buffer, len, FRAM_I2C_ADDRESS);
    return ret;
}

uint8_t Fram::read_from_address(uint16_t memory_address, uint8_t *buffer, uint8_t len) {
    uint8_t ret;
    uint8_t address_buffer[2];
    uint16_t offset = 0;
    add_to_buffer(&address_buffer[0], offset, memory_address);
    ret = i2c_write(i2c_fram, (uint8_t*)address_buffer, 2, FRAM_I2C_ADDRESS);
    ret = i2c_read(i2c_fram, (uint8_t*)buffer, len, FRAM_I2C_ADDRESS);
    return ret;
}

uint8_t Fram::save_sensor_data(uint16_t co2, uint16_t temperature_deg_c, uint16_t humidity_percent_rh) {
    uint8_t ret;
    uint8_t buffer[8];
    uint16_t offset = 0;
    offset = add_to_buffer(buffer, offset, address);
    offset = add_to_buffer(buffer, offset, co2);
    offset = add_to_buffer(buffer, offset, temperature_deg_c);
    offset = add_to_buffer(buffer, offset, humidity_percent_rh);
    ret = i2c_write(i2c_fram, (uint8_t*)buffer, 8, FRAM_I2C_ADDRESS);
    address += 6;
    if(address > 0x1fff) {
        address = address - 0x1fff;
    }
    return ret;
}

uint8_t Fram::read_last_sensor_data(uint16_t& co2, uint16_t& temperature_deg_c, uint16_t& humidity_percent_rh) {
    uint8_t buffer[6];
    uint8_t ret;
    uint16_t temp_address = address - 6;
    if(temp_address < 0) {
        temp_address += 0x1fff;
    }
    ret = read_from_address(temp_address, (uint8_t*)&buffer[0], 6);
    if(ret != 0) {
        return ret;
    }
    co2 = combine_to_16b(buffer, 0);
    temperature_deg_c = combine_to_16b(buffer, 2);
    humidity_percent_rh = combine_to_16b(buffer, 4);
    return ret;
}