//=============================================================================>
// 1. Project - start

// Author:    MagSem
// Fork:      Maxim Integrated Firmware
// Data:      12.07.2019
// Details:   Rewrite Maxim Integrated EvKit examples to Arduino       
// uC board:  MAX32620FTHR

// 1. Project - finish
//=============================================================================<

/**
 * @file  
 * @brief   Demonstrates using the nanoring watchdog timer (WDT2)
 *          to wake-up from LP1 and/or reset the program
 *
 * @details When the program starts LED3 is turned on (LP3 mode) and LED2 start
 *          blinking continuously.
 *          Open a terminal program to see interrupt messages.
 *
 *          SW1: Push SW1 to enter LP1 and wake-up based on watchdog timer's
 *               wake-up period (2 seconds).
 *               While in LP1 LED2 stops blinking and LED1 turns on.
 *               After about 2 seconds the watchdog wake-up period expires and
 *               wakes up to LP3 (LED3 on, LED2 continuous blinking).
 *
 *          SW2: Push SW2 to enter LP1 and reset program based on watchdog
 *               timer's reset period (4 seconds).
 *               While in LP1 LED2 stops blinking and LED1 turns on.
 *               After about 4 seconds the watchdog reset period expires
 *               and resets the program. (LED3 blinks three times).
 *
 *          SW3: Push SW3 to trigger a long delay and see the program restart
 *               by blinking LED3 three times.
 *               During the delay LED2 will stop blinking.
 *               This delay is long enough for the reset period to
 *               expire and trigger a reset (4 seconds).
 */

//=============================================================================>
// 2. Includes - start

#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "wdt2.h"
#include "lp.h"
#include "mxc_sys.h"

#include "MAX32620FTHRM.h"            // MAX32620FTHRM  MagSem Lib
#include <U8g2lib.h>                  // Output Oled olikraus Lib

// 2. Includes - finish
//=============================================================================<

//=============================================================================>
// 3. Definitions - start

// 3. Definitions - finish
//=============================================================================<

//=============================================================================>
// 4. Globals - start

uint8_t LED_Status[] = {0, 0, 0, 0};

uint8_t PB1 = P3_0;

// 4. Globals - finish
//=============================================================================<

//=============================================================================>
// 5. Functions - start

//----------------------------------------------------------------------------->
// U8GLIB Display which does not send AC
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

U8G2LOG u8g2log;

// setup the terminal (U8G2LOG) and connect to u8g2 for automatic refresh of the display
// The size (width * height) depends on the selected font and the display

// assume 4x6 font
#define U8LOG_WIDTH  21
#define U8LOG_HEIGHT  7
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
unsigned long t = 0;

void OLEDTerminalInit(void)
{
  u8g2.begin();  
  
  u8g2.setFont(u8g2_font_6x10_mf); // set the font for the terminal window
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(0); // set extra space between lines in pixel, this can be negative
  u8g2log.setRedrawMode(0);   // 0: Update screen with newline, 1: Update screen for every char  
}

// print the output of millis() to the terminal every second
void OLEDPrintMillis(void)
{
  if ( t < millis() )
  {
    t = millis() + 15000;     // every 15 seconds
    u8g2log.print("\f");      // \f = form feed: clear the screen
  }
  u8g2log.print("millis = ");
  u8g2log.print(millis());
  u8g2log.print("\n");
  delay(1000);
}

// Analog Serial.println
void OLEDprintln(char *str, int arg)
{
  u8g2log.print(str);
  if (arg != -32627) u8g2log.print(arg);
  u8g2log.print("\n");
}

//-----------------------------------------------------------------------------<

//----------------------------------------------------------------------------->
void WDT2_Setup()
{
  int enableInSleep = 1;
  //initialize watchdog clock for run and sleep mode
  WDT2_Init(enableInSleep, MXC_V_WDT2_UNLOCK_KEY); //WDT2 uses 8KHz NanoRing clock

  //setup wake-up and reset times
  WDT2_EnableWakeUp(WDT2_PERIOD_2_14_CLKS, MXC_V_WDT2_UNLOCK_KEY); // 2^14/8KHz = 2.048s
  WDT2_EnableReset(WDT2_PERIOD_2_15_CLKS, MXC_V_WDT2_UNLOCK_KEY);  // 2^15/8KHz = 4.096s

  //start watchdog timer
  WDT2_Start(MXC_V_WDT2_UNLOCK_KEY);
}
//-----------------------------------------------------------------------------<

