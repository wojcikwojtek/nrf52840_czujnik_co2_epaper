#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string>


class Arduino_Own {
public:
	int16_t abs(int16_t a);
	long map(long x, long in_min, long in_max, long out_min, long out_max);
	uint8_t bitClear(uint8_t& value, uint8_t bit);
	uint8_t bitSet(uint8_t& value, uint8_t bit);
	uint8_t bitRead(uint8_t value, uint8_t bit);
	char* substring(const char* str, uint8_t start, uint8_t end);
};