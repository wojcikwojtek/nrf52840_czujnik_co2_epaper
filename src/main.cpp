#include <stdint.h>
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>

#include "scd40.h"
#include "fram.h"
#include "PDLS_EXT3_Basic_Global.h"
#include "hV_HAL_Peripherals.h"
#include "hV_Configuration.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000
#define SCD40_I2C_ADDRESS 0x62 
#define FRAM_I2C_ADDRESS 0x50
#define I2C_NODE DT_NODELABEL(i2c0)
//#define SPI_NODE DT_NODELABEL(spi3)
//#define GPIO_NODE DT_NODELABEL(gpio1)

static const struct device* i2c_test_dev = DEVICE_DT_GET(I2C_NODE);
//static const struct device* spi_dev = DEVICE_DT_GET(SPI_NODE);
//static const struct device* gpio_dev = DEVICE_DT_GET(GPIO_NODE);

Screen_EPD_EXT3 myScreen(eScreen_EPD_EXT3_154);

void wait(uint8_t second) {
	for(uint8_t i = second; i > 0; i--) {
		printk(" > %d \r", i);
		k_msleep(1000);
	}
	printk("    \r");
}

void displayFonts(uint16_t co2, uint16_t temperature_deg_c, uint16_t humidity_percent_rh) {
	uint16_t y = 10;
	myScreen.setOrientation(7);

	myScreen.selectFont(Font_Terminal12x16);

	myScreen.gText(10, y, "CO2:", myColours.red);
	y += myScreen.characterSizeY();
	myScreen.gText(10, y, formatString("%i ppm", co2));
	y += myScreen.characterSizeY();

	myScreen.gText(10, y, "Temperature:", myColours.red);
	y += myScreen.characterSizeY();
	myScreen.gText(10, y, formatString("%i deg C", temperature_deg_c));
	y += myScreen.characterSizeY();

	myScreen.gText(10, y, "Humidity:", myColours.red);
	y += myScreen.characterSizeY();
	myScreen.gText(10, y, formatString("%i RH", humidity_percent_rh));
	y += myScreen.characterSizeY();
	/*
	myScreen.selectFont(Font_Terminal12x16);
	
	myScreen.gText(10, y, myScreen.WhoAmI(), myColours.red);
	y += myScreen.characterSizeY();
	myScreen.gText(10, y, formatString("%i x %i", myScreen.screenSizeX(), myScreen.screenSizeY()), myColours.red);
	y += myScreen.characterSizeY();
	y += myScreen.characterSizeY();

	myScreen.selectFont(Font_Terminal6x8);
	myScreen.gText(10, y, "Terminal6x8");
	y += myScreen.characterSizeY();

	myScreen.selectFont(Font_Terminal8x12);
	myScreen.gText(10, y, "Terminal8x12");
	y += myScreen.characterSizeY();

	myScreen.selectFont(Font_Terminal12x16);
	myScreen.gText(10, y, "Terminal12x16");
	y += myScreen.characterSizeY();

	myScreen.selectFont(Font_Terminal16x24);
	myScreen.gText(10, y, "Terminal16x24");
	y += myScreen.characterSizeY();
	*/
	myScreen.flush();
}

void test_i2c() {
	int err;
	uint8_t buf[1] = {1};
	struct i2c_msg msg;
	msg.buf = (uint8_t *)buf;
	msg.len = 1;
	msg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;
	for(uint16_t address = 0; address < 127; address++) {
		err = i2c_transfer(i2c_test_dev, &msg, 1, address);
		if(err == 0) {
			printk("Device found at 0x%x\n", address);
		}
	}
}

int main()
{	
	printk("Screen init\n");
	myScreen.begin();
	//myScreen.clear();
	//myScreen.flush();
	//return 0;
	//kod do testowania pamieci 
	test_i2c();
    int ret;

	if(!device_is_ready(i2c_test_dev)) {
		printk("i2c_dev is not ready\n");
		return 0;
	} 
	/*
	uint8_t buffer[8] = {0x00, 0x02, 0x23, 0x67, 0x01, 0x02, 0x03, 0x04};
	ret = i2c_write(i2c_test_dev, (uint8_t *)buffer, 8, FRAM_I2C_ADDRESS);
	if(ret != 0) {
		printk("Write to fram memory failed\n");
	} 
	uint8_t read_buffer[6];
	uint8_t address_buffer[2] = {0x00, 0x02};
	ret = i2c_write(i2c_test_dev, (uint8_t*)address_buffer, 2, FRAM_I2C_ADDRESS);
	ret = i2c_read(i2c_test_dev, (uint8_t *)read_buffer, 6, FRAM_I2C_ADDRESS);
	if(ret != 0) {
		printk("Read from fram memory failed\n");
	} else {
		for(int i = 0; i < 6; i++) {
			printk("Read value = 0x%x\n", read_buffer[i]);
		}
	}
	*/
	//kod to testowania czujnika 
    SCD40 czujnik;
	Fram pamiec;
    ret = czujnik.scd4x_stop_periodic_measurement();
	if(ret != 0) {
		printk("Failed to stop peariodic measurements\n");
	}

	ret = czujnik.scd4x_start_periodic_measurement();
	if(ret!=0) {
		printk("Failed to start periodic measurement\n");
	} 

	while (1) {

		uint16_t co2;
		uint16_t temperature_deg_c, humidity_percent_rh;
		int32_t temperature_m_deg_c, humidity_m_percent_rh;
		ret = czujnik.scd4x_read_measurement(&co2, &temperature_m_deg_c, &humidity_m_percent_rh);
		temperature_deg_c = temperature_m_deg_c/1000;
		humidity_percent_rh = humidity_m_percent_rh/1000;
		if(ret != 0) {
			//printk("Failed to read measurments\n");
		} else {
			printk("CO2: %d\nTemperature: %d\nHumidity: %d\n", co2, temperature_deg_c, humidity_percent_rh);
			myScreen.clear();
			displayFonts(co2, temperature_deg_c, humidity_percent_rh);
			ret = pamiec.save_sensor_data(co2, temperature_deg_c, humidity_percent_rh);
			uint16_t a, b, c;
			ret = pamiec.read_last_sensor_data(a, b, c);
			printk("Odczytanie z pamieci wartosci:\n");
			printk("CO2: %d\nTemperature: %d\nHumidity: %d\n", a, b, c);
			wait(8);
		}

		//k_msleep(SLEEP_TIME_MS);
	}
    return 0;
}
