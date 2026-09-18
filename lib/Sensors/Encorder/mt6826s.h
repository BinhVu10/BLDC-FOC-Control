#ifndef ENCORDER_H
#define ENCORDER_H

#include <stdbool.h>
#include <stdint.h>
#include "spi_port.h"

#define MT6826_FRAME_LENGTH 6U

typedef struct
{
    uint8_t tx_buffer[MT6826_FRAME_LENGTH];

    uint8_t rx_buffer[MT6826_FRAME_LENGTH];

    uint16_t raw_angle;

    bool dma_busy;

    bool data_valid;

    uint8_t status;

    uint8_t crc;

    bool crc_fault;

    uint16_t crc_fault_cnt;

} encoder_t;

// hàm khởi tạo đối tượng encoder
void encoder_init(encoder_t *encoder);

//hàm transfer DMA gửi lệnh đọc angle từ encorder trả về
bool encoder_start_dma (encoder_t *encoder);

// hàm đánh dấu hoàn thành truyền nhận
void encoder_dma_complete(encoder_t *encoder);

// hàm giải mã lấy góc encoder
float encoder_read_angle (encoder_t *encoder);

// Hàm đọc encoder polling
void encoder_start_TransmitReceive(encoder_t *encoder);



#endif
