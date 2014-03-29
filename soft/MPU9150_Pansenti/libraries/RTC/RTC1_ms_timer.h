/*
 * RTC1_ms_timer.h
 *
 *  Created on: 2013. 8. 17.
 *      Author: Lee joongkeun
 */

#ifndef RTC1_MS_TIMER_H_
#define RTC1_MS_TIMER_H_

void setup_RTC1_1ms_timer(void);
void RTC1_1ms_user_int_task(void);
void RTC1_IRQHandler(void);

unsigned long millis(void);
void get_ms(long unsigned int *timestamp);

#endif /* RTC1_MS_TIMER_H_ */
