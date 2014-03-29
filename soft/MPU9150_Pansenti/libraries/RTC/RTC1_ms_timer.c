//  COPYRIGHT FLIPTRONICS (C)   2013
//
//  RTC1_ms_timer.c
//
//  This library module sets up the RTC1 to interrupt every millisecond (actually .0009765625 seconds)
//  It maintains two global variables: RTC1_Milliseconds and RTC1_Seconds, which are both uint32_t
//      RTC1_Milliseconds       overflows every  49 days
//      RTC1_Seconds            overflows every 136 years
//
//  It also supports a user task that is called every millisecond from withing the interrupt routine.
//  The user task is optional, and if use, it must be named RTC1_1ms_user_int_task(void). It must complete
//  in significantly less time than 1 ms.
//
//  Since the RTC1 is an RC ocillator, it is not very accurate. And our interrupt occurs in slightly
//  less than 1 ms, even if the oscillator was perfect. The code trys to compensate for these deficiencies
//  and this can lead to occasional times that RTC1_Milliseconds in not incremented in the interrupt routine.
//  This will occur 24 times, at every 42nd interrupt. This gets us pretty close.
//  Overall the goal is to have RTC1_Milliseconds be incremented 1000 times per second.
//  Even when the increment is suppressed, the user task, if any is called.
//

#include <stdbool.h>
#include "RTC1_ms_timer.h"
#include "nrf.h"
#include "nrf51.h"
#include "nrf51_bitfields.h"

uint32_t RTC1_Milliseconds;
uint32_t RTC1_Seconds;

static uint8_t RTC1_skip_increment_counter;

void setup_RTC1_1ms_timer(void)
{
    //
    //  Set up clock source for RTC (both RTC0 and RTC1
    //
    NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_RC << CLOCK_LFCLKSRC_SRC_Pos);    //  RC oscillator
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;                                         //  Clear it so we can sense when it changes
    NRF_CLOCK->TASKS_LFCLKSTART = 1;                                            //  Start RC oscillator
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        //  Wait for oscillator to start
    }
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;                                         //  Clean up
    //
    //  Config RTC1 to generate an interrupt every .0009765625 seconds
    //
    NRF_RTC1->TASKS_STOP = 1;                                                   //  Must be stopped to be able to write PRESCALER
    RTC1_Milliseconds = 0;
    RTC1_Seconds = 0;
    RTC1_skip_increment_counter = 0;
    NRF_RTC1->PRESCALER = 31;                                                   //  RTC1 clock is 32768 Hz, divided by 32 == 1024Hz

    // Enable TICK event and TICK interrupt:
    NRF_RTC1->EVTENSET = (RTC_EVTENSET_TICK_Enabled << RTC_EVTENSET_TICK_Pos);
    NRF_RTC1->INTENSET = (RTC_INTENSET_TICK_Enabled << RTC_INTENSET_TICK_Pos);

    NVIC_EnableIRQ(RTC1_IRQn);                                                  // Enable Interrupt for RTC1 in the core

    NRF_RTC1->TASKS_START = 1;
}

//
//  If the user does not want an interrupt task that is triggered every millisecond (more or less), then
//  this null function is a stub so the compiler does not complain.
//
void RTC1_1ms_user_int_task(void)
{
    // do absolutely nothing
}

void RTC1_IRQHandler(void)
{
    if (NRF_RTC1->EVENTS_TICK != 0)
    {                                                   //  make sure it is a tick event, every 976.5625 us
        NRF_RTC1->EVENTS_TICK = 0;                      //  clear the event
        RTC1_skip_increment_counter++;                  //  special handling of every 42nd event
        if (RTC1_skip_increment_counter < 42)
        {
            if ((++RTC1_Milliseconds % 1000) == 0)
            {                                           //  check if we are at a multiple of 1000 milliseconds
                RTC1_Seconds++;
            }
        }
        else
        {                                               //  special case for every 42nd tick. No increment of millisecond counter
            RTC1_skip_increment_counter = 0;            //  reset event counter. No need to check for incrementing seconds, as 1000
        }                                               //  is not divisible by 42
        RTC1_1ms_user_int_task();                       //  call the user interrupt routine if supplied
    }

}

unsigned long millis(void)
{
    return RTC1_Milliseconds;
}

void get_ms(long unsigned int *timestamp)
{
    *timestamp = millis();
}
