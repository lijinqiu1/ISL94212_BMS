//###################################################################################################################################################################################
//
// Headerdatei der Bibliothek "mcp2515.h"
//
// Ersteller : K. Evangelos 
// Datum     : Oktober 2015
// Umgebung  : IAR Embedded Workbench IDE 6.30
//
// Erläutert : * Inhalt: - Allgemein Defines (Timin, true, false, ...)
//                       - MCP2515 BEFEHLSSATZ
//                       - CAN-STRUCT-Variable
//                       - Prototypen
//
//             * Diese Library ermöglicht es CAN-Informationen über einen MCP2515 zu Empfangen und zu senden. In der aktuelle Konfiguration wird mit 250kbit/s genutzt. Als Schaltung ...
//               schlage ich, für 3,3V-Logik, vor:
//
//                                     ----------------
//                Über 10k an 3,3V -->| RST            |        ------------                 -------
//               An IR-Pin vom MCU <--| INT      TXCAN |------>| TXD   CANL |<------------->|
//                                    |          RXCAN |<------| RXD        |   |           |
//               An IO-Pin vom MCU -->| CS             |       |            |   -           |
//                        SPI MOSI -->| MOSI           | 10k-->| RS         |  | | 120 Ohm  | BUS
//                        SPI MISO <--| MISO           |       |            |  | |          |
//                       SPI Clock -->| SCK(Max 10MHz) |       |            |   -           |
//                                    |                |                    |   |           |
//                     Quarz 16MHz -->| OSC            |       |       CANH |<------------->|
//                                     ----------------         ------------                 -------
//                                      (MCP2515-E/SO)         (SN65HVD231D)
//              
//
//             * Um die Software zu nutzen, muss du folgende 5 Punkte ändern:
//
//               1. Die Chip-Select-Leitung ändern. Dazu folgende 2 Defines manipulieren
//                  #define MCP2515_CS_HIGH    P4OUT |= BIT7
//                  #define MCP2515_CS_LOW     P4OUT &=~ BIT7
//
//               2. Deine eigene SPI Transmit Funktion schreiben. Diese sollte folgendes Schema haben
//                  unsigned char MCP2515_SPI_transmit (unsigned char data)
//                  {
//                    // Sende Daten über TX-Puffer
//                    // Warte bis Daten übermittelt
//                    // Gebe RX-Puffer zurück
//                  }
//
//               3. Deine eigene SPI Funktion Initialisieren. Diese sollte folgendes Schema haben
//                  void MCP2515_SPI_init (void)
//                  {
//                    // SPI für Incative High, MSB first, 8-Bit, MCU ist Master einstellen
//                    // SPI Modi 0,0 oder 1,1
//                    // SPI Frequenz maximal 10MHz
//                    // SPI aktivieren
//                    // SPI Dummy senden (8Bit)
//                  }

//                4. Die Library für uint8_t, uint16_t, uint32_t einfügen. Dazu in der Header
//                   #include <stdint.h> 
//
//                5. Folgende Bereiche komplett löschen. Wirst dir deine eigene Schreiben
//                   CAN-OPEN-BEFEHLSSATZ        
// 
//####################################################################################################################################################################################

#ifndef MCP2515_H
#define MCP2515_H

#include <msp430.h> 
#include <stdint.h>

//########################## ALLGEMEIN DEFINES #######################################################################################################################################

#define F_CPU       1000000                                                      // Frequenz mit der die CPU arbeitet

#define DELAY_1s    F_CPU                                                        // Für __delay_cycles() ...
#define DELAY_100ms (F_CPU / 10)                                                 // ...
#define DELAY_10ms  (F_CPU / 100)                                                // ...
#define DELAY_1ms   (F_CPU / 1000)                                               // ...
#define DELAY_100us (F_CPU / 10000)                                              // ...
#define DELAY_10us  (F_CPU / 100000)                                             // ...
#define DELAY_1us   (F_CPU / 1000000)                                            // .-

