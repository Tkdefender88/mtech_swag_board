#include <msp430.h> 


/**
 * main.c
 * 1.0 - SW2
 * 1.1 - RX0
 * 1.2 - TX0
 * 1.3 - SW1
 * 1.4 - RX1
 * 1.5 - TX1
 * 1.6 - SCL
 * 1.7 - SDA
 * 2.0 - SIG_C1
 * 2.1 - SIG_C2
 * 2.2 - SIG_C3
 * 2.3 - SIG_C4
 * 2.4 - SIG_C5
 * 2.5 - PWM_K
 * 2.6 - SW3
 * 2.7 - NC
 * 3.0 - NC
 * 3.1 - SIG_R1
 * 3.2 - SIG_R2
 * 3.3 - SIG_R3
 * 3.4 - SIG_R4
 * 3.5 - SIG_R5
 * 3.6 - SIG_R6
 * 3.7 - SIG_R7
 *
 */

char gridArray[5] = {0x7F, 0x04, 0x18, 0x04, 0x7F};
int isrCount = 0;

int main(void){
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    led_init();
    switch_init();
    timer_ISR_init();
    int a_val, k_val;
    while(1){
//        for (a_val=0;a_val<5; a_val++){
//            for (k_val=0;k_val<7; k_val++){
//                test_sequence(a_val, k_val);
//                __delay_cycles(100000);
//            }
//        }
        //        P2OUT ^= (BIT0+BIT1+BIT2+BIT3+BIT4);
        //        __delay_cycles(1000000);
        //        P3OUT ^= (BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
        //        __delay_cycles(1000000);


    }
}

void test_sequence(int a_in, int k_in){  // Simply lights each LED by itself
    P2OUT |= (BIT0+BIT1+BIT2+BIT3+BIT4);
    P3OUT &=~ (BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
    P2OUT &=~ (1<<a_in);
    P3OUT |= (1<< (k_in+1));
}

void led_init(void){
    P2DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);   // Annodes as outputs  (Active Low)
    P2OUT |= (BIT0+BIT1+BIT2+BIT3+BIT4);        // Annodes all off
    P2OUT |= BIT5;                             // PWM pin on (for brightness) On
    P3DIR |= (BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);  // Cathodes all as outputs
    P3OUT &=~ (BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);  // Cathodes all low (Active high)

}

void timer_ISR_init(void){
    TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TA1CCR0 = 0;
    TA1CTL = TASSEL_2 + MC_2;                  // SMCLK, contmode

    _BIS_SR( GIE);                 // Enter LPM0 w/ interrupt
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IFG & BIT0){
        P1IFG &= ~BIT0;                           // P1.0 IFG cleared
    }
    if (P1IFG & BIT3){
        P1IFG &= ~BIT3;                           // P1.4 IFG cleared
    }
}


// Port 1 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if (P2IFG & BIT6){
        P2IFG &= ~BIT6;                           // P1.0 IFG cleared
    }

}

// Timer A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A1 (void)
{
    P2OUT |= (BIT0+BIT1+BIT2+BIT3+BIT4);
    P3OUT &=~ (BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
    P2OUT &=~ (1<<isrCount);
    P3OUT |= gridArray[isrCount]<<1;

    isrCount++;
    if (isrCount>4)
        isrCount=0;
    TA1CCR0 +=1000;                            // Add Offset to CCR0
}

void switch_init(void){
    P1DIR &=~ (BIT0+BIT3);  // INputs for switches
    P2SEL &=~ (BIT6+BIT7);              // Turn off oscillator
    P2SEL2 &=~ (BIT6+BIT7);              // Turn off oscillator
    P2DIR &=~ (BIT6);       // Input for switch
    P1OUT |= (BIT0+BIT3);   // Pull-UP for switches
    P2OUT |= (BIT6);    // Pull-UP for switches
    P1REN |= (BIT0+BIT3);   // Pulling resistors for switches
    P2REN |= (BIT6);    // Pulling resistor for switch
    P1IE |=(BIT0+BIT3);   // Hardware interrupt P1 switches
    P1IES |=(BIT0+BIT3);   // High to low edge trigger
    P2IE |=(BIT6);   // Hardware interrupt P1 switches
    P2IES |=(BIT6);   // High to low edge trigger
    P1IFG &= ~ (BIT0+BIT3);
    P2IFG &= ~BIT6;
    _BIS_SR( GIE);                 // Global interrupt enable
}

