/*
 * isl94212.c
 *
 *  Created on: 2018年10月15日
 *      Author: Administrator
 */

#include <msp430.h>
#include "isl94212.h"
#include "spi.h"

unsigned char CurrentVoltageBuffer[26];
unsigned char CurrentTemperatureBuffer[14];

unsigned char ISL94212_SPI_transmit(unsigned char daten)
{
    return spi_transmit(daten);
}

//发送命令
void ISL94212_Send_DEVICE_COMMAND(unsigned char cmd)
{
    ISL94212_Command_InitTypeDef ISL94212_Command;
    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_DEVICE_COMMANDS_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = cmd;
    ISL94212_Command.bits.TRAILING = 0;
    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-

    spi_set_status(SPI_READY);
}

//读取电池电压
unsigned short ISL94212_Read_Cell_Voltage(unsigned char cell)
{
    unsigned char data;
    unsigned short Voltage;
    ISL94212_Command_InitTypeDef ISL94212_Command;
    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_CELL_VOLTAGE_TEMPERATURE_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = cell;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return 0;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                 // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    Voltage = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    Voltage += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);

    return Voltage * 625.0 / 1024;
}

//更新电池电压
void ISL94212_updateReadings(void)
{
    unsigned char i;
    ISL94212_Command_InitTypeDef ISL94212_Command;
    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_CELL_VOLTAGE_TEMPERATURE_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_CELL_ALL_VOLTAGE_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    for (i = 0; i < 26; i++)
    {
        ISL94212_CS_LOW;
        CurrentVoltageBuffer[i] = ISL94212_SPI_transmit(ISL94212_DUMMY);
        ISL94212_CS_HIGH;
        __delay_cycles(DELAY_10us);
    }

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);

}

//读取温度
unsigned char ISL94212_Read_Temperature(unsigned char num)
{
    unsigned char data;
    unsigned short temperature;
    ISL94212_Command_InitTypeDef ISL94212_Command;
    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_CELL_VOLTAGE_TEMPERATURE_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = num;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return 0;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    temperature = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    temperature += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);

    return temperature;
}

//更新温度
void ISL94212_updateTemperature(void)
{
    unsigned char i;
    ISL94212_Command_InitTypeDef ISL94212_Command;
    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_CELL_VOLTAGE_TEMPERATURE_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_READ_ALL_TEMPERATURE_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    for (i = 0; i < 14; i++)
    {
        ISL94212_CS_LOW;
        CurrentVoltageBuffer[i] = ISL94212_SPI_transmit(ISL94212_DUMMY);
        ISL94212_CS_HIGH;
        __delay_cycles(DELAY_10us);
    }

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

//读取错误状态
unsigned short ISL94212_Read_Fault(unsigned char Fault)
{
    unsigned char data;
    unsigned short value;
    ISL94212_Command_InitTypeDef ISL94212_Command;
    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_FAULT_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = Fault;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return 0;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);

    return value;
}