#define BOOL char

#define CAN_RTR FALSE        // 1: Datenanforderung, 0: Nicht
#define CAN_DLC 8            // Anzahl der Byte des Datensatzes (Maximal 8 Byte)
#define CAN_EXTENDET FALSE   // Nur Standart IDs empfangen 

#define MCP2515_CS_LOW   SPI_CS_HIGH
#define MCP2515_CS_HIGH  SPI_CS_LOW

//############################# MCP2515 BEFEHLSSATZ ###################################################################################################################################

// -------------------------- Grund-Befehle ------------------------------------

#define MCP2515_WRITE           0x02
#define MCP2515_READ            0x03
#define MCP2515_READ_RX         0x90
#define MCP2515_WRITE_TX        0x40
#define MCP2515_DUMMY           0xFF                                             // oder 0xAA??
#define MCP2515_RESET           0xC0
#define MCP2515_BIT_MODIFY      0x05
#define MCP2515_READ_STATUS     0xA0
#define MCP2515_RX_STATUS       0xB0
#define MCP2515_RTS         0x80                                             // Siehe Seite 66

// ------------------------------- Register ------------------------------------

#define MCP2515_RXF0SIDH    0x00
#define MCP2515_RXF0SIDL    0x01
#define MCP2515_RXF0EID8    0x02
#define MCP2515_RXF0EID0    0x03
#define MCP2515_RXF1SIDH    0x04
#define MCP2515_RXF1SIDL    0x05
#define MCP2515_RXF1EID8    0x06
#define MCP2515_RXF1EID0    0x07
#define MCP2515_RXF2SIDH    0x08
#define MCP2515_RXF2SIDL    0x09
#define MCP2515_RXF2EID8    0x0A
#define MCP2515_RXF2EID0    0x0B
#define MCP2515_BFPCTRL     0x0C
#define MCP2515_TXRTSCTRL   0x0D
#define MCP2515_CANSTAT     0x0E
#define MCP2515_CANCTRL     0x0F

#define MCP2515_RXF3SIDH    0x10
#define MCP2515_RXF3SIDL    0x11
#define MCP2515_RXF3EID8    0x12
#define MCP2515_RXF3EID0    0x13
#define MCP2515_RXF4SIDH    0x14
#define MCP2515_RXF4SIDL    0x15
#define MCP2515_RXF4EID8    0x16
#define MCP2515_RXF4EID0    0x17
#define MCP2515_RXF5SIDH    0x18
#define MCP2515_RXF5SIDL    0x19
#define MCP2515_RXF5EID8    0x1A
#define MCP2515_RXF5EID0    0x1B
#define MCP2515_TEC     0x1C
#define MCP2515_REC     0x1D

#define MCP2515_RXM0SIDH    0x20                                             //
#define MCP2515_RXM0SIDL    0x21
#define MCP2515_RXM0EID8    0x22
#define MCP2515_RXM0EID0    0x23
#define MCP2515_RXM1SIDH    0x24
#define MCP2515_RXM1SIDL    0x25
#define MCP2515_RXM1EID8    0x26
#define MCP2515_RXM1EID0    0x27
#define MCP2515_CNF3        0x28
#define MCP2515_CNF2        0x29
#define MCP2515_CNF1        0x2A
#define MCP2515_CANINTE     0x2B                                             // Siehe Datenblatt, Seite 50
#define MCP2515_CANINTF     0x2C                                             // Siehe Datenblatt, Seite 51
#define MCP2515_EFLG        0x2D                                             // Siehe Datenblatt, Seite 47. Register für aktuelles Error-Mode (Falls ein Error-Frame kam)

