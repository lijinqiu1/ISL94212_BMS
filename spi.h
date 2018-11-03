/*
 * spi.h
 *
 *  Created on: 2018Äê10ÔÂ15ÈÕ
 *      Author: Administrator
 */

#ifndef SPI_H_
#define SPI_H_

typedef enum
{
    SPI_BUSY,
    SPI_READY,
    SPI_SEND_SUCCESS,
}SPI_STATUS_t;

#define F_CPU       1000000                                                      // Frequenz mit der die CPU arbeitet

#define DELAY_1s    F_CPU                                                        // Für __delay_cycles() ...
#define DELAY_100ms (F_CPU / 10)                                                 // ...
#define DELAY_10ms  (F_CPU / 100)                                                // ...
#define DELAY_1ms   (F_CPU / 1000)                                               // ...
#define DELAY_100us (F_CPU / 10000)                                              // ...
#define DELAY_10us  (F_CPU / 100000)                                             // ...
#define DELAY_1us   (F_CPU / 1000000)                                            // .-

#define BOOL char
#define TRUE 0
#define FALSE 1

#define SPI_DUMMY           0xFF                                             // oder 0xAA??


#define SPI_CS_LOW   P2OUT &=~ BIT0
#define SPI_CS_HIGH  P2OUT |=  BIT0

unsigned char spi_transmit(unsigned char daten);
SPI_STATUS_t spi_get_status(void);
void spi_set_status(SPI_STATUS_t status);
#endif /* SPI_H_ */
