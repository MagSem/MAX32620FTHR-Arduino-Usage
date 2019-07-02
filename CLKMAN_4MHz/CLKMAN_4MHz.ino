//=============================================================================>
// 1. Project - start

// Author:    MagSem
// Fork:      Maxim Integrated Firmware
// Data:      02.07.2019
// Details:   Rewrite Maxim Integrated EvKit examples to Arduino       
// uC board:  MAX32620FTHR

// 1. Project - finish
//=============================================================================<

/**
 * @file
 * @brief      Example using the 4MHz oscillator.
 * @details    This example configures the system to run off the 4MHz
 *             oscillator. It then re-configures to run off of the 96MHz
 *             oscillator. If you do not re-configure the system clock, the
 *             flash loading and debugging will be noticeably slower.
 */

 /* ***************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * $Date: 2017-05-02 13:57:41 -0500 (Tue, 02 May 2017) $
 * $Revision: 27735 $
 *
 **************************************************************************** */

//=============================================================================>
// 2. Includes - start

#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "tmr_utils.h"

#include "MAX32620FTHRM.h"            // MAX32620FTHRM  MagSem Lib
#include <U8g2lib.h>                  // Output Oled 1306

// 2. Includes - finish
//=============================================================================<

//=============================================================================>
// 3. Definitions - start

// 3. Definitions - finish
//=============================================================================<

//=============================================================================>
// 4. Globals - start
uint8_t LED_Status[] = {0, 0, 0, 0};

// 4. Globals - finish
//=============================================================================<

//=============================================================================>
// 5. Functions - start

// U8GLIB Display which does not send AC
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//----------------------------------------------------------------------------->
// Send String to OLED to x-y coordinats (0,0 - left up corner)
void StringToOLED(char *s, uint8_t x, uint8_t y)
{
  u8g2.clearBuffer();                 // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(x, y, s);              // write something to the internal memory
  u8g2.sendBuffer();                  // transfer internal memory to the display
}
//-----------------------------------------------------------------------------<

// 5. Functions - finish
//=============================================================================<

//=============================================================================>
// 6. SetUp - start

void setup(void)
{
  // 96 MHz very fast for first print
  delay(300);

  // Prepare the User LEDs
  pinMode(MAX32620FTHR_RLED, OUTPUT);
  pinMode(MAX32620FTHR_GLED, OUTPUT);
  pinMode(MAX32620FTHR_BLED, OUTPUT);

  // USB to Serial (PC to uC)
  Serial.begin(0);

  // Initialize the OLED 
  u8g2.begin();

}

// 6. SetUp - finish
//=============================================================================<

//=============================================================================>
// 7. Loop - start

void loop(void)
{
  int i = 5;

  while(i--)
  {
    // Set the System clock to the 96MHz oscillator
    CLKMAN_SetSystemClock(CLKMAN_SYSTEM_SOURCE_96MHZ, CLKMAN_SYSTEM_SCALE_DIV_1);
    LED_On(2);
    LED_Off(1);
    StringToOLED("96 Mhz clock", 0, 10);
    TMR_Delay(MXC_TMR0, SEC(1));

    // Set the System clock to the 4MHz oscillator
    CLKMAN_SetSystemClock(CLKMAN_SYSTEM_SOURCE_4MHZ, CLKMAN_SYSTEM_SCALE_DIV_1);
    LED_Off(2);
    LED_On(1);
    StringToOLED("4 Mhz clock", 0, 10);
    TMR_Delay(MXC_TMR0, SEC(1));
  }
    
  // Set the System clock to the 96MHz oscillator
  CLKMAN_SetSystemClock(CLKMAN_SYSTEM_SOURCE_96MHZ, CLKMAN_SYSTEM_SCALE_DIV_1);
  LED_On(2);
  LED_Off(1);   
  StringToOLED("96 Mhz clock", 0, 10);

  while(1) {}
}

// 7. Loop - finish
//=============================================================================<
