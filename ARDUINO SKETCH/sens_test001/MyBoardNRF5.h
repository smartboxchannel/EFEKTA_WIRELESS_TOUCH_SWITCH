/*
  If you don't use an nRF5 board, you can ignore this file.

  This file was part of the "My Sensors nRF5 Boards" board repository
  available at https://github.com/mysensors/ArduinoBoards If you have
  questions, please refer the documentation at
  https://github.com/mysensors/ArduinoHwNRF5 first.

  This file is compatible with ArduinoHwNRF5 >= 0.2.0

  This file allows you to change the pins of internal hardware, like the
  serial port, SPI bus or Wire bus.

  All pins referenced here are mapped via the "g_ADigitalPinMap" Array
  defined in "MyBoardNRF5.cpp" to pins of the MCU.
  
  As an example, if you have at the third position in "g_ADigitalPinMap" the
  12, then all ports referenced in Arduino with 2 are mapped to P0.12. If you
  don't change the "g_ADigitalPinMap" Array, the Arduino pins 0..31 are
  translated to P0.00..P0..31.
   
  ###########################################################################
 
  This file is compatible with ArduinoHwNRF5 > 0.1.0
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry. All right reserved.
  Copyright (c) 2017 Sensnology AB. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _MYBOARDNRF5_H_
#define _MYBOARDNRF5_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Number of pins defined in PinDescription array
#define PINS_COUNT           (32u)
#define NUM_DIGITAL_PINS     (32u)
#define NUM_ANALOG_INPUTS    (8u)
#define NUM_ANALOG_OUTPUTS   (8u)

/* 
 *  LEDs
 *  
 *  This is optional
 *  
 *  With My Sensors, you can use
 *  hwPinMode() instead of pinMode()
 *  hwPinMode() allows to use advanced modes like OUTPUT_H0H1 to drive LEDs.
 *  https://github.com/mysensors/MySensors/blob/development/drivers/NRF5/nrf5_wiring_constants.h
 *
 */
#define PIN_LED1                (16)
#define PIN_LED2                (15)
#define PIN_LED3                (17)
#define RED_LED                 (PIN_LED1)
#define GREEN_LED              (PIN_LED2)
#define BLUE_LED                 (PIN_LED3)
#define INTERRUPT_PIN                 (5)
#define MODE_PIN                 (25)
#define SENS_PIN                 (27)


/* 
 * Analog ports
 *  
 * If you change g_APinDescription, replace PIN_AIN0 with
 * port numbers mapped by the g_APinDescription Array.
 * You can add PIN_AIN0 to the g_APinDescription Array if
 * you want provide analog ports MCU independed, you can add
 * PIN_AIN0..PIN_AIN7 to your custom g_APinDescription Array
 * defined in MyBoardNRF5.cpp
 */
static const uint8_t A0  = ADC_A0;
static const uint8_t A1  = ADC_A1;
static const uint8_t A2  = ADC_A2;
static const uint8_t A3  = ADC_A3;
static const uint8_t A4  = ADC_A4;
static const uint8_t A5  = ADC_A5;
static const uint8_t A6  = ADC_A6;
static const uint8_t A7  = ADC_A7;

/*
 * Serial interfaces
 * 
 * RX and TX are required.
 * If you have no serial port, use unused pins
 * CTS and RTS are optional.
 */
#define PIN_SERIAL_RX       (11)
#define PIN_SERIAL_TX       (12)


#ifdef __cplusplus
}
#endif

#endif
