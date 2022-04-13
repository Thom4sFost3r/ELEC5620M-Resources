/*
 * PrivateTimer.c
 *
 *  Created on: 19 Mar 2022
 *      Author: tom
 *
 */

#include "PrivateTimer.h"

// BASE ADDRESS INIT
// ARM A9 Private Timer Load
volatile unsigned int *private_timer_load		= (unsigned int *) 0xFFFEC600;
// ARM A9 Private Timer Value
volatile unsigned int *private_timer_counter	= (unsigned int *) 0xFFFEC604;
// ARM A9 Private Timer Control
volatile unsigned int *private_timer_control	= (unsigned int *) 0xFFFEC608;
// ARM A9 Private Timer Interrupt
volatile unsigned int *private_timer_interrupt	= (unsigned int *) 0xFFFEC60C;

// CONSTANT INIT
// For the load value register. Calculated using the Private Timers 225MHz frequency
const unsigned int OneCsPeriod = 2250000;
// For prescaler in control register
const unsigned int Prescaler = 0; // Timer lasts ~ 19s

// VARIABLE INIT
// old timer stores the previous centi-second time
unsigned int oldtimervalue = 0;
// new timer stores the current centi-second time
unsigned int newtimervalue;
// timed gap stores the difference between old and new timer values in seconds
double timed_gap;
// fps shows the number of frames per second assuming timed_gap is of one frame
int fps;

/**
 * ConfigTimer
 *
 * Configures the necessary Private Timer related register settings at startup
 *
 * Methods:
 * 		Points to the address of each register required register and sets their values
 *
 * Inputs:
 * 		none
 */
void ConfigTimer() {

    // Set the "Load" value of the timer to max value:
    *private_timer_load = 0xFFFFFFFF;
    // Set the "Prescaler" value, disable ISR (I=0), enable auto reload (A=1) and enable timer
    // disabled (E=0)
    *private_timer_control = (Prescaler << 8) | (0 << 2) | (1 << 1) | (0 << 0);
}

/**
 * ToggleTimer
 *
 * Function used to both enable or disable the timer when changing states.
 * This is so the timer pauses when in rest.
 *
 * Methods:
 * 		Uses the boolean enable bit to chose between setting or clearing the enable bit
 *
 * Inputs:
 * 		en - set 1 to enable timer : 0 to disable
 */
void ToggleTimer(bool en) {

	if(en) {
		*private_timer_control |= 1<<0;
	} else {
		*private_timer_control &= ~(1<<0);
	}
}

/**
 * is1cs
 *
 * Function is used to determine if one centi-second has passed since the last pass.
 * Should only be called while timer is enabled (in state 1).
 *
 * Methods:
 * 		the time, 1cs, is determined by OneCsPeriod as defined in CONSTANT INIT
 *
 * Inputs:
 * 		none
 * Output:
 * 		returns true if the current timer value is a whole multiple of 'centiseconds' from
 * 		the stopwatch start time
 */
bool is1cs() {

	newtimervalue = *private_timer_counter;
    // Check if it is time to blink
    if ((oldtimervalue - newtimervalue) >= OneCsPeriod) {

        // Set old timer value to exactly 1cs from last old timer value.
    	// This prevents the timer from deviating over time.
    	oldtimervalue = oldtimervalue - OneCsPeriod;

        // If the time elapsed is enough return true.
        return true;
    } else {
    	return false;
    }
}

/**
 * PrintTimeGap
 *
 * Function is used to print the time difference between function calls
 * Timer starts when called and print == 0
 * Timer ends when called and print == 1
 *
 * Methods:
 * 		if print is false
 * 			Set the start point of the timer
* 		if print is true
* 			set the end point of the timer
* 			calculate difference between start and end
* 			print difference in centi-seconds
 */
void PrintTimeGap(bool print) {

	int old;
	int new;

	if (print) {
		newtimervalue = *private_timer_counter;
		new = newtimervalue;
		timed_gap = (double)(oldtimervalue - newtimervalue);
		timed_gap = timed_gap / 22500000; // difference in seconds
		fps = (int)(1/timed_gap);
		printf("%d\n",(oldtimervalue-newtimervalue));
		printf("%lf\n",timed_gap);
		printf("Timed Gap = %lf\n", timed_gap);
		printf("fps = %d\n", fps);
	} else {
		oldtimervalue = *private_timer_counter;
		old = oldtimervalue;
	}
}

/**
 * ClearInterruptFlag
 *
 * Clears the interrupt flag if set.
 * No extra functionality as interrupts aren't used in this assignment
 *
 * Methods:
 * 		Uses a pointer to both determine and clear the interrupt bit
 *
 * Inputs:
 * 		none
 */
void ClearInterruptFlag() {

    // if interrupt flag is set, clear flag
    if (*private_timer_interrupt & 0x1) {
        // If the timer interrupt flag is set, clear the flag
        *private_timer_interrupt = 0x1;
    }
}
