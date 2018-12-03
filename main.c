#include <msp430.h> 
#include <isl94212.h>
#include "spi.h"

unsigned char flag = 1;
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    P3DIR &= ~(BIT3|BIT7);
    P3DIR |= BIT3|BIT7;
    P3OUT |= BIT7;

    BCSCTL1 = CALBC1_1MHZ;                    // Set range
    DCOCTL  = CALDCO_1MHZ;

    // initialize Timer_A module
    TACCR0 = 12500;
    TACTL = TASSEL_2 + MC_1 + TACLR + ID_3;   // ACLK, up mode, clear TAR
    TACCTL0 |= CCIE;                          // CCR0 interrupt enabled
    _BIS_SR(GIE);              //�����ж�
    spi_init();
    ISL94212_Init();
    while(1)
    {
        if (flag)
        {
            __bis_SR_register(LPM1_bits);       // Enter LPM3, enable interrupts
//            ISL94212_updateReadings();
            flag = 0;
        }
    }
	return 0;
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    static unsigned char count=0;
    if (count ++ > 10)
    {
        __bic_SR_register_on_exit(LPM1_bits);
        count = 0;
        P3OUT ^= BIT3;
        flag = 1;
    }
}
