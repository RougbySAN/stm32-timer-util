/*
 * timer_utils.c
 *
 *  Created on: Oct 26, 2024
 *      Author: Ibrahim Yahya Ortakaya
 */

#include "timer_util.h"
#include "float.h"

/* Global timer util handler */
timer_util_handler_t timer_util_handler;

/* - Get the time tick from the timer.
 * - Check the overflow interrupt to handle
 * correctly.
 * - Sometimes the CNT register can overflow and
 * become 0 but the overflow flag may not be set yet
 * therefore double read of CNT register is done.
 * */
timer_util_time_t timer_util_get_tick(void)
{
	__IO uint32_t cnt_val = timer_util_handler.timer_handler->CNT;
	// Check for overflow and increment the upper bits if necessary
	if (LL_TIM_IsActiveFlag_UPDATE(timer_util_handler.timer_handler))
	{
		LL_TIM_ClearFlag_UPDATE(timer_util_handler.timer_handler);
#if TIMER_UTIL_TIMER_SIZE == 0
		timer_util_handler.global_time += 0x10000;
#else
		timer_util_handler.global_time += 0x100000000;
#endif
		cnt_val = timer_util_handler.timer_handler->CNT;
	}

	// Return the combined value of the upper bits and current timer count
#if TIMER_UTIL_TIMER_SIZE == 0
	return (timer_util_handler.global_time & 0xFFFFFFFFFFFF0000) |
		   (cnt_val & 0x0000FFFF);
#else
	return (timer_util_handler.global_time & 0xFFFFFFFF00000000) |
		   (cnt_val);
#endif
}


timer_util_time_t timer_util_get_us(void)
{
	return timer_util_get_tick();
}


timer_util_time_t timer_util_get_ms(void)
{
	return US_TO_MS(timer_util_get_us());
}


timer_util_time_t timer_util_get_s(void)
{
	return US_TO_S(timer_util_get_ms());
}


/* Check if the timeout reached from "time" parameter
 * The input parameter should be its own unit what ever it is
 * on your system. */
bool timer_util_check_elapsed(timer_util_time_t *time, timer_util_time_t timeout)
{
	return ((timer_util_get_tick() - (*time)) > timeout);
}


/* Performance measurement functions */
void timer_util_perf_init(timer_util_perf_t *perf_timer)
{
	/* Initialize the performance timer type with default values */
	perf_timer->avg = 0;
	perf_timer->elapsed = 0;
	perf_timer->last_time = 0;
	perf_timer->max = FLT_MIN;
	perf_timer->min = FLT_MAX;
	perf_timer->count = 0;
}


void timer_util_perf_start(timer_util_perf_t *perf_timer)
{
	perf_timer->last_time = timer_util_get_tick();
}


void timer_util_perf_update(timer_util_perf_t *perf_timer)
{
	timer_util_time_t current_time = timer_util_get_tick();
	perf_timer->elapsed = (float)(current_time - perf_timer->last_time);
	perf_timer->last_time = current_time;

	// Update maximum and minimum
	if(perf_timer->elapsed > perf_timer->max)
	{
		perf_timer->max = perf_timer->elapsed;
	}

	if(perf_timer->elapsed < perf_timer->min)
	{
		perf_timer->min = perf_timer->elapsed;
	}

	// Calculate the running average using a weighted average and keep sample count
	perf_timer->count += 1;
	perf_timer->avg += (perf_timer->elapsed - perf_timer->avg) / perf_timer->count;
}


/* Initialize the timer_util module */
timer_util_status_t timer_util_init(TIM_TypeDef *htim)
{
	timer_util_handler.timer_handler = htim;
	timer_util_handler.global_time = 0;

	/* Disable time for initialization */
	LL_TIM_DisableCounter(htim);

	/* Clear the timer counter value */
	htim->CNT = 0;

	/* Disable all interrupts */
	htim->DIER = 0;

	/* Enable Counter Overflow interrupt */
	LL_TIM_EnableIT_UPDATE(htim);

	/* Start the timer */
	LL_TIM_EnableCounter(htim);

	return TIMER_UTIL_OK;
}
