/*
 * main.h
 *
 *  Created on: Jun 10, 2023
 *      Author: Justin
 */

#ifndef MAIN_H_
#define MAIN_H_

#define EVER (;;)

#define ANODES (BIT0 + BIT1 + BIT2 + BIT3 + BIT4)
#define CATHODES (BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7)
#define TIMER_INCREMENT 1000

void selectLED(char* display, int row, unsigned int column, int state);

typedef enum _MicroControllerState {
    GAME_MODE,
    DISPLAY_MODE,
    SLAVE_MODE,
    MENU_MODE,
} MicroControllerState;

#endif /* MAIN_H_ */
