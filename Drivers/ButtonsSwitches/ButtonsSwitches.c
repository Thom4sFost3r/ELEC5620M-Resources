/*
 * Buttons.c
 *
 *  Created on: 20 Mar 2022
 *      Author: tom
 */

#include "ButtonsSwitches.h"

// KEY buttons base address
volatile int *KEY_ptr = (int *)0xFF200050;
// Slide switches base address
volatile int *SWITCH_ptr = (int *)0xFF200040;


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

int GetSwitch() {
	return (*SWITCH_ptr & 0x3FF);
}
