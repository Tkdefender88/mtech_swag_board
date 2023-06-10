/*
 * main.h
 *
 *  Created on: Jun 10, 2023
 *      Author: juicetin
 */

#ifndef MAIN_H_
#define MAIN_H_

#define ANODES (BIT0 + BIT1 + BIT2 + BIT3 + BIT4)
#define CATHODES (BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7)
#define TIMER_INCREMENT 1000

void selectLED(char* display, int row, int column, int state);

#endif /* MAIN_H_ */
