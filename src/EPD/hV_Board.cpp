//
// hV_Board.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 16 Aug 2023
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
//

// Library header
#include "hV_Board.h"

hV_Board::hV_Board()
{
    ;
}

void hV_Board::b_begin(pins_t board, uint8_t family, uint16_t delayCS)
{
    b_pin = board;
    b_family = family;
    b_delayCS = delayCS;
}

void hV_Board::b_reset(uint32_t ms1, uint32_t ms2, uint32_t ms3, uint32_t ms4, uint32_t ms5)
{
    //delay(ms1); // delay 5ms
    k_msleep(ms1);
    //digitalWrite(b_pin.panelReset, HIGH); // RES# = 1
    EPD_RST_1;
    //delay(ms2); // delay 5ms
    k_msleep(ms2);
    //digitalWrite(b_pin.panelReset, LOW);
    EPD_RST_0;
    //delay(ms3);
    k_msleep(ms3);
    //digitalWrite(b_pin.panelReset, HIGH);
    EPD_RST_1;
    //delay(ms4);
    k_msleep(ms4);
    //digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
    EPD_CS_1;
    //delay(ms5);
    k_msleep(ms5);
}

void hV_Board::b_waitBusy(bool state)
{
    // LOW = busy, HIGH = ready
    //while (digitalRead(b_pin.panelBusy) != state)
    //{
        //delay(32); // non-blocking
    //}
    while(isEPD_BUSY != state) {
        k_msleep(32);
    }
}

void hV_Board::b_suspend()
{
    // Not implemented
}

void hV_Board::b_resume()
{
    // Not implemented
}

void hV_Board::b_sendIndexFixed(uint8_t index, uint8_t data, uint32_t size)
{
    // Not implemented
}

void hV_Board::b_sendIndexData(uint8_t index, uint8_t * data, uint32_t size)
{
    //digitalWrite(b_pin.panelDC, LOW); // DC Low
    EPD_DC_0;
    //digitalWrite(b_pin.panelCS, LOW); // CS Low
    EPD_CS_0;
    //if (b_family == FAMILY_LARGE)
    //{
        //if (b_pin.panelCSS != NOT_CONNECTED)
        //{
            //digitalWrite(b_pin.panelCSS, LOW);
        //}
        //delayMicroseconds(450); // 450 + 50 = 500
    //}
    //delayMicroseconds(b_delayCS);
    k_usleep(b_delayCS);
    //SPI.transfer(index);
    struct spi_buf bufs[] = {
        {
            .buf = &index,
            .len = 1
        },
    };
    struct spi_buf_set tx = {
        .buffers = bufs,
        .count = 1
    };
    spi_write(spi_dev, &spi_cfg, &tx);
    //delayMicroseconds(b_delayCS);
    k_usleep(b_delayCS);
    //if (b_family == FAMILY_LARGE)
    //{
        //if (b_pin.panelCSS != NOT_CONNECTED)
        //{
            //delayMicroseconds(450); // 450 + 50 = 500
            //digitalWrite(b_pin.panelCSS, HIGH);
        //}
    //}
    //digitalWrite(b_pin.panelCS, HIGH); // CS High
    EPD_CS_1;
    //digitalWrite(b_pin.panelDC, HIGH); // DC High
    EPD_DC_1;
    //digitalWrite(b_pin.panelCS, LOW); // CS Low
    EPD_CS_0;
    //if (b_family == FAMILY_LARGE)
    //{
        //if (b_pin.panelCSS != NOT_CONNECTED)
        //{
            //digitalWrite(b_pin.panelCSS, LOW); // CSS Low
            //delayMicroseconds(450); // 450 + 50 = 500
        //}
    //}
    //delayMicroseconds(b_delayCS);
    k_usleep(b_delayCS);
    for (uint32_t i = 0; i < size; i++)
    {
        //SPI.transfer(data[i]);
        struct spi_buf bufs1[] = {
            {
                .buf = static_cast<void*>(&data[i]),
                .len = 1
            },
        };
        struct spi_buf_set tx1 = {
            .buffers = bufs1,
            .count = 1
        };
        spi_write(spi_dev, &spi_cfg, &tx1);
    }
    //delayMicroseconds(b_delayCS);
    k_usleep(b_delayCS);
    //if (b_family == FAMILY_LARGE)
    //{
        //if (b_pin.panelCSS != NOT_CONNECTED)
        //{
            //delayMicroseconds(450); // 450 + 50 = 500
            //digitalWrite(b_pin.panelCSS, HIGH);
        //}
    //}
    //digitalWrite(b_pin.panelCS, HIGH); // CS High
    EPD_CS_1;
}

