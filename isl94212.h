/*
 * isl94212.h
 *
 *  Created on: 2018Äê10ÔÂ15ÈÕ
 *      Author: Administrator
 */

#ifndef ISL94212_H_
#define ISL94212_H_

/*************************************************************/
#define ISL94212_CELL_DATA_BASE_ADDR                     0x01
/*************************************************************/
#define ISL94212_CELL_VOLTAGE_TEMPERATURE_BASE_ADDR      ISL94212_CELL_DATA_BASE_ADDR
#define ISL94212_CELL_VBAT_VOLTAGE_REGISTER_ADDR         0x00
#define ISL94212_CELL_1_VOLTAGE_REGISTER_ADDR            0x01
#define ISL94212_CELL_2_VOLTAGE_REGISTER_ADDR            0x02
#define ISL94212_CELL_3_VOLTAGE_REGISTER_ADDR            0x03
#define ISL94212_CELL_4_VOLTAGE_REGISTER_ADDR            0x04
#define ISL94212_CELL_5_VOLTAGE_REGISTER_ADDR            0x05
#define ISL94212_CELL_6_VOLTAGE_REGISTER_ADDR            0x06
#define ISL94212_CELL_7_VOLTAGE_REGISTER_ADDR            0x07
#define ISL94212_CELL_8_VOLTAGE_REGISTER_ADDR            0x08
#define ISL94212_CELL_9_VOLTAGE_REGISTER_ADDR            0x09
#define ISL94212_CELL_10_VOLTAGE_REGISTER_ADDR           0x0A
#define ISL94212_CELL_11_VOLTAGE_REGISTER_ADDR           0x0B
#define ISL94212_CELL_12_VOLTAGE_REGISTER_ADDR           0x0C
#define ISL94212_CELL_ALL_VOLTAGE_REGISTER_ADDR          0x0F
#define ISL94212_TEMPERATURE_INTERNAL_REGISTER_ADDR      0x10
#define ISL94212_TEMPERATURE_EXT1_REGISTER_ADDR          0x11
#define ISL94212_TEMPERATURE_EXT2_REGISTER_ADDR          0x12
#define ISL94212_TEMPERATURE_EXT3_REGISTER_ADDR          0x13
#define ISL94212_TEMPERATURE_EXT4_REGISTER_ADDR          0x14
#define ISL94212_REFERENCE_VOLTAGE_REGISTER_ADDR         0x15
#define ISL94212_SCAN_COUNT_REGISTER_ADDR                0x16
#define ISL94212_READ_ALL_TEMPERATURE_REGISTER_ADDR      0x1F

/*************************************************************/
#define ISL94212_SYSTEM_BASE_ADDR                        0x02
/*************************************************************/
#define ISL94212_FAULT_BASE_ADDR                         ISL94212_SYSTEM_BASE_ADDR
#define ISL94212_OVERVOLTAGE_FAULT_REGISTER_ADDR         0x00
#define ISL94212_UNDERVOLTAGE_FAULT_REGISTER_ADDR        0x01
#define ISL94212_OPENWIRE_FAULT_REGISTER_ADDR            0x02
#define ISL94212_FAULTSETUP_FAULT_REGISTER_ADDR          0x03
#define ISL94212_FAULTSTATUS_REGISTER_ADDR               0x04
#define ISL94212_CELLSETUP_REGISTER_ADDR                 0x05
#define ISL94212_OVERTEMPERATURE_FAULT_REGISTER_ADDR     0x06

/*************************************************************/
#define ISL94212_SETUP_REGISTERS_BASE_ADDR               ISL94212_SYSTEM_BASE_ADDR
#define ISL94212_OVERVOLTAGE_LIMIT_REGISTER_ADDR         0x10
#define ISL94212_UNDERVOLTAGE_LIMIT_REGISTER_ADDR        0x11
#define ISL94212_EXT_TEMPERATURE_LIMIT_REGISTER_ADDR     0x12
#define ISL94212_BALANCE_SETUP_REGISTER_ADDR             0x13
#define ISL94212_BALANCE_STATUS_REGISTER_ADDR            0x14
#define ISL94212_WATCHDOG_BALANCE_TIME_REGISTER_ADDR     0x15
#define ISL94212_COMMS_SETUP_REGISTER_ADDR               0x18
#define ISL94212_DEVICE_SETUP_REGISTER_ADDR              0x19
#define ISL94212_INTERAL_TEMPERATURE_LIMIT_REGISTER_ADDR 0x1A

#define ISL94212_USER_REGISTER_ONE_REGISTER_ADDR         0x16
#define ISL94212_USER_REGISTER_TWO_REGISTER_ADDR         0x17

