#pragma once
///
/// @file hV_Utilities_Common.h
/// @brief Utilities for highView Library Suite
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Nov 2023
/// @version 702
///
/// @copyright (c) Rei Vilo, 2010-2023
/// @copyright Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// The highView Library Suite is shared under the Creative Commons licence Attribution-ShareAlike 4.0 International (CC BY-SA 4.0).
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// @see https://creativecommons.org/licenses/by-sa/4.0/
///
/// @n Consider the Evaluation or Commercial editions for professionals or organisations and for commercial usage
///

// SDK
#include "hV_HAL_Peripherals.h"
#include "hV_List_Boards.h"

#ifndef hV_UTILITIES_RELEASE
///
/// @brief Library release number
///
#define hV_UTILITIES_RELEASE 702

#ifndef minEPD
#define minEPD(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef maxEPD
#define maxEPD(a, b) ((a) > (b) ? (a) : (b))
#endif

///
/// @name Orientation constants
/// @{
///
#define ORIENTATION_PORTRAIT 6 ///< portrait or vertical, higher than large
#define ORIENTATION_LANDSCAPE 7 ///< landscape or horizontal, larger than high
/// @}
///

///
/// @name Utilities
///
/// @{

///
/// @brief Blocking delay
/// @param ms duration in ms
/// @note delay_ms() is blocking, as it doesn't use delay()
///
void delay_ms(uint32_t ms);

///
/// @brief Cosinus
/// @param degreesX100 angle in degrees, x100
/// @return cosinus value, x100
/// @note This function uses integers only.
///
int32_t cos32x100(int32_t degreesX100);

///
/// @brief Sinus
/// @param degreesX100 angle in degrees, x100
/// @return sinus value, x100
/// @note This function uses integers only.
///
int32_t sin32x100(int32_t degreesX100);

/*
///
/// @brief Convert polar to rectangle coordinates
/// @param[in] centerX circle center, x coordinate
/// @param[in] centerY circle center, y coordinate
/// @param[in] angle angle, degrees 0..360°
/// @param[in] radius radius, pixels
/// @param[out] rectangularX x rectangular coordinate
/// @param[out] rectangularY y rectangular coordinate
/// @note This function uses floats.
///
void convertPolar2Rectangle(uint16_t centerX, uint16_t centerY, uint16_t angle, uint16_t radius, uint16_t & rectangularX, uint16_t & rectangularY);

/// @brief Convert rectangle to polar coordinates
/// @param[in] centerX circle center, x coordinate
/// @param[in] centerY circle center, y coordinate
/// @param[in] rectangularX x rectangular coordinate
/// @param[in] rectangularY y rectangular coordinate
/// @param[out] angle angle in degrees 0..360°
/// @param[out] radius radius in pixels
/// @note This function uses floats.
///
void convertRectangle2Polar(uint16_t centerX, uint16_t centerY, uint16_t rectangularX, uint16_t rectangularY, uint16_t & angle, uint16_t & radius);
*/

///
/// @brief UTF-8 to ISO-8859-1 Converter
/// @param s UTF-8 string, input
/// @return ISO-8859-1 string, output
///
/// @see The Unicode Consortium. The Unicode Standard, Version 6.2.0,
/// (Mountain View, CA: The Unicode Consortium, 2012. ISBN 978-1-936213-07-8)
/// http://www.unicode.org/versions/Unicode6.2.0/
///
const char* utf2iso(const char* s);

///
/// @brief Format string
/// @details Based on vsprint
/// @param format format with standard codes
/// @param ... list of values
/// @return string with values formatted
/// @see http://www.cplusplus.com/reference/cstdio/printf/?kw=printf for codes
///
char* formatString(const char * format, ...);

///
/// @brief Remove leading and ending characters
/// @param text input text
/// @return trimmed text
/// @note Removed characters are LF CR TAB SPACE '
///
const char* trimString(const char* text);

/// @}

///
/// @name Range
/// @brief Utilities to check range, set min and max
/// @{

///
/// @brief Check value in range
/// @param value value to check
/// @param valueMin range minimum
/// @param valueMax range maximum
/// @return value within range
/// @note Assuming valueMin < valueMax
/// * if value < valueMin, return valueMin
/// * if value > valueMax, return valueMax
/// Otherwise, if valueMin > valueMax, valueMin and valueMax are swapped
///
uint16_t checkRange(uint16_t value, uint16_t valueMin, uint16_t valueMax);

///
/// @brief Update min and max values
/// @param value value to consider
/// @param valueMin minimum value to update if value < valueMin
/// @param valueMax maximum value to update if value > valueMax
/// @note valueMin < valueMax is assumed
///
void setMinMax(uint16_t value, uint16_t & valueMin, uint16_t & valueMax);

///
/// @brief Round-up
/// @param value value to round-up
/// @param modulo modulo
/// @return rounded-up value / modulo
/// @note Examples
/// * roundUp(7, 8) = 1;
/// * roundUp(8, 8) = 1;
/// * roundUp(9, 8) = 2;
///
uint32_t roundUp(uint32_t value, uint16_t modulo);

// Utilities
///
/// @brief Swap number, int16_t
/// @param a first number
/// @param b second number
///
void swap(int16_t & a, int16_t & b);

///
/// @brief Swap number, uint16_t
/// @param a first number
/// @param b second number
///
void swap(uint16_t & a, uint16_t & b);

///
/// @brief Swap number, uint8_t
/// @param a first number
/// @param b second number
///
void swap(uint8_t & a, uint8_t & b);

/// @}

#endif // hV_UTILITIES_RELEASE
