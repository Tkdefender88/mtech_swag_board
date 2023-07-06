#include <msp430.h> 
#include "serial_handler.h"


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
int isrCount = 0,scrollCount = 0;

int main(void){
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    led_init();
    switch_init();
    timer_ISR_init();
    uart_init(4);
    int a_val, k_val, scrollPos=0, scrollNum, scrollMax;
    char disp_string[] = "MONTANA TECH EE WILL CHALLENGE YOU!    MTECH.EDU|";
    char two_char[11] = {0,0,0,0,0,0,0,0,0,0,0};
    char scrollLast=0;
    a_val = 0;
    while (scrollNum != '|')
        scrollNum = disp_string[a_val++];
    scrollNum=a_val;
    scrollMax = scrollNum*6;
    disp_string[scrollNum]='[';
    for (k_val=0;k_val<scrollNum;k_val++){
        if(disp_string[k_val]==' ')
            disp_string[k_val]='[';
    }

    while(1){
        if (scrollCount>=2000){
            scrollCount=0;
            a_val = scrollPos/6;
            charMap(disp_string[a_val], two_char);
            a_val = scrollPos/6;
            charMap(disp_string[a_val+1], two_char+6);
            a_val = scrollPos%6;
//            tx_data_str[0] = gridArray[0];
//            uart_write_string(0,1);
            UCA0TXBUF = gridArray[0];
            for (k_val=0;k_val<5;k_val++){
                gridArray[k_val] = two_char[k_val+a_val];
            }
            scrollPos++;
            if (scrollPos > scrollMax)
                scrollPos = 0;
        }
    }
}


void charMap(int valin, char * mapOut){
    char mapArray[44][5] = {{0x3E, 0x45, 0x49,0x51, 0x3E},  //0
                            {0x00, 0x21, 0x7F, 0x01, 0x00}, // 1
                            {0x21, 0x43,0x45, 0x49, 0x31},  // 2
                            {0x42, 0x41, 0x51, 0x69, 0x46},
                            {0x0C, 0x14, 0x24, 0x7F, 0x04},
                            {0x72, 0x51,0x51,0x51,0x4E},
                            {0x1E, 0x29, 0x49, 0x49, 0x06},
                            {0x40, 0x47, 0x48, 0x50, 0x60},
                            {0x36, 0x49, 0x49,0x49, 0x36},
                            {0x30, 0x49, 0x49, 0x4A, 0x3C}, //9
                            {0x00, 0x36, 0x36, 0x00, 0x00}, //:
                            {0x00, 0x35, 0x36, 0x00, 0x00}, //;
                            {0x08, 0x14, 0x22, 0x41, 0x00},//<
                            {0x14,0x14,0x14,0x14,0x14},// =
                            {0x00, 0x41, 0x22, 0x14, 0x08}, //>
                            {0x20, 0x40, 0x45, 0x48, 0x30}, //?
                            {0x26, 0x49, 0x4F, 0x41, 0x3E}, //@
                            {0x3F, 0x44, 0x44, 0x44, 0x3F}, //A
                            {0x7F, 0x49, 0x49, 0x49, 0x36},
                            {0x3E, 0x41, 0x41, 0x41, 0x22},
                            {0x7F, 0x41, 0x41, 0x22, 0x1C},
                            {0x7F, 0x49, 0x49, 0x49, 0x41},
                            {0x7F, 0x48, 0x48, 0x48, 0x40},
                            {0x3E, 0x41, 0x49, 0x49, 0x2F},
                            {0x7F, 0x08, 0x08, 0x08, 0x7F},
                            {0x00, 0x41, 0x7F, 0x41, 0x00},
                            {0x02, 0x01, 0x41, 0x7E, 0x40},
                            {0x7F, 0x08, 0x14, 0x22, 0x41},
                            {0x7F, 0x01, 0x01, 0x01, 0x01},
                            {0x7F, 0x20, 0x10, 0x20, 0x7F},
                            {0x7F, 0x10, 0x08, 0x04, 0x7F}, //N
                            {0x3E, 0x41, 0x41, 0x41, 0x3E}, //O
                            {0x7F, 0x48, 0x48,  0x48, 0x30},//P
                            {0x3E, 0x41, 0x45, 0x42, 0x3D},
                            {0x7F, 0x48, 0x4C, 0x4A, 0x31},
                            {0x31, 0x49, 0x49, 0x49, 0x46},
                            {0x40, 0x40, 0x7F, 0x40, 0x40},
                            {0x7E, 0x01, 0x01, 0x01, 0x7E},
                            {0x7C, 0x02, 0x01, 0x02, 0x7C},
                            {0x3E, 0x01, 0x0E, 0x01, 0x3E},
                            {0x63, 0x14, 0x08, 0x14, 0x63},
                            {0x70, 0x08, 0x07, 0x08, 0x70},
                            {0x43, 0x45, 0x49, 0x51, 0x61},
                            {0,0,0,0,0}};
    int k,n;
    char tempval,holder;
    for (k=0;k<5;k++){
        holder=mapArray[valin-0x30][k];
        tempval=0;
        for (n=0;n<8;n++){
            tempval<<=1;
            if (holder&(1<<n)){

                tempval+=1;

            }



        }
        tempval>>=1;
        mapOut[k]=tempval;
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
    scrollCount++;
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

