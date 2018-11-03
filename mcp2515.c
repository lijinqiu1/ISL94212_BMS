#include "mcp2515.h"
#include "spi.h"

//####################################################################################################################################################################################
//                                                                     MCP2515_SPI_transmit()
//
// Senden und Empfangen über SPI-UCB0.
//
//                               Variablen
// @ daten    : Unsigned 8-Bit-Datensatz der über UCB0 via SPI gesendet werden soll
//
// @ rückgabe : Unsigned 8-Bit-Datensatz der über UCB0 via SPI empfangen wird
//
unsigned char MCP2515_SPI_transmit(unsigned char daten)
{   
  return spi_transmit(daten);                                                              // Gebe empfangenen Datensatz zurück
} 

//####################################################################################################################################################################################
//                                                                      MCP2515_spi_test()
//
// Teste ob MCP2515 funktioniert. 
// Dazu: 1. Schreibe etwas in den Registern und lesen die Register daraufhin aus
//       2. Vergleiche das empfangene mit dem beschiebenen Array. Falls ungleich Kommunikations-Fehler. Sonst alles OK             
//
//                               Variablen
// @ rückgabe : Unsigned 8-Bit-Datensatz (TRUE oder FALSE); TRUE = 1 = MCP2515 OK, FALSE = 0 = MCP2515 Fehler
//
BOOL MCP2515_spi_test (void)
{
  uint8_t i;
  uint16_t data_rcv[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                     // Array der empfangenen Daten. Mit 0 Initialisieren
  uint16_t data_snd[11]={0x88,0x03,0x90,0x02,0x05,0x02,0x3f,0x23,0x40,0x40,0x00};// Array der zu sendenen Daten.
  
  MCP2515_write(MCP2515_CANCTRL, data_snd[0]);                                   // 1. Gehe im Konfig-Mode. Zum Überprüfen, ...
  data_rcv[0]=MCP2515_read(MCP2515_CANCTRL);                                     // schreibe zuerst in den Registern etwas und anschließend lese es aus. ...
  
  MCP2515_write(MCP2515_CNF1,data_snd[1]);                                       // ...
  MCP2515_write(MCP2515_CNF2,data_snd[2]);                                       // ...
  MCP2515_write(MCP2515_CNF3,data_snd[3]);                                       // ...
  data_rcv[1]=MCP2515_read(MCP2515_CNF1);                                        // ...
  data_rcv[2]=MCP2515_read(MCP2515_CNF2);                                        // ...
  data_rcv[3]=MCP2515_read(MCP2515_CNF3);                                        // ...
  MCP2515_write(MCP2515_RXM0SIDH, data_snd[4]);                                  // ...
  MCP2515_write(MCP2515_RXM0SIDL, data_snd[5]);                                  // ...                                  
  data_rcv[4]=MCP2515_read(MCP2515_RXM0SIDH);                                    // ...                                    
  data_rcv[5]=MCP2515_read(MCP2515_RXM0SIDL);                                    // ...  
  MCP2515_write(MCP2515_CANINTE, data_snd[6]);                                   // ...
  data_rcv[6]=MCP2515_read(MCP2515_CANINTE);                                     // ...
  MCP2515_write(MCP2515_CANINTF, data_snd[7]);                                   // ...
  data_rcv[7]=MCP2515_read(MCP2515_CANINTF);                                     // ...
  MCP2515_write(MCP2515_TXB0SIDL, data_snd[8]);                                  // ...
  data_rcv[8]=MCP2515_read(MCP2515_TXB0SIDL);                                    // ...
  MCP2515_write(MCP2515_TXB1SIDL, data_snd[9]);                                  // ...
  data_rcv[9]=MCP2515_read(MCP2515_TXB1SIDL);                                    // ...
  
  MCP2515_write(MCP2515_CANCTRL, data_snd[10]);                                  // ... (gehe auch zum Normalmodus)
  data_rcv[10]=MCP2515_read(MCP2515_CANCTRL);                                    // .-
  
  for(i = 0; i < 11; i++)                                                   // 2. Vergleiche das empfangene mit dem beschiebenen Array. ...
  {                                                                              // ...
    if(data_snd[i] != data_rcv[i]) return FALSE;                                 // Falls Ungleich. Kommunikation Fehlerhaft (FALSE = 0). ...
  } // for                                                                       // Sonst ...
  
  MCP2515_init();                                                                // Muss neu Initialisieren da ich dem MCP2515 manipuliert habe. ...

  return TRUE;                                                                   // Sende das Kommunikation mit MCP2515 stabil ist (TRUE = 1).-
}

//####################################################################################################################################################################################
//                                                                      MCP2515_reset()
//
// Resetet den MCP2515, dabei soll Hardware oder Software-Reset die gleiche auswirkung haben laut Datenblatt Seite 63
//
void MCP2515_reset (void)
{
  if (spi_get_status() == SPI_BUSY)
  {
    return ;
  }
  spi_set_status(SPI_BUSY);

  MCP2515_CS_LOW;                                                                // Starte Kommunikation indem ich !CS auf Low Ziehe. Sende ...  
  MCP2515_SPI_transmit(MCP2515_RESET);                                           // Resetbefehl 0xC0. Hardware- oder Software-Reset haben laut Datenblatt die gleiche auswirkung. ... 
  MCP2515_CS_HIGH;                                                               // Beende den Frame in dem ich !CS wieder auf High setze und ...

  __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
  spi_set_status(SPI_READY);
}

//####################################################################################################################################################################################
//                                                                 MCP2515_CanVariable_init()
//
// Im Projekt möchte ich eine Variable haben die alle CAN-Eigenschaften einer Information zusammenbündelt. Bevor ich diese nutze solle zuerst die Variable initialisiert werden.
//
//                               Variablen
// @ can      : Struckt für CAN-Variable. Siehe "MCP2515.h"
//
void MCP2515_CanVariable_init (can_t *can)
{
  uint8_t i;
  can->COB_ID = 0x181;                                                           // ID der Information
  can->status = 0x01;                                                            // Zufalswert als Beispiel                                             
  can->dlc = CAN_DLC;                                                            // Länge der Information (0 bis 8)
  can->rtr = CAN_RTR;                                                            // Request Data oder sende
  can->ext = CAN_EXTENDET;                                                       // Extender oder standart ID, hier standart genutzt
  for(i = 0; i < CAN_DLC; i++)can->data[i] = i;                             // Initialisiere mit 0
}



//####################################################################################################################################################################################
//                                                                      MCP2515_write()
//
// In einem einzelnen Register des MCP2515 über SPI Schreiben
//
//                               Variablen
// @ addr     : Adresse des MCP2515
// @ data     : Unsigned 8-Bit-Datensatz
//
void MCP2515_write (uint8_t addr, uint8_t data)
{
  if (spi_get_status() == SPI_BUSY)
  {
    return ;
  }
  spi_set_status(SPI_BUSY);

  MCP2515_CS_LOW;                                                                // Starte Frame in dem ich !CS auf Low Ziehe. Sende ...
  
  MCP2515_SPI_transmit(MCP2515_WRITE);                                           // Schreibbefehl ; Befehl 0x02, ...
  MCP2515_SPI_transmit(addr);                                                    // sende Befehl (Register-Adresse), ...
  MCP2515_SPI_transmit(data);                                                    // sende Daten für diese Adresse, ...
  
  MCP2515_CS_HIGH;                                                               // Beende den Frame in dem ich !CS wieder auf High setze und ...

  __delay_cycles(DELAY_1ms);                                                     // warte ein bischen.-
  spi_set_status(SPI_READY);
}

//####################################################################################################################################################################################
//                                                                 MCP2515_write_many_registers()
//
// Um mehrere Register des MCP2515 mit einem Befehl zu beschreiben.
//
//                               Variablen
// @ addr     : Adresse des MCP2515
// @ len      : Länge des Array-Datensatzes (data) das gesendet werden soll
// @ data     : Unsigned 8-Bit-Datensatz
//
void MCP2515_write_many_registers(uint8_t addr, uint8_t len, uint8_t *data)
{ 
  uint8_t i;

  if (spi_get_status() == SPI_BUSY)
  {
    return ;
  }
  spi_set_status(SPI_BUSY);

  MCP2515_CS_LOW;                                                                // Starte Frame in dem ich !CS auf Low Ziehe. Sende ...
  
  MCP2515_SPI_transmit(MCP2515_WRITE);                                           // Schreibbefehl ; Befehl 0x02, ...
  MCP2515_SPI_transmit(addr);                                                    // sende Befehl (Register-Adresse), ...
  for(i=0; i < len; i++)                                                     // solange i < datenlänge,
  {
    MCP2515_SPI_transmit(*data);                                                 // sende Daten für diese Adresse, ...
    data++;                                                          // nächstes byte vorbereiten, ...
  } // for
  
  MCP2515_CS_HIGH;                                                               // beende den Frame in dem ich !CS wieder auf High setze und ...

  __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
  spi_set_status(SPI_READY);
}

//####################################################################################################################################################################################
//                                                                       MCP2515_read()
//
// Aus einem einzelne Resister des MCP25115 lesen
//
//                               Variablen
// @ addr     : Adresse des MCP2515
//
// @ rückgabe : Registerwert des MCP2515
//
uint8_t  MCP2515_read(uint8_t addr)
{
  uint8_t data;                                                                  // Hier soll der Darensatz die der MCP mir sendet zwischengespeichert werden

  if (spi_get_status() == SPI_BUSY)
  {
    return ;
  }
  spi_set_status(SPI_BUSY);

  MCP2515_CS_LOW;                                                                // Starte Frame in dem ich !CS auf Low Ziehe. Sende ...
  
  MCP2515_SPI_transmit(MCP2515_READ);                                            // Lesebefehl ; Befehl 0x03, ...
  MCP2515_SPI_transmit(addr);                                                    // sende Befehl (Register-Adresse), ...
  data = MCP2515_SPI_transmit(MCP2515_DUMMY);                                    // sende Dummy und empfange Daten gleichzeitig, ...
  
  MCP2515_CS_HIGH;                                                               // beende den Frame in dem ich !CS wieder auf High setze, ...

  __delay_cycles(DELAY_100us);                                                   // warte ein bischen und ...

  spi_set_status(SPI_READY);

  return data;                                                                   // gebe den Datensatz zurück.-
}

//####################################################################################################################################################################################
//                                                                MCP2515_read_many_registers()
//
// Liest mehrere Register des MCP25115 aus
//
//                               Variablen
// @ addr     : Adresse des MCP2515
// @ len      : Länge des Array-Datensatzes (data) das gesendet werden soll
// @ data     : Array aus Unsigned 8-Bit-Datensatzes (rückgabe)
//
void MCP2515_read_many_registers(uint8_t addr, uint8_t length, uint8_t *data)
{
  uint8_t i;

  if (spi_get_status() == SPI_BUSY)
  {
    return ;
  }
  spi_set_status(SPI_BUSY);

  MCP2515_CS_LOW;                                                                // Hier soll der Darensatz die der MCP mir sendet zwischengespeichert werden 
  
  MCP2515_SPI_transmit(MCP2515_WRITE);                                           // Schreibebefehl                                  
  MCP2515_SPI_transmit(addr);                                                    // Die dazugehörige Addresse
  
  for(i=1; i < length; i++)                                          //solange x < datengröße
  {
    *data = MCP2515_SPI_transmit(MCP2515_DUMMY);                                 // Sende den Dummy um die Informatinen, ...
    data++;                                                                      // zu erhalten. ...
  } // for
  
  MCP2515_CS_HIGH;                                                               // Beende den Frame in dem ich !CS wieder auf High setze, ...
  
  __delay_cycles(DELAY_100us);                                                   // und warte ein bischen.-

  spi_set_status(SPI_READY);
}

//####################################################################################################################################################################################
//                                                                     MCP2515_write_id()
//
// Soll die ID des Frames senden. Dabei muss unterschieden werden ob ein Standart Identifier (als Default ausgewählt) oder ein Extended Identifier genutzt werden soll. 
//
//                               Variablen
// @ addr     : Adresse des MCP2515. Im diesen Fall der Puffer  (TX0, TX1 oder TX2)
// @ ext      : Extended oder Standart ID (TRUE, FALSE); TRUE = Extended, FALSE = Standart
// @ id       : COB-ID der CAN-Nachricht die gesendet werden soll
//
void MCP2515_write_id(uint8_t addr, BOOL ext, unsigned long id)
{
  uint16_t canid;
  uint8_t tbufdata[4];

  canid = (unsigned short)(id & 0x0ffff);
  
  if(ext == TRUE) // Wenn 29-Bit-Identifier (CAN 2.0B)
  {
    tbufdata[MCP2515_EID0] = (uint8_t) (canid & 0xff);
    tbufdata[MCP2515_EID8] = (uint8_t) (canid / 256);
    canid = (uint16_t)(id / 0x10000);
    tbufdata[MCP2515_SIDL] = (uint8_t) (canid & 0x03);
    tbufdata[MCP2515_SIDL] +=  (uint8_t)((canid & 0x1c)*8);
    tbufdata[MCP2515_SIDL] |= MCP2515_TXBnSIDL_EXIDE;
    tbufdata[MCP2515_SIDH] = (uint8_t)(canid / 32);
  } // if
  
  else // Sonst hier auch genutzt Standart 11-Bit-Identifier (CAN 2.0A)
  {
    tbufdata[MCP2515_SIDH] = (uint8_t)(canid / 8);
    tbufdata[MCP2515_SIDL] = (uint8_t)((canid & 0x07)*32);
    tbufdata[MCP2515_EID0] = 0;
    tbufdata[MCP2515_EID8] = 0;
  } // else
  
  if(tbufdata[0] == 0xff) return;
  MCP2515_write_many_registers(addr, 4, tbufdata);
  
  __delay_cycles(DELAY_100us);

}

//####################################################################################################################################################################################
//                                                                     MCP2515_read_id()
//
// liesst die id des empfangenen frames aus
//
//                               Variablen
// @ addr     : Adresse des MCP2515. Im diesen Fall der Puffer  (RX0 oder RX1)
// @ id       : COB-ID der CAN-Nachricht die empfangen wird
//
void MCP2515_read_id(uint8_t addr, unsigned long* id)
{
  uint16_t ID_Low, ID_High;

  if(addr == MCP2515_RXB0SIDL)
  {  
    ID_Low  = (MCP2515_read(MCP2515_RXB0SIDL) >> 5);                               
    ID_High = (MCP2515_read(MCP2515_RXB0SIDH) << 3);

    *id = (unsigned long)ID_Low | (unsigned long)ID_High;
  }
  else
  {
    ID_Low  = (MCP2515_read(MCP2515_RXB1SIDL) >> 5);                               
    ID_High = (MCP2515_read(MCP2515_RXB1SIDH) << 3);

    *id = (unsigned long)ID_Low | (unsigned long)ID_High;
  }
}

//####################################################################################################################################################################################
//                                                                      MCP2515_init()
//
// Initialisiert den MCP2515-CAN-Controller
//
void MCP2515_init(void)
{ 
  // ------ 1. Resete den Chip ------------------------------------------------
  
  MCP2515_reset ();                                                              // Resete den Chip. Nach dem Reset sollte der Chip im Konfig-Modus starten.
  
  __delay_cycles(DELAY_10ms);                                                    // Gebe den MCP2515 genügend Zeit um sich rückzusetzen 
  
  // ------ 2. Konfiguriere den Chip ------------------------------------------
  
  MCP2515_write(MCP2515_CANCTRL, 0x88);                                          // CAN Controel-Register. Gehe Konfiguration-Modus (Seite 58), eigentlich sollte aber dieser automatisch nach neustart dort sein
  MCP2515_write(MCP2515_CANINTE, 0x03);                                          // Interrupt Enable-Register.  Aktiviere NUR RX0- und RX1-Interrupts (Datenblatt, Seite 50)
  MCP2515_write(MCP2515_TXB0CTRL, 0x03);                                         // Transmit Buffer Control-Register (Datenblatt, Seite 18). Highes Message Priority (interessant wenn mehrere Buffer genutzt)
  
  // ------ 2a Bit Timing ------------------------------------------------------
//10kbps
//  MCP2515_write(MCP2515_CNF1,0x1f);                                              // Bei 16MHz -> 250kb/s. Beachte um mehr als 125kBaud zu bekommen, muss ein externer Quarz von 16MHz ...
//  MCP2515_write(MCP2515_CNF2,0xbf);                                              // angelötet sein. Die Werte für CNF1, CNF2 und CNF3, kann man leicht aus der Microchi-Software bekommen. ...
//  MCP2515_write(MCP2515_CNF3,0x07);                                              // Dazu lade Programm: "Microchip Can Bit Timing Calculator" runter und führe es als ADMIN aus.-
//250kbps
  MCP2515_write(MCP2515_CNF1,0x01);                                              // Bei 16MHz -> 250kb/s. Beachte um mehr als 125kBaud zu bekommen, muss ein externer Quarz von 16MHz ...
  MCP2515_write(MCP2515_CNF2,0xB0);                                              // angelötet sein. Die Werte für CNF1, CNF2 und CNF3, kann man leicht aus der Microchi-Software bekommen. ...
  MCP2515_write(MCP2515_CNF3,0x06);                                              // Dazu lade Programm: "Microchip Can Bit Timing Calculator" runter und führe es als ADMIN aus.-
  // ------ 2b Filter einsetzen -----------------------------------------------
  
  MCP2515_write(MCP2515_RXB0CTRL, 0x64);                                         // Receive Buffer 0 Control, Alle Nachrichten Empfangen, falls nötig RX1 weiterleiten (siehe Datenblatt, Seite 27)
  MCP2515_write(MCP2515_RXB1CTRL, 0x60);                                         // Receive Buffer 1 Control, Alle Nachrichten Empfangen (siehe Datenblatt, Seite 28)
  MCP2515_write(MCP2515_BFPCTRL, 0x00);                                          // Deaktiviere RXnBF Pins (Pin 10 und 11, siehe Datenblatt, Seite 29), lösen daher kein IR an den Pin aus. 
  MCP2515_write(MCP2515_TXRTSCTRL , 0x00);                                       // Deaktiviere RTS Pins (Pin 4,5 und 6, siehe Datenblatt, Seite 19), lösen daher kein IR an den Pin aus.
  
  // ------ 3. Gehe zum Normalen Modus zurück ---------------------------------
  
  MCP2515_write(MCP2515_CANCTRL, 0x00);                                          // Kehre zum normalen Modus zurück
 
  __delay_cycles(DELAY_1s);
}

//####################################################################################################################################################################################
//                                                                    MCP2515_bit_modify()
//
// Hier werden nur bestimmte Bits gesetzt bzw. gelöscht. Ein Bespiel dafür wird man unter anderem bei der Initialisierung sehen, wo damit die Operationsmodi umgeschaltet werden. We-
// lche Bits manipuliert werden sollen, wird aus der Maske bestimmt. Die  Bits die bei der Maske auf High sind, werden bearbeitet.
//
//                               Variablen
// @ addr     : Adresse des MCP2515
// @ mask     : Maske (nur das was High ist wird manpuliert)
// @ data     : Information
//
void MCP2515_bit_modify(uint8_t addr, uint8_t mask, uint8_t data)
{
  if (spi_get_status() == SPI_BUSY)
  {
    return ;
  }
  spi_set_status(SPI_BUSY);

  MCP2515_CS_LOW;                                                                    // Starte Frame in dem ich !CS auf Low Ziehe. Sende ...
  
  MCP2515_SPI_transmit(MCP2515_BIT_MODIFY);
  MCP2515_SPI_transmit(addr);
  MCP2515_SPI_transmit(mask);
  MCP2515_SPI_transmit(data);
 
  MCP2515_CS_HIGH;                                                                   // beende den Frame in dem ich !CS wieder auf High setze, ...
  
  __delay_cycles(DELAY_100us);                                                   // warte ein bischen und ...

  spi_set_status(SPI_READY);
}


//####################################################################################################################################################################################
//                                                                     MCP2515_can_tx0()
//
// Sendet ein CAN-Frame (ID, DLC, Info) über den Buffer 0. Man kann wählen zwischen Remote Frame: Dieser dient dazu andere Teilnehme des Buss nach deren Daten abzufragen. Dabei werden 
// selber keine Daten Übertragen. Oder den Data-Frame: Sende einem Teilnehmer meine Daten. 
//
//                               Variablen
// @ can      : CAN-Variable zum Senden eines CAN-Frames
//
void MCP2515_can_tx0(can_t *can)
{ 
  if(can->dlc > 8) can->dlc = 8;                                                 // Darf Max 8 Byte Lang sein. Im schlimsten Fall werden die Informationen abgeschnitten

  MCP2515_write_id(MCP2515_TXB0SIDH, can->ext, can->COB_ID);                     // Sende zuerst die ID
    
  if (can->rtr == TRUE)                                                          // Wenn Remote Frame. Eine RTR Nachricht hat zwar eine Laenge aber keine Daten. Dient um Teilnehmer abzufragen
  {     
      uint8_t befehl = can->dlc;                                                 // Bereite vor die DLC-Länge zu senden, ...
      befehl = befehl | 0x40;                                                    // ...
      if(befehl == 0x03) return;                                                 // ...
      MCP2515_write(MCP2515_TXB0DLC, can->dlc | 0x40);                           // ... Nachrichten Länge + RTR einstellen
  } // if (rtr)
    
  else                                                                           // Oder  Daten Frame
  {
    MCP2515_write(MCP2515_TXB0DLC, can->dlc);                                    // Sende Länge des Datensatzes
    MCP2515_write_many_registers(MCP2515_TXB0D0, can->dlc, can->data);           // Und den Datensatz selber
    MCP2515_write(MCP2515_TXB0CTRL, 0x0B);                                       // Message Transmitt Resquest, Highes Priority (seite 18)
  } // else (rtr)
}

//####################################################################################################################################################################################
//                                                                     MCP2515_can_tx1()
//
// Sendet ein CAN-Frame (ID, DLC, Info) über den Buffer 1. Man kann wählen zwischen Remote Frame: Dieser dient dazu andere Teilnehme des Buss nach deren Daten abzufragen. Dabei werden 
// selber keine Daten Übertragen. Oder den Data-Frame: Sende einem Teilnehmer meine Daten. 
//
//                               Variablen
// @ can      : CAN-Variable zum Senden eines CAN-Frames
//
void MCP2515_can_tx1(can_t *can)
{
  if(can->dlc > 8) can->dlc = 8;                                                 // Darf Max 8 Byte Lang sein. Im schlimsten Fall werden die Informationen abgeschnitten

  MCP2515_write_id(MCP2515_TXB1SIDH, can->ext, can->COB_ID);                     // Sende zuerst die ID
    
  if (can->rtr == TRUE)                                                          // Wenn Remote Frame. Eine RTR Nachricht hat zwar eine Laenge aber keine Daten. Dient um Teilnehmer abzufragen
  {     
      uint8_t befehl = can->dlc;                                                 // Bereite vor die DLC-Länge zu senden, ...
      befehl = befehl | 0x40;                                                    // ...
      if(befehl == 0x03) return;                                                 // ...
      MCP2515_write(MCP2515_TXB1DLC, can->dlc | 0x40);                           // ... Nachrichten Länge + RTR einstellen
  } // if (rtr)
    
  else                                                                           // Oder  Daten Frame
  {
    MCP2515_write(MCP2515_TXB1DLC, can->dlc);                                    // Sende Länge des Datensatzes
    MCP2515_write_many_registers(MCP2515_TXB1D0, can->dlc, can->data);           // Und den Datensatz selber
    MCP2515_write(MCP2515_TXB1CTRL, 0x0B);                                       // Message Transmitt Resquest, Highes Priority (seite 18)
  } // else (rtr)  

}

//####################################################################################################################################################################################
//                                                                     MCP2515_can_tx2()
//
// Sendet ein CAN-Frame (ID, DLC, Info) über den Buffer 2. Man kann wählen zwischen Remote Frame: Dieser dient dazu andere Teilnehme des Buss nach deren Daten abzufragen. Dabei werden 
// selber keine Daten Übertragen. Oder den Data-Frame: Sende einem Teilnehmer meine Daten. 
//
//                               Variablen
// @ can      : CAN-Variable zum Senden eines CAN-Frames
//
void MCP2515_can_tx2(can_t *can)
{

  if(can->dlc > 8) can->dlc = 8;                                                 // Darf Max 8 Byte Lang sein. Im schlimsten Fall werden die Informationen abgeschnitten

  MCP2515_write_id(MCP2515_TXB2SIDH, can->ext, can->COB_ID);                     // Sende zuerst die ID
    
  if (can->rtr == TRUE)                                                          // Wenn Remote Frame. Eine RTR Nachricht hat zwar eine Laenge aber keine Daten. Dient um Teilnehmer abzufragen
  {     
      uint8_t befehl = can->dlc;                                                 // Bereite vor die DLC-Länge zu senden, ...
      befehl = befehl | 0x40;                                                    // ...
      if(befehl == 0x03) return;                                                 // ...
      MCP2515_write(MCP2515_TXB2DLC, can->dlc | 0x40);                           // ... Nachrichten Länge + RTR einstellen
  } // if (rtr)
    
  else                                                                           // Oder  Daten Frame
  {
    MCP2515_write(MCP2515_TXB2DLC, can->dlc);                                    // Sende Länge des Datensatzes
    MCP2515_write_many_registers(MCP2515_TXB2D0, can->dlc, can->data);           // Und den Datensatz selber
    MCP2515_write(MCP2515_TXB2CTRL, 0x0B);                                       // Message Transmitt Resquest, Highes Priority (seite 18)
  } // else (rtr)

}

//####################################################################################################################################################################################
//                                                                     MCP2515_can_rx0()
//
// Empfängt ein CAN-Frame über den Buffer 0. Darunter zu verstehen ist das Lesen der ID, DLC und der Information des Frames. Abschließend wird der ausgelöste IR wieder zurückgesetzt.
//
//                               Variablen
// @ can      : CAN-Variable zum Empfangen eines CAN-Frames
//
void MCP2515_can_rx0(can_t *can)
{
  uint8_t i;

  MCP2515_read_id(MCP2515_RXB0SIDL, &can->COB_ID);
  can->dlc = MCP2515_read(MCP2515_RXB0DLC);
  
  for(i = 0; i < can->dlc; i++) can->data[i] = MCP2515_read(MCP2515_RXB0D0+i);
  can->status = can->data[0];
  
  MCP2515_clear_rx0();                                                   //löscht die register des rx0
  MCP2515_int_clear();  
  
  __delay_cycles(DELAY_1ms);

}

//####################################################################################################################################################################################
//                                                                     MCP2515_can_rx1()
//
// Empfängt ein CAN-Frame über den Buffer 1. Darunter zu verstehen ist das Lesen der ID, DLC und der Information des Frames. Abschließend wird der ausgelöste IR wieder zurückgesetzt.
//
//                               Variablen
// @ can      : CAN-Variable zum Empfangen eines CAN-Frames
//
void MCP2515_can_rx1(can_t *can)
{
  uint8_t i;

  MCP2515_read_id(MCP2515_RXB1SIDL, &can->COB_ID); 
  can->dlc = MCP2515_read(MCP2515_RXB1DLC);
  
  for(i = 0; i < can->dlc; i++) can->data[i] = MCP2515_read(MCP2515_RXB1D0+i);
  can->status = can->data[0];
  
  MCP2515_clear_rx1();
  MCP2515_int_clear();
  
  __delay_cycles(DELAY_1ms);

}

//####################################################################################################################################################################################
//                                                                     MCP2515_clear_rx0()
//
// In dem nur ein einzelnes Bit modifiziert wird, wird der RX0-Interrupt zurückgesetzt (falls dieser ausgelöst wurde)
//
void MCP2515_clear_rx0(void)
{
  MCP2515_bit_modify(MCP2515_CANINTF, MCP2515_RX0IF, 0x00);
}

//####################################################################################################################################################################################
//                                                                     MCP2515_clear_rx0()
//
// In dem nur ein einzelnes Bit modifiziert wird, wird der RX1-Interrupt zurückgesetzt (falls dieser ausgelöst wurde)
//
void MCP2515_clear_rx1(void)
{
  MCP2515_bit_modify(MCP2515_CANINTF, MCP2515_RX1IF, 0x00);
}

//####################################################################################################################################################################################
//                                                                     MCP2515_int_clear()
//
// Löscht alle interrupts des mcp2515 (falls ausgelöst)
//
void MCP2515_int_clear(void)
{
  MCP2515_write(MCP2515_CANINTF, MCP2515_CANINTF_ALL_DISABLE);
}



