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
#include "i2cm.h"

#include "MAX32620FTHRM.h"            // MAX32620FTHRM  MagSem Lib

// 2. Includes - finish
//=============================================================================<

//=============================================================================>
// 3. Definitions - start

#define I2C_MASTER        MXC_I2CM2
#define I2C_SPEED         I2CM_SPEED_400KHZ
#define PMIC_SLAVE_ADDR   0x48
#define PMIC_LED0         0x40
#define PMIC_LED1         0x41
#define PMIC_LED2         0x42
#define PMIC_LED_ON       0x40
#define PMIC_LED_OFF      0x00

// 3. Definitions - finish
//=============================================================================<

//=============================================================================>
// 4. Globals - start
uint8_t LED_Status[] = {0, 0, 0, 0};

// 4. Globals - finish
//=============================================================================<

//=============================================================================>
// 5. Functions - start

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
}

// 6. SetUp - finish
//=============================================================================<

//=============================================================================>
// 7. Loop - start

void loop(void)
{
  int i;
  uint8_t bright[2];
  int count = 0;

  Serial.println("Hello World!");

  sys_cfg_i2cm_t i2cm_sys_cfg;
  ioman_cfg_t io_cfg = IOMAN_I2CM2(1, 0);
  i2cm_sys_cfg.clk_scale = CLKMAN_SCALE_DIV_1;
  i2cm_sys_cfg.io_cfg = io_cfg;
  I2CM_Init(I2C_MASTER, &i2cm_sys_cfg, I2C_SPEED);

  while(1)
  {
    LED_Off(2);
    LED_On(0);
    bright[0] = PMIC_LED0;
    bright[1] = PMIC_LED_ON;
    I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
    for(i=0; i<16; i++)
    {
      bright[0] = PMIC_LED2;
      bright[1] = PMIC_LED_ON + 15 - i;
        I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
      bright[0] = PMIC_LED0;
      bright[1] = PMIC_LED_ON + i;
        I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
        TMR_Delay(MXC_TMR0, MSEC(25));
    }
    bright[0] = PMIC_LED2;
    bright[1] = PMIC_LED_OFF;
    I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
    TMR_Delay(MXC_TMR0, MSEC(100));

    LED_Off(0);
    LED_On(1);
    bright[0] = PMIC_LED1;
    bright[1] = PMIC_LED_ON;
    I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
    for(i=0; i<16; i++)
    {
      bright[0] = PMIC_LED0;
      bright[1] = PMIC_LED_ON + 15 - i;
        I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
      bright[0] = PMIC_LED1;
      bright[1] = PMIC_LED_ON + i;
        I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
        TMR_Delay(MXC_TMR0, MSEC(25));
    }
    bright[0] = PMIC_LED0;
    bright[1] = PMIC_LED_OFF;
    I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
    TMR_Delay(MXC_TMR0, MSEC(100));

    LED_Off(1);
    LED_On(2);
    bright[0] = PMIC_LED2;
    bright[1] = PMIC_LED_ON;
    I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
    for(i=0; i<16; i++)
    {
      bright[0] = PMIC_LED1;
      bright[1] = PMIC_LED_ON + 15 - i;
        I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
      bright[0] = PMIC_LED2;
      bright[1] = PMIC_LED_ON + i;
        I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
        TMR_Delay(MXC_TMR0, MSEC(25));
    }
    bright[0] = PMIC_LED1;
    bright[1] = PMIC_LED_OFF;
    I2CM_Write(I2C_MASTER, PMIC_SLAVE_ADDR, NULL, 0, bright, 2);
    TMR_Delay(MXC_TMR0, MSEC(100));
    Serial.print("count = ");
    Serial.println(count++);
  }
}

// 7. Loop - finish
//=============================================================================<
