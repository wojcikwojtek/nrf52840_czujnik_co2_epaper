#include "Arduino_Own.h"

int16_t Arduino_Own::abs(int16_t a) {
    if(a < 0) {
        return -a;
    } else {
        return a;
    }
}

long Arduino_Own::map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t Arduino_Own::bitClear(uint8_t& value, uint8_t bit) {
   	value &= ~(1UL << (bit));
    return value;
}

uint8_t Arduino_Own::bitSet(uint8_t& value, uint8_t bit) {
   	value |= (1UL << (bit));
    return value;
}

uint8_t Arduino_Own::bitRead(uint8_t value, uint8_t bit) {
   	value = (value >> bit) & 0x01;
    return value;
}

char* Arduino_Own::substring(const char* str, uint8_t start, uint8_t end) {
    if(str == nullptr || start < 0 || end < start) {
        return nullptr;
    }
    
    int strLength = strlen(str);

    if(end >= strLength) {
        end = strLength - 1;
    }

    int substringLength = end - start + 1;

    if(substringLength <= 0) {
        return nullptr;
    }

    char* result = new char[substringLength + 1];

    for(int i = 0; i < substringLength; i++) {
        result[i] = str[start + i];
    }

    result[substringLength] = '\0';

    return result;
}