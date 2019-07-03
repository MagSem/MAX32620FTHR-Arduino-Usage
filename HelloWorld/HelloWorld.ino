//=============================================================================>
// 1. Project - start

// Author:    MagSem
// Fork:      Maxim Integrated Firmware
// Data:      03.07.2019
// Details:   Rewrite Maxim Integrated EvKit examples to Arduino       
// uC board:  MAX32620FTHR

// 1. Project - finish
//=============================================================================<

/**
 * @file    
 * @brief   Hello World!
 * @details This example uses the UART to print to a terminal and flashes an LED.
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

//----------------------------------------------------------------------------->
// U8GLIB Display which does not send AC
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//-----------------------------------------------------------------------------<

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
  int count = 0;
  int led = 0;
  int timems = 1000;

  Serial.println("Hello World!");

  while(1)
  {
    LED_On(led);
      Serial.print("LED_Status = ");
      Serial.println(LED_Status[led]);
      Serial.print("count = ");
      Serial.println(count);
    TMR_Delay(MXC_TMR0, MSEC(timems));
    LED_Off(led);
      Serial.print("LED_Status = ");
      Serial.println(LED_Status[led]);
    TMR_Delay(MXC_TMR0, MSEC(timems));
    count++;
    StringToOLED("Hello World!", 0, 10 + led * 15);
    if(led < 3) led++; else led = 0;
  }
}

// 7. Loop - finish
//=============================================================================<
