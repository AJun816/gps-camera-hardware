#ifndef __OV7670_H
#define __OV7670_H


void OV7670_Init(void);

void OV7670_GetPic(void);

void OV7670_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t OV7670_ReadReg(uint8_t RegAddress);

#endif
