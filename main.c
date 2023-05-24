/*
 * main.c
 *
 *  Created on: May 3, 2023
 *      Author: juicetin
 */
#include <msp430.h>

void led_init(void){
    P2DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);        // Annodes as outputs  (Active Low)
    P2OUT |= (BIT0+BIT1+BIT2+BIT3+BIT4);             // Annodes all off
    P2OUT |= BIT5;                                   // PWM pin on (for brightness) On
    P3DIR |= (BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);   // Cathodes all as outputs
    P3OUT &=~ (BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);  // Cathodes all low (Active high)
}

void clearLEDs(void) {
    P2OUT |= (BIT0+BIT1+BIT2+BIT3+BIT4);
    P3OUT &=~ (BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
}

void selectLED(unsigned int x, unsigned int y) {
    const int cathodePins[] = {BIT0, BIT1, BIT2, BIT3, BIT4};
    const int anodePins[] = {BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7};

    clearLEDs();

    P2OUT &=~ cathodePins[y];
    P3OUT |= anodePins[x];
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    led_init();

    selectLED(1, 1);
}
