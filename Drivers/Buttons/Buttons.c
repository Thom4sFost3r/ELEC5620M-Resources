/*
 * Buttons.c
 *
 *  Created on: 20 Mar 2022
 *      Author: tom
 */

#include "Buttons.h"

// KEY buttons base address
volatile int *KEY_ptr = (int *)0xFF200050;


/**
 * GetButton
 *
 * Returns the index of the buttons being pressed as a four bit number
 *
 * Methods:
 * 		Points to the address holding the button information
 *
 * Inputs:
 * 		none
 */
char GetButton() {
	return (*KEY_ptr & 0x0F);
}