//清除错误状态
void ISL94212_Clear_Fault(unsigned char Fault)
{
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_FAULT_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = Fault;
    ISL94212_Write.bits.Data = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

//设置过压阈值
void ISL94212_Set_OV(unsigned short OV)
{
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_OVERVOLTAGE_LIMIT_REGISTER_ADDR;
    ISL94212_Write.bits.Data = OV&0x7fff;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

//设置欠压阈值
void ISL94212_Set_UV(unsigned short UV)
{
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_UNDERVOLTAGE_LIMIT_REGISTER_ADDR;
    ISL94212_Write.bits.Data = UV&0x7fff;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

//设置温度阈值
void ISL94212_Set_Temperature_Limit(unsigned char Temp)
{
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_EXT_TEMPERATURE_LIMIT_REGISTER_ADDR;
    ISL94212_Write.bits.Data = Temp;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

//设置平衡参数
void ISL94212_Set_Balance_Param(ISL94212_Balance_Setup_InitTypeDef Setup)
{
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Write.bits.Data = Setup.Short;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

//设置平衡策略
void ISL94212_Set_Balance_Status(unsigned char Index,unsigned short Status)
{
    unsigned char data;
    unsigned short value;
    ISL94212_Balance_Setup_InitTypeDef ISL94212_Balance_Setup;
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Command_InitTypeDef ISL94212_Command;

    if (Index > 12)
    {
        return ;
    }

    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_Balance_Setup.Short = value;
    ISL94212_Balance_Setup.bits.BSP = Index;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Write.bits.Data = ISL94212_Balance_Setup.Short;

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);

    ISL94212_Write.bits.DATA_ADDR = ISL94212_BALANCE_STATUS_REGISTER_ADDR;
    ISL94212_Write.bits.Data = Status&0x3fff;

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

void ISL94212_Set_Balance_Mode(unsigned char mode)
{
    unsigned char data;
    unsigned short value;
    ISL94212_Balance_Setup_InitTypeDef ISL94212_Balance_Setup;
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Command_InitTypeDef ISL94212_Command;

    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_Balance_Setup.Short = value;
    ISL94212_Balance_Setup.bits.BMOD = mode;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Write.bits.Data = ISL94212_Balance_Setup.Short;

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

void ISL94212_Set_Balance_Enable(unsigned char enable)
{
    unsigned char data;
    unsigned short value;
    ISL94212_Balance_Setup_InitTypeDef ISL94212_Balance_Setup;
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Command_InitTypeDef ISL94212_Command;

    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_Balance_Setup.Short = value;
    ISL94212_Balance_Setup.bits.BEN = enable;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Write.bits.Data = ISL94212_Balance_Setup.Short;

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

void ISL94212_Set_Balance_Wait_Time(unsigned char time)
{
    unsigned char data;
    unsigned short value;
    ISL94212_Balance_Setup_InitTypeDef ISL94212_Balance_Setup;
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Command_InitTypeDef ISL94212_Command;

    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_Balance_Setup.Short = value;
    ISL94212_Balance_Setup.bits.BWT = time;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Write.bits.Data = ISL94212_Balance_Setup.Short;

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

void ISL94212_Set_Watchdog_Time(unsigned char Watchdog)
{
    unsigned char data;
    unsigned short value;
    ISL94212_Balance_WathcDog_Balance_Time_InitTypedef ISL94212_WatchDog_Time;
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Command_InitTypeDef ISL94212_Command;

    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_WatchDog_Time.Short = value;
    ISL94212_WatchDog_Time.bits.WDG = Watchdog;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_WATCHDOG_BALANCE_TIME_REGISTER_ADDR;
    ISL94212_Write.bits.Data = ISL94212_WatchDog_Time.Short;

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

void ISL94212_Set_Balance_Time(unsigned char Balance_Time)
{
    unsigned char data;
    unsigned short value;
    ISL94212_Balance_WathcDog_Balance_Time_InitTypedef ISL94212_Balance_Time;
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Command_InitTypeDef ISL94212_Command;

    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_BALANCE_SETUP_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_Balance_Time.Short = value;
    ISL94212_Balance_Time.bits.BTM = Balance_Time;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_WATCHDOG_BALANCE_TIME_REGISTER_ADDR;
    ISL94212_Write.bits.Data = ISL94212_Balance_Time.Short;

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

void ISL94212_Set_Cell_Balance_Data(unsigned char num, unsigned int value)
{
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_CELL_BALANCE_REGISTER_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = num;
    ISL94212_Write.bits.Data = (unsigned short)(value&0x3fff);

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-


    ISL94212_Write.bits.DATA_ADDR = num+1;
    ISL94212_Write.bits.Data = (unsigned short)((value >> 14)&0x3fff);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

unsigned short ISL94212_Read_ReferenceCoefficient(unsigned char cell)
{
    unsigned char data;
    unsigned short value;
    ISL94212_Command_InitTypeDef ISL94212_Command;
    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_REFERENCE_COEFFICIENT_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = cell;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return 0;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                 // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);

    return value;
}

void ISL94212_Set_Cells_Balance_Enable(unsigned short Enable)
{
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SYSTEM_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_SYSTEM_BASE_ADDR;
    ISL94212_Write.bits.Data = (unsigned short)(Enable&0x3fff);

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);
}

void ISL94212_Set_Device_Setup_BDDS(unsigned char BDDS)
{
    unsigned char data;
    unsigned short value;
    ISL94212_Device_Setup_InitTypedef ISL94212_Device_Setup;
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Command_InitTypeDef ISL94212_Command;

    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_DEVICE_SETUP_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_Device_Setup.Short = value;
    ISL94212_Device_Setup.bits.BDDS = BDDS;
    ISL94212_Write.bits.RW = ISL94212_ACCESS_WRITE;
    ISL94212_Write.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Write.bits.DATA_ADDR = ISL94212_DEVICE_SETUP_REGISTER_ADDR;
    ISL94212_Write.bits.Data = ISL94212_Device_Setup.Short;

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[2]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Write.bytes[0]);
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_100us);                                                   // warte ein bischen.-
    spi_set_status(SPI_READY);


    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;
}

unsigned char data;
unsigned short value;

void ISL94212_Init(void)
{
    //enable isl94212
    ISL94212_Balance_WathcDog_Balance_Time_InitTypedef ISL94212_WatchDog_Time;
    ISL94212_Write_InitTypeDef ISL94212_Write;
    ISL94212_Command_InitTypeDef ISL94212_Command;

    ISL94212_Command.bits.RW = ISL94212_ACCESS_READ;
    ISL94212_Command.bits.PAGE_ADDR = ISL94212_SETUP_REGISTERS_BASE_ADDR;
    ISL94212_Command.bits.DATA_ADDR = ISL94212_WATCHDOG_BALANCE_TIME_REGISTER_ADDR;
    ISL94212_Command.bits.TRAILING = 0;

    if (spi_get_status() == SPI_BUSY)
    {
        return ;
    }

    spi_set_status(SPI_BUSY);

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[1]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    ISL94212_SPI_transmit(ISL94212_Command.bytes[0]);
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-                                                  // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value = data<<8;
    ISL94212_CS_HIGH;
    __delay_cycles(DELAY_10us);                                                   // warte ein bischen.-

    ISL94212_CS_LOW;
    data = ISL94212_SPI_transmit(ISL94212_DUMMY);
    value += data;
    ISL94212_CS_HIGH;

    __delay_cycles(DELAY_10us);

//    //Step 1. Write Balance Value registers
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_1_BALANCE_REGISTER_ADDR,0x406A);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_2_BALANCE_REGISTER_ADDR,0x3E4D);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_3_BALANCE_REGISTER_ADDR,0x0000);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_4_BALANCE_REGISTER_ADDR,0x292F);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_5_BALANCE_REGISTER_ADDR,0x3E00);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_6_BALANCE_REGISTER_ADDR,0x0000);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_7_BALANCE_REGISTER_ADDR,0x2903);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_8_BALANCE_REGISTER_ADDR,0x3D06);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_9_BALANCE_REGISTER_ADDR,0x0000);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_10_BALANCE_REGISTER_ADDR,0x151E);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_11_BALANCE_REGISTER_ADDR,0x0502);
//    ISL94212_Set_Cell_Balance_Data(ISL94212_CELL_12_BALANCE_REGISTER_ADDR,0x06D6);
//
//    //Step 2. Write BDDS bit in Device Setup register (turn balancing functions off during measurement)
//    ISL94212_Set_Device_Setup_BDDS(1);
//
//    //Step 3. Write balance timeout setting to the Watchdog/Balance Time register: Balance timeout code = 0000001 (20 seconds)
//    ISL94212_Set_Balance_Time(1);
//
//    //Step 4. Set up Balance Status register
//
//    //Step 4A. Write Balance Setup register: Set Auto Balance mode, set 8 second Balance wait time, and set balance off:
//    ISL94212_Balance_Setup_InitTypeDef Balance_Setup;
//    Balance_Setup.bits.BMOD = ISL94212_BALANCE_MODE_AUTO;
//    Balance_Setup.bits.BWT = 8;
//    Balance_Setup.bits.BSP = 0;
//    Balance_Setup.bits.BEN = 0;
//    ISL94212_Set_Balance_Param(Balance_Setup);
//
//    //Step 4B. Write Balance Setup register: Set Balance Status Pointer = 1
//    //Step 4C. Write Balance Status register: Set bits 1, 4, 7 and 10
//    ISL94212_Set_Balance_Status(ISL94212_BALANCE_STATUS_REGISTER_1_ADDR,0x0249);
//
//    //Step 4D. Write Balance Setup register: Set Balance Status Pointer = 2
//    //Step 4E. Write Balance Status register: Set bits 3, 6, 9 and 12
//    ISL94212_Set_Balance_Status(ISL94212_BALANCE_STATUS_REGISTER_2_ADDR,0x0924);
//
//    //Step 4F. Write Balance Setup register: Set Balance Status
//    //Step 4G. Write Balance Status register: Set bits 2, 5, 8 and 11
//    ISL94212_Set_Balance_Status(ISL94212_BALANCE_STATUS_REGISTER_3_ADDR,0x0492);
//
//    //Step 4H. Write Balance Setup register: Set Balance Status Pointer = 4
//    //Step 4I. Write Balance Status register: Set bits to all zero to set the end point for the instances.
//    ISL94212_Set_Balance_Status(ISL94212_BALANCE_STATUS_REGISTER_4_ADDR,0x0000);
//
//    //Step 5. Enable balancing using the Balance Enable command
//    ISL94212_Set_Balance_Enable(1);

}
