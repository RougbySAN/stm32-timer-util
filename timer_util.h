/*
 * timer_util.h
 *
 *  Created on: Oct 26, 2024
 *      Author: Ibrahim Yahya Ortakaya
 */

#ifndef TIMER_UTIL_TIMER_UTIL_H_
#define TIMER_UTIL_TIMER_UTIL_H_

#include "tim.h"
#include "stdbool.h"

#define TIMER_UTIL_TIMER_SIZE	1	// 0 = 16 bit timer
									// 1 = 32 bit timer


// Time Conversion to Tick Macros
#define US_TO_TICK(us) ((us))
#define MS_TO_TICK(ms) ((ms) * 1000)
#define S_TO_TICK(s)	 ((s) * 1000 * 1000)


// Time Conversion Macros
#define US_TO_MS(us)  ((us) / 1000)          // Convert microseconds to milliseconds
#define US_TO_S(us)   ((us) / 1000000)       // Convert microseconds to seconds
#define MS_TO_US(ms)  ((ms) * 1000)          // Convert milliseconds to microseconds
#define MS_TO_S(ms)   ((ms) / 1000)          // Convert milliseconds to seconds
#define S_TO_US(s)    ((s) * 1000000)        // Convert seconds to microseconds
#define S_TO_MS(s)    ((s) * 1000)           // Convert seconds to milliseconds


/* TIMER UTIL STATUS */
typedef enum
{
	TIMER_UTIL_OK,
	TIMER_UTIL_ERROR,
	TIMER_UTIL_TIMEOUT,
	TIMER_UTIL_BUSY,
}timer_util_status_t;


/* TIMER UTIL TIME TYPE
 * timer_util_time_t is the timer tick
 * holder that is 8 byte long. This type
 * is used to carry the outputs of the
 * module and inputs are made with ! */
typedef uint64_t timer_util_time_t;


typedef struct
{
	TIM_TypeDef *timer_handler;
	timer_util_time_t global_time;
}timer_util_handler_t;


/* TIMER UTIL PERFORMANCE MEASUREMENT TYPE
 * The handle is used for performance
 * measurements by the user. */
typedef struct
{
	timer_util_time_t last_time;
	float avg;
	float max;
	float min;
	float elapsed;
	uint32_t count;
}timer_util_perf_t;


/* Functions for current time retrieval */
timer_util_time_t timer_util_get_tick(void);
timer_util_time_t timer_util_get_us(void);
timer_util_time_t timer_util_get_ms(void);
timer_util_time_t timer_util_get_s(void);


/* Check if the timeout reached from "time" parameter
 * The input parameter should be its own unit what ever it is
 * on your system. */
bool timer_util_check_elapsed(timer_util_time_t *time, timer_util_time_t timeout);


/* Performance measurement functions */
void timer_util_perf_init(timer_util_perf_t *perf_timer);
void timer_util_perf_start(timer_util_perf_t *perf_timer);
void timer_util_perf_update(timer_util_perf_t *perf_timer);
void timer_util_perf_update_elpased(timer_util_perf_t *perf_timer);


/* Initialize the timer_util module */
timer_util_status_t timer_util_init(TIM_TypeDef *htim);

#endif /* TIMER_UTIL_TIMER_UTIL_H_ */
