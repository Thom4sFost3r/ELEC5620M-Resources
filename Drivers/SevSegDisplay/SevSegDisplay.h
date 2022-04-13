/*
 * SevSegDisplay.h
 *
 *  Created on: 18 Mar 2022
 *      Author: tom
 *
 *      About:
 *  	This file creates the function instances for SevSegDisplay.c
 */

// Allows for bool values
#include <stdbool.h>
// Allows for more arithmetic operators
#include <math.h>

#ifndef SEVSEGDISPLAY_SEVSEGDISPLAY_H_
#define SEVSEGDISPLAY_SEVSEGDISPLAY_H_

// FUNCTION INIT
void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value);
void SevSegWriteOne(unsigned int num, unsigned int display);
void SevSegWriteAll(unsigned int hh, unsigned int mm, unsigned int ss, unsigned int ff);


#endif /* SEVSEGDISPLAY_SEVSEGDISPLAY_H_ */