#define MCP2515_TXB0CTRL    0x30                                             // Transmit Buffer Controll Register für Buffer 0
#define MCP2515_TXB0SIDH    0x31                                             // Standart Identifiert High Byte Buffer 0, um Standart-CAN-ID zu schreiben
#define MCP2515_TXB0SIDL    0x32                                             // Standart Identifiert Low Byte Buffer 0, um Standart-CAN-ID zu schreiben
#define MCP2515_TXB0EID8    0x33                                             // Extended Identifiert High Byte Buffer 0, um Extended-CAN-ID zu schreiben
#define MCP2515_TXB0EID0    0x34                                             // Extended Identifiert Low Byte Buffer 0, um Extended-CAN-ID zu schreiben
#define MCP2515_TXB0DLC     0x35                                             // DLC für TX0 schreiben (max 8)
#define MCP2515_TXB0D0      0x36                                             // Sender TX0 ; Der Datensatz (maximal 8 Byte)
#define MCP2515_TXB0D1      0x37
#define MCP2515_TXB0D2      0x38
#define MCP2515_TXB0D3      0x39
#define MCP2515_TXB0D4      0x3A
#define MCP2515_TXB0D5      0x3B
#define MCP2515_TXB0D6      0x3C
#define MCP2515_TXB0D7      0x3D

#define MCP2515_TXB1CTRL    0x40                                             // Transmit Buffer Controll Register für Buffer 1
#define MCP2515_TXB1SIDH    0x41                                             // Standart Identifiert High Byte Buffer 1, um Standart-CAN-ID zu schreiben
#define MCP2515_TXB1SIDL    0x42                                             // Standart Identifiert Low Byte Buffer 1, um Standart-CAN-ID zu schreiben
#define MCP2515_TXB1EID8    0x43                                             // Extended Identifiert High Byte Buffer 1, um Extended-CAN-ID zu schreiben
#define MCP2515_TXB1EID0    0x44                                             // Extended Identifiert Low Byte Buffer 1, um Extended-CAN-ID zu schreiben
#define MCP2515_TXB1DLC     0x45                                             // DLC für TX1 schreiben (max 8)
#define MCP2515_TXB1D0      0x46
#define MCP2515_TXB1D1      0x47
#define MCP2515_TXB1D2      0x48
#define MCP2515_TXB1D3      0x49
#define MCP2515_TXB1D4      0x4A
#define MCP2515_TXB1D5      0x4B
#define MCP2515_TXB1D6      0x4C
#define MCP2515_TXB1D7      0x4D

#define MCP2515_TXB2CTRL    0x50                                             // Transmit Buffer Controll Register für Buffer 2
#define MCP2515_TXB2SIDH    0x51                                             // Standart Identifiert High Byte Buffer 2, um Standart-CAN-ID zu schreiben
#define MCP2515_TXB2SIDL    0x52                                             // Standart Identifiert Low Byte Buffer 2, um Standart-CAN-ID zu schreiben
#define MCP2515_TXB2EID8    0x53                                             // Extended Identifiert High Byte Buffer 2, um Extended-CAN-ID zu schreiben
#define MCP2515_TXB2EID0    0x54                                             // Extended Identifiert Low Byte Buffer 2, um Extended-CAN-ID zu schreiben
#define MCP2515_TXB2DLC     0x55                                             // DLC für TX2 schreiben (max 8)
#define MCP2515_TXB2D0      0x56
#define MCP2515_TXB2D1      0x57
#define MCP2515_TXB2D2      0x58
#define MCP2515_TXB2D3      0x59
#define MCP2515_TXB2D4      0x5A
#define MCP2515_TXB2D5      0x5B
#define MCP2515_TXB2D6      0x5C
#define MCP2515_TXB2D7      0x5D

