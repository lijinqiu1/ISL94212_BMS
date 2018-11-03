#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    P3DIR &= ~BIT3;
    P3DIR |= BIT3;
    P3DIR |= BIT3;

    BCSCTL1 = CALBC1_1MHZ;                    // Set range
    DCOCTL  = CALDCO_1MHZ;

    // initialize Timer_A module
    TACCR0 = 12500;
    TACTL = TASSEL_2 + MC_1 + TACLR + ID_3;   // ACLK, up mode, clear TAR
    TACCTL0 |= CCIE;                          // CCR0 interrupt enabled
    _BIS_SR(GIE);              //¿ª×ÜÖÐ¶Ï

    while(1)
    {
        __bis_SR_register(LPM1_bits);       // Enter LPM3, enable interrupts

        P3OUT ^= BIT3;
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
    }
}
