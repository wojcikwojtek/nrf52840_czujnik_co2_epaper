#pragma once
///
/// @file hV_HAL_Peripherals.h
/// @brief Light hardware abstraction layer for peripherals
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Sep 2023
/// @version 700
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

#ifndef hV_HAL_PERIPHERALS_RELEASE
///
/// @brief Release
///
#define hV_HAL_PERIPHERALS_RELEASE 700

///
/// @brief SDK library
/// @see References
/// * Arduino SDK https://www.arduino.cc/reference/en/
/// * Energia SDK https://energia.nu/reference/
///
#include "Arduino_Own.h"

///
/// @brief SDK other libraries
///

///
/// @brief Other libraries
///
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#endif // hV_HAL_PERIPHERALS_RELEASE
