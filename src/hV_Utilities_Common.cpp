//
// hV_Utilities_Common.cpp
// Library C++ code
// ----------------------------------
//
// Project highView Library Suite
//
// Created by Rei Vilo, 01 Jun 2013
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
//
// See hV_Utilities_Common.h for references
//
// Release 700: Refactored screen and board functions
//

// Library header
#include "hV_Utilities_Common.h"
#include "stdarg.h"
#include "stdio.h"
#include <string>
using namespace std;

#define PI 3.1415926535897932384626433832795

//void delay_ms(uint32_t ms)
//{
    //uint32_t chrono = millis() + ms;
    //while (millis() < chrono);
//}

char bufferIn[128];
char bufferOut[128];

// Code
// Utilities
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

string formatString(const char * format, ...)
{
    memset(&bufferOut, 0x00, sizeof(bufferOut));
    va_list args;
    va_start(args, format);
    vsprintf(bufferOut, format, args);

    va_end(args);
    return string(bufferOut);
}

string trimString(string text)
{
    string work = "";
    bool flag = true;
    char c;

    uint8_t index;
    uint8_t start, end;

    // Upwards from start
    index = 0;
    flag = true;
    while ((index < text.length()) and flag)
    {
        if ((text[index] != '\n') and (text[index] != '\r') and (text[index] != ' ') and (text[index] != '\t'))
        {
            flag = false;
            start = index;
        }
        index++;
    }

    // Downwards from end
    index = text.length();
    flag = true;
    while ((index > 0) and flag)
    {
        if ((text[index] != '\n') and (text[index] != '\r') and (text[index] != ' ') and (text[index] != '\t'))
        {
            flag = false;
            end = index - 1;
        }
        index--;
    }

    return text.substr(start, end);
}

int32_t cos32x100(int32_t degreesX100)
{
    int32_t i = 1;

    if (degreesX100 < 0)
    {
        i = -i;
        degreesX100 = -degreesX100;
    }

    degreesX100 %= 36000;

    if (degreesX100 > 9000)
    {
        if (degreesX100 < 18000)
        {
            i = -i;
            degreesX100 = 18000 - degreesX100;
        }
        else if (degreesX100 < 27000)
        {
            i = -i;
            degreesX100 = degreesX100 - 18000;
        }
        else
        {
            degreesX100 = 36000 - degreesX100;
        }
    }

    if (degreesX100 < 1000)
    {
        return i * map(degreesX100,    0, 1000, 100, 98);
    }
    else if (degreesX100 < 2000)
    {
        return i * map(degreesX100, 1000, 2000,  98, 93);
    }
    else if (degreesX100 < 3000)
    {
        return i * map(degreesX100, 2000, 3000,  93, 86);
    }
    else if (degreesX100 < 4000)
    {
        return i * map(degreesX100, 3000, 4000,  86, 76);
    }
    else if (degreesX100 < 5000)
    {
        return i * map(degreesX100, 4000, 5000,  76, 64);
    }
    else if (degreesX100 < 6000)
    {
        return i * map(degreesX100, 5000, 6000,  64, 50);
    }
    else if (degreesX100 < 7000)
    {
        return i * map(degreesX100, 6000, 7000,  50, 34);
    }
    else if (degreesX100 < 8000)
    {
        return i * map(degreesX100, 7000, 8000,  34, 17);
    }
    else
    {
        return i * map(degreesX100, 8000, 9000,  17,  0);
    }
}

int32_t sin32x100(int32_t degreesX100)
{
    return cos32x100(degreesX100 + 27000);
}

void convertPolar2Rectangle(uint16_t centerX, uint16_t centerY, uint16_t angle, uint16_t radius, uint16_t & rectangularX, uint16_t & rectangularY)
{
    rectangularX = (uint16_t)(centerX + radius * sin32x100(angle * 100) / 100);
    rectangularY = (uint16_t)(centerY - radius * cos32x100(angle * 100) / 100);
}

void convertRectangle2Polar(uint16_t centerX, uint16_t centerY, uint16_t rectangularX, uint16_t rectangularY, uint16_t & angle, uint16_t & radius)
{
    float fX = (float)rectangularX - centerX;
    float fY = (float)rectangularY - centerY;
    float fZ = sqrt(fX * fX + fY * fY);
    fX /= fZ;
    fY /= fZ;

    if ((fY == 0) and (fX > 0))
    {
        angle = 90;
    }
    else if ((fY == 0) and (fX < 0))
    {
        angle = 270;
    }
    else
    {
        float fAngle = -atan(fX / fY);
        fAngle *= 360 / 2 / PI;

        if (fY > 0)
        {
            fAngle += 180;
        }
        if ((fX < 0) and (fY < 0))
        {
            fAngle += 360;
        }
        angle = (uint16_t)fAngle;
    }
}
string utf2iso(string s)
{
    uint8_t c;

    //s.toCharArray(bufferIn, s.length() + 1);
    strcpy(bufferIn, s.c_str());

    memset(&bufferOut, 0x00, sizeof(bufferOut));

    // Serial.print("> ");
    // Serial.print(s);
    // Serial.print(" : ");
    // for (uint8_t i=0; i<strlen(bufferIn)+1; i++) {
    //  Serial.print((uint8_t)bufferIn[i], HEX);
    //  Serial.print(".");
    // }
    // Serial.println();

    for (uint8_t i = 0; i < strlen(bufferIn); i++)
    {
        c = (uint8_t)bufferIn[i];

        if (c < 0x80)
        {
            bufferOut[strlen(bufferOut)] = c;
        }
        else if (c == 0xc3)
        {
            bufferOut[strlen(bufferOut)] = (bufferIn[++i] + 64);
        }
        else if (c == 0xc2)
        {
            bufferOut[strlen(bufferOut)] = bufferIn[++i];
        }
        else if (c == 0xe2)
        {
            if ((bufferIn[i + 1] == 0x82) && (bufferIn[i + 2] == 0xac))
            {
                bufferOut[strlen(bufferOut)] = 0x80;
                i += 2;
            }
        }
    }
    //    bufferOut[strlen(bufferOut)+1] = 0;

    //    Serial.print("> ");
    // Serial.print(s);
    // Serial.print(" : ");
    // for (uint8_t i=0; i<strlen(bufferOut)+1; i++) {
    //  Serial.print((uint8_t)bufferOut[i], HEX);
    //  Serial.print(".");
    // }
    // Serial.println();

    return bufferOut;
}

uint16_t checkRange(uint16_t value, uint16_t valueMin, uint16_t valueMax)
{
    uint16_t localMin = min(valueMin, valueMax);
    uint16_t localMax = max(valueMin, valueMax);

    return min(max(localMin, value), localMax);
}

void setMinMax(uint16_t value, uint16_t & valueMin, uint16_t & valueMax)
{
    if (value < valueMin)
    {
        valueMin = value;
    }
    if (value > valueMax)
    {
        valueMax = value;
    }
}

uint32_t roundUp(uint32_t value, uint16_t modulo)
{
    uint32_t result = value / modulo;
    if ((value % modulo) > 0)
    {
        result++;
    }
    return result;
}

// Utilities
void swap(uint16_t & a, uint16_t & b)
{
    uint16_t w = a;
    a = b;
    b = w;
}
void swap(int16_t & a, int16_t & b)
{
    int16_t w = a;
    a = b;
    b = w;
}
void swap(uint8_t & a, uint8_t & b)
{
    uint8_t w = a;
    a = b;
    b = w;
}

