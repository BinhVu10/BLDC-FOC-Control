#ifndef SPI_PORT_H
#define SPI_PORT_H

#include <stdint.h>
#include <stdbool.h>

bool SPI_Encoder_TransferDMA(uint8_t* tx, uint8_t *rx ,uint16_t length);

void SPI_Encoder_TransferComplete(void);

bool SPI_Encoder_TransmitReceive(uint8_t* tx, uint8_t *rx ,uint16_t length);

#endif
