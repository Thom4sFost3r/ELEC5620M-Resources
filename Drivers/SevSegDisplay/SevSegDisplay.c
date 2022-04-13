/*
 * SevSegDisplay.c
 *
 *  Created on: 18 Mar 2022
 *      Author: tom
 *
 *  About:
 *  	This file defines the functions controlling the Seven Segment Displays
 */

#include "SevSegDisplay.h"

// BASE ADDRESS INIT
// The lo_ptr drives HEX3 to HEX0
// The hi_ptr drive HEX5 and HEX4
volatile unsigned char *sevseg_base_lo_ptr = (unsigned char*) 0xFF200020;
volatile unsigned char *sevseg_base_hi_ptr = (unsigned char*) 0xFF200030;

// CONSTANT INIT
// There are four displays attached to the lo address.
#define SEVENSEG_IDX_DISPLAYS_LO 4
// There are two displays attached to the hi address.
#define SEVENSEG_IDX_DISPLAYS_HI 2

// VARIABLE INIT

/**
 * DE1SoC_SevenSeg_Write
 *
 * Disclaimer:	This function was created by Harry Clegg on 12 Feb 2021.
 * 				Created for ELEC5620M Assignment 1
 * 				- https://github.com/leeds-embedded-systems/ELEC5620M-Assignment1-Thom4sFost3r/blob/main/1-A-SevenSegDisplay/DE1SoC_SevenSeg/DE1SoC_SevenSeg.c
 *
 * Function that writes 'value' into the register controlling the Seven Seg Display
 * at index 'display'
 *
 * Methods:
 * 		Uses global constants to determine whether lo or hi address needs writing to
 * 		Uses 'if' statement to distinguish between the the lo and hi address'
 *
 * Inputs:
 * 		display: 	The index of the seven segment display
 * 		value:		The value to be written into the register
 */
void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value) {
    // Select between the two addresses so that the higher level functions
    // have a seamless interface.
    if (display < SEVENSEG_IDX_DISPLAYS_LO) {
        // If we are targeting a low address, use byte addressing to access
        // directly.
    	sevseg_base_lo_ptr[display] = value;
    } else {
        // If we are targeting a high address, shift down so byte addressing
        // works.
        display = display - SEVENSEG_IDX_DISPLAYS_LO;
        sevseg_base_hi_ptr[display] = value;
    }
}

/**
 * SevSegWriteOne
 *
 * Function that converts a single digit decimal integer 'num' (0-9) to a value to be written into the Seven Seg
 * Display register at index 'display'
 *
 * Methods:
 * 		input integer converted to a binary number
 * 		values for each segment are determined via boolean expressions
 * 		- Boolean expressions from - https://www.electronicshub.org/bcd-7-segment-led-display-decoder-circuit/
 * 		Uses function DE1SoC_SevenSeg_Write to write to display
 *
 * Inputs:
 * 		display: 	The index of the seven segment display
 * 		num:		The single digit decimal integer to be displayed
 */
void SevSegWriteOne(unsigned int num, unsigned int display) {

	// move initialisation to .h file
	// For int to bin conversion
	int i = 0;
	int div = num;
	bool bits[4] = {0,0,0,0};
	// Each bit represents each segment of the display
	bool segs[7] = {0,0,0,0,0,0,0};
	unsigned char seg = 0;

	// Convert the input to their binary values bits
	for(i = 0; i <=3; i++){
		bits[i] = div%2;
		div = div/2;
	}

	// boolean expressions for each of the seven segments.
	// See source link in function description
	segs[0] = bits[3] || bits[1] || (bits[2]&&bits[0]) || (!bits[2]&&!bits[0]);
	segs[1] = !bits[2] || (!bits[1]&&!bits[0]) || (bits[1]&&bits[0]);
	segs[2] = bits[2] || !bits[1] || bits[0];
	segs[3] = (!bits[2]&&!bits[0]) || (bits[1]&&!bits[0]) || (bits[2]&&!bits[1]&&bits[0]) || (!bits[2]&&bits[1]) || bits[3];
	segs[4] = (!bits[2]&&!bits[0]) || (bits[1]&&!bits[0]);
	segs[5] = bits[3] || (!bits[1]&&!bits[0]) || (bits[2]&&!bits[1]) || (bits[2]&&!bits[0]);
	segs[6] = bits[3] || (bits[2]&&!bits[1]) || (!bits[2]&&bits[1]) || (bits[1]&&!bits[0]);

	// Convert binary value segs to one unsigned char seg
	for(i=0; i<7; i++){
		seg += segs[i] * pow(2,i);
	}

	// write
	DE1SoC_SevenSeg_Write(display, seg);

}

/**
 * SevSegWriteAll
 *
 * Determines if the value to be displayed should be in hh.mm.ss or mm.ss.ff format
 * Function that splits three separate two digit decimal integers (0-99) into six single digit decimal integers.
 * Sends each of the 6 digits to be displayed
 *
 * Methods:
 * 		switches to hour based time format if at least one hour has passed
 * 		Uses division by 10 to separate the most significant digit
 * 		Uses modulo by 10 to separate the least significant digit
 * 		Uses the SevSegWriteOne function to write each bit
 *
 *
 * Inputs:
 * 		hh: hours to display
 * 		mm: minutes to display
 * 		ss: seconds to display
 * 		ff: centi-seconds to display
 */
void SevSegWriteAll(unsigned int hh, unsigned int mm, unsigned int ss, unsigned int ff) {

	if(hh) {
		SevSegWriteOne(ss%10,0);
		SevSegWriteOne(ss/10,1);
		SevSegWriteOne(mm%10,2);
		SevSegWriteOne(mm/10,3);
		SevSegWriteOne(hh%10,4);
		SevSegWriteOne(hh/10,5);
	} else {
		SevSegWriteOne(ff%10,0);
		SevSegWriteOne(ff/10,1);
		SevSegWriteOne(ss%10,2);
		SevSegWriteOne(ss/10,3);
		SevSegWriteOne(mm%10,4);
		SevSegWriteOne(mm/10,5);
	}

}
