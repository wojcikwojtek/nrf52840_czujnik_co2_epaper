#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


class Arduino_Own {
public:
	int16_t abs(int16_t a);
	long map(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
	uint8_t bitClear(uint8_t a, uint8_t b);
	uint8_t bitSet(uint8_t a, uint8_t b);
	uint8_t bitRead(uint8_t a, uint8_t b);
	const char* substring(const char* str, uint8_t start, uint8_t end);
};