// Software SPI Master protocol setup
void hV_Board::b_sendIndexDataBoth(uint8_t index, uint8_t * data, uint32_t size)
{
    //digitalWrite(b_pin.panelDC, LOW); // DC Low = Command
    EPD_DC_0;
    //digitalWrite(b_pin.panelCS, LOW); // CS Low = Select
    EPD_CS_0;
    //if (b_pin.panelCSS != NOT_CONNECTED)
    //{
        //digitalWrite(b_pin.panelCSS, LOW); // CS Low = Select
    //}

    //delayMicroseconds(b_delayCS); // Longer delay for large screens
    k_usleep(b_delayCS);
    //SPI.transfer(index);
    struct spi_buf bufs[] = {
        {
            .buf = &index,
            .len = 1
        },
    };
    struct spi_buf_set tx = {
        .buffers = bufs,
        .count = 1
    };
    spi_write(spi_dev, &spi_cfg, &tx);
    //elayMicroseconds(b_delayCS); // Longer delay for large screens
    k_usleep(b_delayCS);

    // digitalWrite(b_pin.panelCS, HIGH); // CS High = Unselect
    //digitalWrite(b_pin.panelDC, HIGH); // DC High = Data
    EPD_DC_1;
    // digitalWrite(b_pin.panelCS, LOW); // CS Low = Select

    //delayMicroseconds(b_delayCS); // Longer delay for large screens
    k_usleep(b_delayCS);
    for (uint32_t i = 0; i < size; i++)
    {
        //SPI.transfer(data[i]);
        struct spi_buf bufs1[] = {
            {
                .buf = static_cast<void*>(&data[i]),
                .len = 1
            },
        };
        struct spi_buf_set tx1 = {
            .buffers = bufs1,
            .count = 1
        };
        spi_write(spi_dev, &spi_cfg, &tx1);
    }
    //delayMicroseconds(b_delayCS); // Longer delay for large screens
    k_usleep(b_delayCS);

    //digitalWrite(b_pin.panelCS, HIGH); // CS High = Unselect
    EPD_CS_1;
    //if (b_pin.panelCSS != NOT_CONNECTED)
    //{
        //digitalWrite(b_pin.panelCSS, HIGH); //  CS High = Unselect
    //}
}

void hV_Board::b_sendIndexDataMaster(uint8_t index, uint8_t * data, uint32_t size)
{
    //if (b_pin.panelCSS != NOT_CONNECTED)
    //{
        //digitalWrite(b_pin.panelCSS, HIGH); // CS slave HIGH
    //}
    //digitalWrite(b_pin.panelDC, LOW); // DC Low = Command
    EPD_DC_0;
    //digitalWrite(b_pin.panelCS, LOW); // CS Low = Select
    EPD_CS_0;
    //delayMicroseconds(500);
    k_usleep(500);
    //SPI.transfer(index);
    struct spi_buf bufs[] = {
        {
            .buf = &index,
            .len = 1
        },
    };
    struct spi_buf_set tx = {
        .buffers = bufs,
        .count = 1
    };
    spi_write(spi_dev, &spi_cfg, &tx);
    //delayMicroseconds(500);
    k_usleep(500);
    //digitalWrite(b_pin.panelCS, HIGH); // CS High = Unselect
    EPD_CS_1;
    //digitalWrite(b_pin.panelDC, HIGH); // DC High = Data
    EPD_DC_1;
    //digitalWrite(b_pin.panelCS, LOW); // CS Low = Select
    EPD_CS_0;
    //delayMicroseconds(500);
    k_usleep(500);

    for (uint32_t i = 0; i < size; i++)
    {
        //SPI.transfer(data[i]);
        struct spi_buf bufs1[] = {
            {
                .buf = static_cast<void*>(&data[i]),
                .len = 1
            },
        };
        struct spi_buf_set tx1 = {
            .buffers = bufs1,
            .count = 1
        };
        spi_write(spi_dev, &spi_cfg, &tx1);
    }
    //delayMicroseconds(500);
    k_usleep(500);
    //digitalWrite(b_pin.panelCS, HIGH); // CS High= Unselect
    EPD_CS_1;
}