#define MCP2515_RXB0CTRL        0x60                                             // Siehe Datenblatt Seite 27
#define MCP2515_RXB0SIDH    0x61
#define MCP2515_RXB0SIDL    0x62
#define MCP2515_RXB0EID8    0x63
#define MCP2515_RXB0EID0    0x64
#define MCP2515_RXB0DLC     0x65
#define MCP2515_RXB0D0      0x66
#define MCP2515_RXB0D1      0x67
#define MCP2515_RXB0D2      0x68
#define MCP2515_RXB0D3      0x69
#define MCP2515_RXB0D4      0x6A
#define MCP2515_RXB0D5      0x6B
#define MCP2515_RXB0D6      0x6C
#define MCP2515_RXB0D7      0x6D

#define MCP2515_RXB1CTRL    0x70
#define MCP2515_RXB1SIDH    0x71
#define MCP2515_RXB1SIDL    0x72
#define MCP2515_RXB1EID8    0x73
#define MCP2515_RXB1EID0    0x74
#define MCP2515_RXB1DLC     0x75
#define MCP2515_RXB1D0      0x76
#define MCP2515_RXB1D1      0x77
#define MCP2515_RXB1D2      0x78
#define MCP2515_RXB1D3      0x79
#define MCP2515_RXB1D4      0x7A
#define MCP2515_RXB1D5      0x7B
#define MCP2515_RXB1D6      0x7C
#define MCP2515_RXB1D7      0x7D

// --------------------- Bit Defintions ----------------------------------------

//  Register BFPCTRL
#define MCP2515_B1BFS   0x5
#define MCP2515_B0BFS   0x4
#define MCP2515_B1BFE   0x3
#define MCP2515_B0BFE   0x2
#define MCP2515_B1BFM   0x1
#define MCP2515_B0BFM   0x0

//  Register TXRTSCTRL, Datenblatt Seite 19
#define MCP2515_B2RTS   0x20                                                     // TX2RTS Pin State, 1: Digital Input Mode                 - 0: Request to Send Mode
#define MCP2515_B1RTS   0x10                                                     // TX1RTS Pin State, 1: Digital Input Mode                 - 0: Request to Send Mode
#define MCP2515_B0RTS   0x08                                                     // TX0RTS Pin State, 1: Digital Input Mode                 - 0: Request to Send Mode
#define MCP2515_B2RTSM  0x04                                                     // TX2RTS Pin Mode ,  1: Request Message Transmission TXB2 - 0: Is digital Input
#define MCP2515_B1RTSM  0x02                                                     // TX1RTS Pin Mode ,  1: Request Message Transmission TXB1 - 0: Is digital Input
#define MCP2515_B0RTSM  0x01                                                     // TX0RTS Pin Mode ,  1: Request Message Transmission TXB0 - 0: Is digital Input

//  Register CANSTAT
#define MCP2515_OPMOD2  0x7
#define MCP2515_OPMOD1  0x6
#define MCP2515_OPMOD0  0x5
#define MCP2515_ICOD2   0x3
#define MCP2515_ICOD1   0x2
#define MCP2515_ICOD0   0x1

//  Register CANCTRL
#define MCP2515_REQOP2  0x7
#define MCP2515_REQOP1  0x6
#define MCP2515_REQOP0  0x5
#define MCP2515_ABAT    0x4
#define MCP2515_CLKEN   0x2
#define MCP2515_CLKPRE1 0x1
#define MCP2515_CLKPRE0 0x0

//  Register CNF3
#define MCP2515_WAKFIL  0x6
#define MCP2515_PHSEG22 0x2
#define MCP2515_PHSEG21 0x1
#define MCP2515_PHSEG20 0x0

//  Register CNF2
#define MCP2515_BTLMODE 0x7
#define MCP2515_SAM 0x6
#define MCP2515_PHSEG12 0x5
#define MCP2515_PHSEG11 0x4
#define MCP2515_PHSEG10 0x3
#define MCP2515_PRSEG2  0x2
#define MCP2515_PRSEG1  0x1
#define MCP2515_PRSEG0  0x0

