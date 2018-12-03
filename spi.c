/*
 * spi.c
 *
 *  Created on: 2018Äê10ÔÂ15ÈÕ
 *      Author: Administrator
 */
#include <msp430.h>
#include "spi.h"

static SPI_STATUS_t spi_status = SPI_READY;

SPI_STATUS_t spi_get_status(void)
{
    return spi_status;
}

void spi_set_status(SPI_STATUS_t status)
{
    spi_status = status;
}

void spi_init(void)
{
      P1SEL = BIT5 + BIT6 + BIT7;                                                    // Spezial Funktion SCK, MOSI und MISO ...
      P1SEL2 = BIT5 + BIT6 + BIT7;                                             // (Falls P1SEL2 unterstützt).-

      P2DIR |= BIT0;                                                                 // !CS-Leitung (beachte auch Defines MCP2515_CS_LOW und MCP2515_CS_High)  ...
      P2OUT |= BIT0;                                                                 // .-

      UCB0CTL1 |= UCSWRST;                                                           // Reset
      UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCMODE_0 + UCSYNC;                        // 3-pin, 8-bit SPI master
      UCB0CTL1 |= UCSSEL_2;                                                          // SMCLK
      UCB0BR0 |= 0x02;                                                               // /2 ...
      UCB0BR1 = 0;                                                                   // .-
      //UCB0MCTL = 0;                                                                // No modulation
      UCB0CTL1 &= ~UCSWRST;                                                          // Initialize USCI state machine

      __delay_cycles(DELAY_100ms);                                                   // Warte 100ms
      while (!(IFG2 & UCB0TXIFG));                                                   // Warte bist übermittelt
//      UCB0TXBUF = 0x00;                                                              // Dummy Senden
}

unsigned char spi_transmit(unsigned char daten)
{
    UCB0TXBUF = daten;                                                             // Sende Datensatz
    while(UCB0STAT & UCBUSY);                                                      // Warte bist übermittelt
    return UCB0RXBUF;                                                              // Gebe empfangenen Datensatz zurück
}


