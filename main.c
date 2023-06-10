/*
 * main.c
 *
 *  Created on: May 3, 2023
 *      Author: Justin Bak
 */
#include <msp430.h>
#include "main.h"

char display[] = {0x7f, 0x04, 0x18, 0x04, 0x7f};

void selectLED(char* display, int row, int column, int state) {
    if (state != 0) {
        display[column] |= (1<<row);
    } else {
        display[column] &=~ (1<<row);
    }
}

void initLed(void){
    P2DIR |= ANODES + BIT5;                             // Annodes as outputs  (Active Low)
    P2OUT |= ANODES + BIT5;
    P3DIR |= CATHODES;                                  // Cathodes all as outputs
    P3OUT &=~ CATHODES;                                 // Cathodes all low (Active high)
}

void initTimer(void) {
    TA1CCTL0 = CCIE;
    TA1CCR0 = 0;
    TA1CTL = TASSEL_2 + MC_2;

    _BIS_SR(GIE);
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    initLed();
    initTimer();


    selectLED(display, 0, 2, 1);

    selectLED(display, 1, 2, 1);

    __no_operation();
}

unsigned int cathode_index = 0;
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void) {
    P2OUT |= (ANODES);
    P2OUT &=~ (1<<cathode_index);

    P3OUT &=~ (CATHODES);
    P3OUT |= display[cathode_index]<<1;

    if(++cathode_index > 4) {
        cathode_index = 0;
    }

    TA1CCR0 += TIMER_INCREMENT;
}
