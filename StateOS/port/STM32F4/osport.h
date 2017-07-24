/******************************************************************************

    @file    StateOS: osport.h
    @author  Rajmund Szymanski
    @date    24.07.2017
    @brief   StateOS port definitions for STM32F4 uC.

 ******************************************************************************

    StateOS - Copyright (C) 2013 Rajmund Szymanski.

    This file is part of StateOS distribution.

    StateOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    StateOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef __STATEOSPORT_H
#define __STATEOSPORT_H

#include <stm32f4xx.h>
#include <osconfig.h>
#include <osdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#define GLUE( a, b, c )            a##b##c
#define  CAT( a, b, c )       GLUE(a, b, c)

/* -------------------------------------------------------------------------- */

#ifndef  OS_TIMER
#define  OS_TIMER             0 /* os uses SysTick as system timer            */
#endif

/* -------------------------------------------------------------------------- */

#if      OS_TIMER

#define  OS_TIM            CAT(TIM,OS_TIMER,)
#define  OS_TIM_CLK_ENABLE CAT(RCC_APB1ENR_TIM,OS_TIMER,EN)
#define  OS_TIM_IRQn       CAT(TIM,OS_TIMER,_IRQn)
#define  OS_TIM_IRQHandler CAT(TIM,OS_TIMER,_IRQHandler)

#define  Counter           OS_TIM->CNT

#endif

/* -------------------------------------------------------------------------- */

#ifndef CPU_FREQUENCY
#error   osconfig.h: Undefined CPU_FREQUENCY value!
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_FREQUENCY

#if      OS_TIMER
#define  OS_FREQUENCY   1000000 /* Hz */
#else
#define  OS_FREQUENCY      1000 /* Hz */
#endif

#endif //OS_FREQUENCY

#if     (OS_TIMER == 0) && (OS_FREQUENCY > 1000)
#error   osconfig.h: Incorrect OS_FREQUENCY value!
#endif

/* -------------------------------------------------------------------------- */

#define  ST_FREQUENCY (CPU_FREQUENCY/8) /* alternate clock source for SysTick */

/* -------------------------------------------------------------------------- */

#ifndef  OS_ROBIN
#define  OS_ROBIN             0 /* system works in cooperative mode           */
#endif

#if     (OS_ROBIN > OS_FREQUENCY)
#error   osconfig.h: Incorrect OS_ROBIN value!
#endif

/* -------------------------------------------------------------------------- */

// force yield system control to the next process
__STATIC_INLINE
void port_ctx_switch( void )
{
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/* -------------------------------------------------------------------------- */

// reset context switch indicator
__STATIC_INLINE
void port_ctx_reset( void )
{
#if OS_TIMER == 0
	SysTick->CTRL;
#endif
#if OS_ROBIN && OS_TIMER
	SysTick->VAL = 0;
#endif
}

/* -------------------------------------------------------------------------- */

// clear time breakpoint
__STATIC_INLINE
void port_tmr_stop( void )
{
#if OS_ROBIN && OS_TIMER
	OS_TIM->DIER = 0;
#endif
}
	
/* -------------------------------------------------------------------------- */

// set time breakpoint
__STATIC_INLINE
void port_tmr_start( uint32_t timeout )
{
#if OS_ROBIN && OS_TIMER
	OS_TIM->CCR1 = timeout;
	OS_TIM->DIER = TIM_DIER_CC1IE;
#else
	(void) timeout;
#endif
}

/* -------------------------------------------------------------------------- */

// force timer interrupt
__STATIC_INLINE
void port_tmr_force( void )
{
#if OS_ROBIN && OS_TIMER
	NVIC_SetPendingIRQ(OS_TIM_IRQn);
#endif
}

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#endif//__STATEOSPORT_H