//  Register CNF1
#define MCP2515_SJW1    0x7
#define MCP2515_SJW0    0x6
#define MCP2515_BRP5    0x5
#define MCP2515_BRP4    0x4
#define MCP2515_BRP3    0x3
#define MCP2515_BRP2    0x2
#define MCP2515_BRP1    0x1
#define MCP2515_BRP0    0x0

//  Register CANINTE
#define MCP2515_MERRE   0x7
#define MCP2515_WAKIE   0x6
#define MCP2515_ERRIE   0x5
#define MCP2515_TX2IE   0x4
#define MCP2515_TX1IE   0x3
#define MCP2515_TX0IE   0x2
#define MCP2515_RX1IE   0x1
#define MCP2515_RX0IE   0x0

//  Register CANINTF
#define MCP2515_MERRF   0x7
#define MCP2515_WAKIF   0x6
#define MCP2515_ERRIF   0x5
#define MCP2515_TX2IF   0x4
#define MCP2515_TX1IF   0x3
#define MCP2515_TX0IF   0x2
#define MCP2515_RX1IF   0x1
#define MCP2515_RX0IF   0x0

//  Register EFLG
#define MCP2515_RX1OVR  0x7
#define MCP2515_RX0OVR  0x6
#define MCP2515_TXBO    0x5
#define MCP2515_TXEP    0x4
#define MCP2515_RXEP    0x3
#define MCP2515_TXWAR   0x2
#define MCP2515_RXWAR   0x1
#define MCP2515_EWARN   0x0

//  Register TXB0CTRL, TXB1CTRL, TXB2CTRL, Datenblatt Seite 18
#define MCP2515_ABTF    0x40                                                     // Message Abort Flag, 1: Abbort          - 0: Success
#define MCP2515_MLOA    0x20                                                     // Lost Arbiation Bit, 1: Lost            - 0: Not Lost
#define MCP2515_TXERR   0x10                                                     // Transmission Error, 1: Error           - 0: No Error
#define MCP2515_TXREQ   0x08                                                     // Message Request   , 1: Is trnsmiting   - 0: Is not transmitting
#define MCP2515_TXP1    0x02                                                     // Message Priority  , 0: Lowest,           1: Low Intermediate, ...
#define MCP2515_TXP0    0x01                                                     // Message Priority  , 2: High Intermediate,3: Highest

//  Register RXB0CTRL
#define MCP2515_RXM1    0x6
#define MCP2515_RXM0    0x5
#define MCP2515_RXRTR   0x3
#define MCP2515_BUKT    0x2
#define MCP2515_BUKT1   0x1
#define MCP2515_FILHIT0 0x0

//  Register RXB1CTRL
#define MCP2515_FILHIT2 0x2
#define MCP2515_FILHIT1 0x1
#define MCP2515_FILHIT0 0x0

#define MCP2515_SIDH    0x00                                                     // Standart-Identifier High-Byte, um CAN-ID zu schreiben genutzt
#define MCP2515_SIDL    0x01                                                     // Standart-Identifier Low-Byte, um CAN-ID zu schreiben genutzt
#define MCP2515_EID8    0x02                                                     // Extended-Identifier High-Byte, um CAN-ID zu schreiben genutzt
#define MCP2515_EID0    0x03                                                     // Extended-Identifier Low-Byte, um CAN-ID zu schreiben genutzt

#define MCP2515_TXBnSIDL_EXIDE      0x3                                          // Extended Identifier Enable

#define MCP2515_CAN_EXT_ID    1
#define MCP2515_CAN_NO_EXT_ID 0