//----------------------------------------------------------------------------->
void WakupFromLP1()
{
  //Clear existing wake-up config
  LP_ClearWakeUpConfig();

  //Clear any event flags
  LP_ClearWakeUpFlags();

  //turn on LED1
  LED_On(1);
  LED_Off(3);

  //reset watchdog before sleep
  WDT2_Reset();

  //global disable interrupt
  __disable_irq();

  //Enter sleep
  LP_EnterLP1();

  //global enable interrupt
  __enable_irq();

  //Wake from sleep and turn on LED3
  LED_Off(1);
  LED_On(3);

  //Clear all wake-up flags
  LP_ClearWakeUpFlags();
}
//-----------------------------------------------------------------------------<

//----------------------------------------------------------------------------->
int WDTResetFlag()
{
  int reset = 0;
  uint32_t flags = WDT2_GetFlags();

  //check for watchdog reset
  if(flags & MXC_F_WDT2_FLAGS_RESET_OUT)
  {
    reset = 1;

    WDT2_ClearFlags(MXC_F_WDT2_FLAGS_RESET_OUT);

    //blink LED3 three times
    int numBlinks = 3;

    while(numBlinks)
    {
      LED_On(3);
      delay(100);
      LED_Off(3);
      delay(100);

      numBlinks--;
    }
  }

  return reset;
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
  // Prepare the User PUSH BUTTON
  pinMode(MAX32620FTHR_PBUT, INPUT_PULLUP);
  // Prepare the external PUSH BUTTON
  pinMode(PB1, INPUT_PULLUP);

  // USB to Serial (PC to uC)
  Serial.begin(0);

  //set LED3 on
  LED_On(3);

  //Initialize Display
  OLEDTerminalInit();
  OLEDprintln("Start:", -32627);
  delay(1000);

  //check if starting at main because of watchdog reset
  if(WDTResetFlag() == 0)
  {
    Serial.println("\n**********Lower Power with Watchdog Demo***********");
    Serial.println("Press a button to start action:");
    Serial.println("SW1 = Enter LP1 and wake-up based on watchdog timer");
    Serial.println("SW2 = Enter LP1 and reset based on watchdog timer");
    Serial.println("SW3 = In run mode (LP3), delay long enough to trigger watchdog reset timeout");
  }

  //Initialize and setup watchdog wake-up and reset times
  WDT2_Setup();

}

// 6. SetUp - finish
//=============================================================================<

//=============================================================================>
// 7. Loop - start

void loop(void)
{

  int SW = PB_Get(MAX32620FTHR_PBUT, LOW);
  //int SW = PB_Get(PB1, HIGH);

  //Push SW1 to enter LP1 and wake-up based on watchdog timer
  if(SW == 1)
  {
    OLEDprintln("Switch = ", SW);
    OLEDprintln("LP1 and wake-up", -32627);
    WakupFromLP1();
  }

  //Push SW2 to enter LP1 and reset based on watchdog timer
  if(SW == 2)
  {
    OLEDprintln("Switch = ", SW);
    OLEDprintln("LP1 and reset", -32627);
    //disable wake-up time for watchdog
    WDT2_DisableWakeUp(MXC_V_WDT2_UNLOCK_KEY);
    WakupFromLP1();
  }

  //Push SW3 to delay long enough to reset program based on watchdog timer
  if(SW == 3)
  {
    OLEDprintln("Switch = ", SW);
    OLEDprintln("Delay and reset", -32627);
    delay(4100);
  }

  //blink LED2
  delay(100);
  LED_On(2);
  delay(100);
  LED_Off(2);

  //Reset watchdog
  WDT2_Reset();

}

// 7. Loop - finish
//=============================================================================<
