#ifndef __MY_SPI_H__
#define __MY_SPI_H__

#include <stdint.h>

#include "main.h"

void SPI_WriteByte(uint8_t byte);
void SPI_WriteWord(uint16_t word);
void SPI_Write(uint8_t *data, size_t len);

#endif