// Software SPI Slave protocol setup
void hV_Board::b_sendIndexDataSlave(uint8_t index, uint8_t * data, uint32_t size)
{
    //digitalWrite(b_pin.panelCS, HIGH); // CS Master High
    EPD_CS_1;
    //digitalWrite(b_pin.panelDC, LOW); // DC Low= Command
    EPD_DC_0;
    //if (b_pin.panelCSS != NOT_CONNECTED)
    //{
        //digitalWrite(b_pin.panelCSS, LOW); // CS slave LOW
    //}

    //delayMicroseconds(500);
    k_usleep(500);
    //SPI.transfer(index);
    struct spi_buf bufs[] = {
        {
            .buf = &index,
            .len = 1
        },
    };
    struct spi_buf_set tx = {
        .buffers = bufs,
        .count = 1
    };
    spi_write(spi_dev, &spi_cfg, &tx);
    //delayMicroseconds(500);
    k_usleep(500);

    //if (b_pin.panelCSS != NOT_CONNECTED)
    //{
        //digitalWrite(b_pin.panelCSS, HIGH); // CS slave HIGH
    //}

    //digitalWrite(b_pin.panelDC, HIGH); // DC High = Data
    EPD_DC_1;

    //if (b_pin.panelCSS != NOT_CONNECTED)
    //{
        //digitalWrite(b_pin.panelCSS, LOW); // CS slave LOW
    //}

    //delayMicroseconds(500);
    k_usleep(500);

    for (uint32_t i = 0; i < size; i++)
    {
        //SPI.transfer(data[i]);
        struct spi_buf bufs1[] = {
            {
                .buf = static_cast<void*>(&data[i]),
                .len = 1
            },
        };
        struct spi_buf_set tx1 = {
            .buffers = bufs1,
            .count = 1
        };
        spi_write(spi_dev, &spi_cfg, &tx1);
    }
    //delayMicroseconds(500);
    k_usleep(500);
    //if (b_pin.panelCSS != NOT_CONNECTED)
    //{
        //digitalWrite(b_pin.panelCSS, HIGH); // CS slave HIGH
    //}
}

void hV_Board::b_sendCommand8(uint8_t command)
{
    //digitalWrite(b_pin.panelDC, LOW);
    EPD_DC_0;
    //digitalWrite(b_pin.panelCS, LOW);
    EPD_CS_0;

    //SPI.transfer(command);
    struct spi_buf bufs[] = {
        {
            .buf = &command,
            .len = 1
        },
    };
    struct spi_buf_set tx = {
        .buffers = bufs,
        .count = 1
    };
    spi_write(spi_dev, &spi_cfg, &tx);

    //digitalWrite(b_pin.panelCS, HIGH);
    EPD_CS_1;
}

void hV_Board::b_sendCommandData8(uint8_t command, uint8_t data)
{
    //digitalWrite(b_pin.panelDC, LOW); // LOW = command
    EPD_DC_0;
    //digitalWrite(b_pin.panelCS, LOW);
    EPD_CS_0;

    //SPI.transfer(command);
    struct spi_buf bufs[] = {
        {
            .buf = &command,
            .len = 1
        },
    };
    struct spi_buf_set tx = {
        .buffers = bufs,
        .count = 1
    };
    spi_write(spi_dev, &spi_cfg, &tx);

    //digitalWrite(b_pin.panelDC, HIGH); // HIGH = data
    EPD_DC_1;
    //SPI.transfer(data);
    struct spi_buf bufs1[] = {
        {
            .buf = &data,
            .len = 1
        },
    };
    struct spi_buf_set tx1 = {
        .buffers = bufs1,
        .count = 1
    };
    spi_write(spi_dev, &spi_cfg, &tx1);

    //digitalWrite(b_pin.panelCS, HIGH);
    EPD_CS_1;
}

//
// === Miscellaneous section
//
pins_t hV_Board::getBoardPins()
{
    return b_pin;
}
//
// === End of Miscellaneous section
//