#define ISL94212_SERIALNUMBER_HIGH_REGISTER_ADDR         0x1B
#define ISL94212_SERIALNUMBER_LOW_REGISTER_ADDR          0x1C

#define ISL94212_TRIMVOLTAGES_REGISTER_ADDR              0x1D
#define ISL94212_READ_ALL_SETUP_REGISTER_ADDR            0x1F

/*************************************************************/
#define ISL94212_CELL_BALANCE_REGISTER_BASE_ADDR         ISL94212_SYSTEM_BASE_ADDR
#define ISL94212_CELL_1_BALANCE_REGISTER_ADDR            0x20
#define ISL94212_CELL_1_BALANCE_REGISTER_ADDR_H          0x21
#define ISL94212_CELL_2_BALANCE_REGISTER_ADDR            0x22
#define ISL94212_CELL_2_BALANCE_REGISTER_ADDR_H          0x23
#define ISL94212_CELL_3_BALANCE_REGISTER_ADDR            0x24
#define ISL94212_CELL_3_BALANCE_REGISTER_ADDR_H          0x25
#define ISL94212_CELL_4_BALANCE_REGISTER_ADDR            0x26
#define ISL94212_CELL_4_BALANCE_REGISTER_ADDR_H          0x27
#define ISL94212_CELL_5_BALANCE_REGISTER_ADDR            0x28
#define ISL94212_CELL_5_BALANCE_REGISTER_ADDR_H          0x29
#define ISL94212_CELL_6_BALANCE_REGISTER_ADDR            0x2A
#define ISL94212_CELL_6_BALANCE_REGISTER_ADDR_H          0x2B
#define ISL94212_CELL_7_BALANCE_REGISTER_ADDR            0x2C
#define ISL94212_CELL_7_BALANCE_REGISTER_ADDR_H          0x2D
#define ISL94212_CELL_8_BALANCE_REGISTER_ADDR            0x2E
#define ISL94212_CELL_8_BALANCE_REGISTER_ADDR_H          0x2F
#define ISL94212_CELL_9_BALANCE_REGISTER_ADDR            0x30
#define ISL94212_CELL_9_BALANCE_REGISTER_ADDR_H          0x31
#define ISL94212_CELL_10_BALANCE_REGISTER_ADDR           0x32
#define ISL94212_CELL_10_BALANCE_REGISTER_ADDR_H         0x33
#define ISL94212_CELL_11_BALANCE_REGISTER_ADDR           0x34
#define ISL94212_CELL_11_BALANCE_REGISTER_ADDR_H         0x35
#define ISL94212_CELL_12_BALANCE_REGISTER_ADDR           0x36
#define ISL94212_CELL_12_BALANCE_REGISTER_ADDR_H         0x37

/*************************************************************/
#define ISL94212_REFERENCE_COEFFICIENT_BASE_ADDR         ISL94212_SYSTEM_BASE_ADDR
#define ISL94212_REFERENCE_COEFFICIENT_C_REGISTER_ADDR   0x38
#define ISL94212_REFERENCE_COEFFICIENT_B_REGISTER_ADDR   0x39
#define ISL94212_REFERENCE_COEFFICIENT_A_REGISTER_ADDR   0x3A

#define ISL94212_CELLS_BALANCE_ENABLED_REGISTER_ADDR     0x3B

/*************************************************************/
#define ISL94212_DEVICE_COMMANDS_BASE_ADDR               0x03
#define ISL94212_SCAN_VOLTAGES_COMMAND                   0x01
#define ISL94212_SCAN_TEMPERATURES_COMMAND               0x02
#define ISL94212_SCAN_MIXED_COMMAND                      0x03
#define ISL94212_SCAN_WIRES_COMMAND                      0x04
#define ISL94212_SCAN_ALL_COMMAND                        0x05
#define ISL94212_SCAN_CONTINUOUS_COMMAND                 0x06
#define ISL94212_SCAN_INHLBIT_COMMAND                    0x07
#define ISL94212_MEASURE_COMMAND                         0x08
#define ISL94212_IDENTIFY_COMMAND                        0x09
#define ISL94212_SLEEP_COMMAND                           0x0A
#define ISL94212_NAK_COMMAND                             0x0B
#define ISL94212_ACK_COMMAND                             0x0C
#define ISL94212_COMMS_FALLURE_COMMAND                   0x0E
#define ISL94212_WAKE_UP_COMMAND                         0x0F
#define ISL94212_BALANCE_ENABLE_COMMAND                  0x10
#define ISL94212_BALANCE_INHLBIT_COMMAND                 0x11
#define ISL94212_RESET_COMMAND                           0x12
#define ISL94212_CALCULATE_REGISTER_CHECKSUM_COMMAND     0x13
#define ISL94212_CHECK_REGISTER_CHECKSUM_COMMAND         0x14
/*************************************************************/
#define ISL94212_ACCESS_READ                             0x00
#define ISL94212_ACCESS_WRITE                            0x01
/*************************************************************/
#define ISL94212_CS_LOW                                  SPI_CS_LOW
#define ISL94212_CS_HIGH                                 SPI_CS_HIGH

