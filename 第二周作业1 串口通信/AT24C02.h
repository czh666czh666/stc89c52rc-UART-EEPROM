#ifndef __AT24C02_H__
#define __AT24C02_H__

void AT24C02_Delay5ms(void);
void AT24C02_WriteByte(unsigned char WordAddress,Data);
unsigned char AT24C02_ReadByte(unsigned char WordAddress);

#endif