#define MCP2515_RXB1CTRL_FILHIT_RXF5  0x05                                       // Akzeptanz Filter 5 (RXF5)
#define MCP2515_RXB1CTRL_FILHIT_RXF4  0x04                                       // Akzeptanz Filter 4 (RXF4)
#define MCP2515_RXB1CTRL_FILHIT_RXF3  0x03                                       // Akzeptanz Filter 3 (RXF3)
#define MCP2515_RXB1CTRL_FILHIT_RXF2  0x02                                       // Akzeptanz Filter 2 (RXF2)
#define MCP2515_RXB1CTRL_FILHIT_RXF1  0x01                                       // Akzeptanz Filter 1 (RXF1) (nur wenn BUKT bit gesetzt in RXB0CTRL)
#define MCP2515_RXB1CTRL_FILHIT_RXF0  0x00                                       // Akzeptanz Filter 0 (RXF0) (nur wenn BUKT bit gesetzt in RXB0CTRL)

#define MCP2515_CANINTF_ALL_DISABLE 0x00

#define MCP2515_EFLAG 0x2d

//################################## CAN-VARIABLEN ####################################################################################################################################

#define CAN_RTR FALSE                                                            // 1: Datenanforderung, 0: Nicht
//#define CAN_DLC 8                                                              // Anzahl der Byte des Datensatzes (Maximal 8 Byte)
#define CAN_EXTENDET FALSE 

//########################### CAN-STRUCT-Variable ######################################################################################################################################

typedef struct 
{
  uint32_t COB_ID;                                                               // Adressse/Befehl CAN-Open
  uint8_t status;                                                                // Aktueller Status der dem Nutzer zugesendet wird 
  uint8_t dlc;                                                                   // CAN-DLC
  uint8_t rtr;                                                                   // CAN-Request
  uint8_t ext;                                                                   // Extended oder Standart frame
  uint8_t data[CAN_DLC];                                                         // Datensatz von CAN
} can_t; // CAN-Typ 


//################################### PROTOTYPEN ######################################################################################################################################

void MCP2515_SPI_init(void);                                                     // Initialisiert SPI-UCB0
unsigned char MCP2515_SPI_transmit(unsigned char daten);                         // Senden und Empfangen über SPI-UCB0

BOOL MCP2515_spi_test (void);                                                    // Teste ob MCP2515 funktioniert
void MCP2515_reset (void);                                                       // Resetet den MCP2515 (Hardware oder Software-Reset soll gleiche auswirkung haben)
void MCP2515_CanVariable_init (can_t *can);                                      // initialisiere CAN-Variable
void MCP2515_init(void);                                                         // Initialisiert den MCP2515
void MCP2515_bit_modify(uint8_t addr, uint8_t mask, uint8_t data);               // Einzelne Bits setzen  und löschen

void MCP2515_write(uint8_t addr, uint8_t data);                                  // Sende dem MCP2515
void MCP2515_write_many_registers(uint8_t addr, uint8_t len, uint8_t *data);     // Um mehrere Register mit einem Befehl zu beschreiben
void MCP2515_write_id(uint8_t addr, BOOL ext, unsigned long id);                 // Sendet die ID des Frames

uint8_t MCP2515_read(uint8_t addr);                                              // Lese aus dem MCP2515
void MCP2515_read_many_registers(uint8_t addr, uint8_t length, uint8_t *data);   // Lese mehrere Register aus   
void MCP2515_read_id(uint8_t addr, unsigned long* id);                           // Lese die ID der CAN-Nachricht

void MCP2515_can_tx0(can_t *can);                                                // Sende CAN-Frame über Buffer TX0
void MCP2515_can_tx1(can_t *can);                                                // Sende CAN-Frame über Buffer TX1
void MCP2515_can_tx2(can_t *can);                                                // Sende CAN-Frame über Buffer TX2
void MCP2515_can_rx0(can_t *can);                                                // Sende CAN-Frame über Buffer RX0
void MCP2515_can_rx1(can_t *can);                                                // Sende CAN-Frame über Buffer RX1

void MCP2515_clear_rx0(void);                                                    // Lösche nur den RX0-IR des MCP2515
void MCP2515_clear_rx1(void);                                                    // Lösche nur den RX1-IR des MCP2515
void MCP2515_int_clear(void);                                                    // Lösche alle IRs die der MCP2515 hat

#endif