#define ISL94212_DUMMY                                   0xff
/*************************************************************/
#define ISL94212_BALANCE_STATUS_REGISTER_0_ADDR          0x00
#define ISL94212_BALANCE_STATUS_REGISTER_1_ADDR          0x01
#define ISL94212_BALANCE_STATUS_REGISTER_2_ADDR          0x02
#define ISL94212_BALANCE_STATUS_REGISTER_3_ADDR          0x03
#define ISL94212_BALANCE_STATUS_REGISTER_4_ADDR          0x04
#define ISL94212_BALANCE_STATUS_REGISTER_5_ADDR          0x05
#define ISL94212_BALANCE_STATUS_REGISTER_6_ADDR          0x06
#define ISL94212_BALANCE_STATUS_REGISTER_7_ADDR          0x07
#define ISL94212_BALANCE_STATUS_REGISTER_8_ADDR          0x08
#define ISL94212_BALANCE_STATUS_REGISTER_9_ADDR          0x09
#define ISL94212_BALANCE_STATUS_REGISTER_10_ADDR         0x0A
#define ISL94212_BALANCE_STATUS_REGISTER_11_ADDR         0x0B
#define ISL94212_BALANCE_STATUS_REGISTER_12_ADDR         0x0C
/*************************************************************/
#define ISL94212_BALANCE_MODE_OFF                        0x00
#define ISL94212_BALANCE_MODE_MANUAL                     0x01
#define ISL94212_BALANCE_MODE_TIMED                      0x02
#define ISL94212_BALANCE_MODE_AUTO                       0x03
/*************************************************************/
#define ISL94212_BALANCE_WAIT_TIME_0                     0x00
#define ISL94212_BALANCE_WAIT_TIME_1                     0x01
#define ISL94212_BALANCE_WAIT_TIME_2                     0x02
#define ISL94212_BALANCE_WAIT_TIME_4                     0x03
#define ISL94212_BALANCE_WAIT_TIME_8                     0x04
#define ISL94212_BALANCE_WAIT_TIME_16                    0x05
#define ISL94212_BALANCE_WAIT_TIME_32                    0x06
#define ISL94212_BALANCE_WAIT_TIME_64                    0x07
/*************************************************************/
#define ISL94212_WATCHDOG_DISABLE                        0x00
#define ISL94212_BALANCE_CYCLE_DISABLE                   0x00
/*************************************************************/
#pragma pack(1)
typedef union
{
    unsigned char bytes[2];
    struct
    {
        unsigned char TRAILING  :6;
        unsigned char DATA_ADDR :6;
        unsigned char PAGE_ADDR :3;
        unsigned char RW        :1;
    }bits;

}ISL94212_Command_InitTypeDef;

#pragma pack(1)
typedef union
{
    unsigned char bytes[3];
    struct
    {
        unsigned short Data     :14;
        unsigned char DATA_ADDR :6;
        unsigned char PAGE_ADDR :3;
        unsigned char RW        :1;
    }bits;
}ISL94212_Write_InitTypeDef;

#pragma pack(1)
typedef union
{
    unsigned short Short;
    struct
    {
        unsigned char BMOD          :2;
        unsigned char BWT           :3;
        unsigned char BSP           :4;
        unsigned char BEN           :1;
        unsigned char Reserved      :4;
    }bits;
}ISL94212_Balance_Setup_InitTypeDef;

#pragma pack(1)
typedef union
{
    unsigned short Short;
    struct
    {
        unsigned char WDG           :7;
        unsigned char BTM           :7;
        unsigned char Reserved      :2;
    }bits;
}ISL94212_Balance_WathcDog_Balance_Time_InitTypedef;

#pragma pack(1)
typedef union
{
    unsigned short Short;
    struct
    {
        unsigned char PIN37          :1;
        unsigned char PIN39          :1;
        unsigned char Reserved       :1;
        unsigned char EOB            :1;
        unsigned char SCAN           :1;
        unsigned char ISCN           :1;
        unsigned char Reserved1      :1;
        unsigned char BDDS           :1;
        unsigned char WP             :6;
    }bits;
}ISL94212_Device_Setup_InitTypedef;

void ISL94212_Init(void);
void ISL94212_updateReadings(void);

#endif /* ISL94212_H_ */
