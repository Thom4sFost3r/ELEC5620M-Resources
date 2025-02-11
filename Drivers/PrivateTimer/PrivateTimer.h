/*
 * PrivateTimer.h
 *
 *  Created on: 19 Mar 2022
 *      Author: tom
 */

#ifndef PRIVATETIMER_PRIVATETIMER_H_
#define PRIVATETIMER_PRIVATETIMER_H_

// Allows for bool values
// Allows for logic statements
#include <stdbool.h>

// FUNCTION INIT
void ConfigTimer(void);
void ToggleTimer(bool en);
bool is1cs(void);
void PrintTimeGap(bool print);
void ClearInterruptFlag(void);
unsigned int RandomValue(void);


#endif /* PRIVATETIMER_PRIVATETIMER_H_ */
