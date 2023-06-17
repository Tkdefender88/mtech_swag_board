/*
 * main.c
 *
 *  Created on: May 3, 2023
 *      Author: Justin Bak
 */
#include <msp430.h>
#include "main.h"
#include "Queue.h"

//char display[] = {0x7f, 0x04, 0x18, 0x04, 0x7f};
Queue display;
int button_input = 0;
int scroll_count = 0;

MicroControllerState mcu_state = MENU_MODE;

void selectLED(char* display, int row, unsigned int column, int state) {
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

void initButtons(void) {
    P1DIR &=~ BIT0; // set 1.0 as input
    P1IES |= BIT0;
    P1IE |= BIT0;
    P1IFG &=~ BIT0;

    _BIS_SR(GIE);
}

void menuMode() {
    char* message = "HELLO WORLD!";
    static int scroll_index = 0;
    static int character_index = 0;
    static int character_column_index = 0;

    if (scroll_count > SCROLL_DELAY) {
        scroll_count = 0;

        char current_character = message[character_index];
        char column_value = MiniAscii[current_character - 0x30][character_column_index];

        enqueue(&display, column_value);

        character_column_index++;
        if (character_column_index > 4) {
            character_column_index = 0;
            character_index++;
            if (character_index > 11) {
                character_index = 0;
            }
        }
    }
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    initQueue(&display)

    initLed();
    initTimer();

    for EVER {
        switch(mcu_state) {
        case GAME_MODE:
            break;
        case DISPLAY_MODE:
            break;
        case SLAVE_MODE:
            break;
        case MENU_MODE:
            menuMode();
            break;
        }
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void) {

}

#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void) {

}

unsigned int cathode_index = 0;
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void) {
    P2OUT |= (ANODES);
    P2OUT &=~ (1<<cathode_index);

    unsigned int data_index = (cathode_index + display.front) % 5;

    P3OUT &=~ (CATHODES);
    P3OUT |= display.data[data_index]<<1;

    if(++cathode_index > 4) {
        cathode_index = 0;
    }
    scroll_count++;

    TA1CCR0 += TIMER_INCREMENT;
}
