#ifndef _W24C02_H_
#define _W24C02_H_

#include "i2c.h"
#include "com_debug.h"


#define W24C02_ADDR 0xA0 //设备地址


void W24C02_WriteByte(uint8_t byte_addr, uint8_t data);
uint8_t W24C02_ReadByte(uint8_t byte_addr);
void W24C02_WriteBytes(uint8_t byte_addr, uint8_t *data, uint8_t len);
void W24C02_ReadBytes(uint8_t byte_addr, uint8_t *data, uint8_t len);


#endif // _W24C02_H_
