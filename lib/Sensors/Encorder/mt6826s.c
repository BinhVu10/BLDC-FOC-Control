#include <stddef.h>
#include "mt6826S.h"

#define CMD_BURST_READ 		0xA0U						// Lệnh đọc góc
#define ANGLE_ADDRESS_REG   0x03U						// Địa chỉ của thanh ghi lưu góc

static uint8_t encoder_crc_calc(const uint8_t *data, uint8_t len);

// Hàm init encorder
void encoder_init(encoder_t* encoder)
{
  if(encoder!=NULL)
  {
    encoder->raw_angle = 0;

    encoder->dma_busy = false;

    encoder->data_valid = false;

    encoder->status = 0U;

    encoder->crc = 0U;

    encoder->crc_fault = false;

    encoder->crc_fault_cnt = 0U;
  }
}

//hàm transfer DMA gửi lệnh đọc angle từ encorder trả về
bool encoder_start_dma(encoder_t *encoder)
{
    bool status = false;

    if((encoder != NULL) && (encoder->dma_busy == false)){

		encoder -> tx_buffer[0]= CMD_BURST_READ;

		encoder -> tx_buffer[1]= ANGLE_ADDRESS_REG;

		encoder -> tx_buffer[2] = 0U;

		encoder -> tx_buffer[3] = 0U;

		encoder -> tx_buffer[4] = 0U;

		encoder -> tx_buffer[5] = 0U;

		encoder -> data_valid = false;

		encoder -> dma_busy = true;



		status = SPI_Encoder_TransferDMA(encoder->tx_buffer,encoder->rx_buffer, MT6826_FRAME_LENGTH);

		if(status == false){

			encoder -> dma_busy = false;
		}
    }

    return status;


}

// Hàm transmit receive polling angle
void encoder_start_TransmitReceive(encoder_t *encoder){
	bool status;

	// Chuẩn bị tx frame đề truyền
	if(encoder != NULL){

		encoder -> tx_buffer[0]= CMD_BURST_READ;

		encoder -> tx_buffer[1]= ANGLE_ADDRESS_REG;

		encoder -> tx_buffer[2] = 0U;

		encoder -> tx_buffer[3] = 0U;

		encoder -> tx_buffer[4] = 0U;

		encoder -> tx_buffer[5] = 0U;

		encoder -> data_valid = false;

		// Dùng SPI API để gửi data đi, tại đây sẽ polling
		status = SPI_Encoder_TransmitReceive(encoder->tx_buffer,encoder->rx_buffer, MT6826_FRAME_LENGTH);

		// Nếu mà quá trình truyền nhận có vấn đề thì ta sẽ set data valid false
		if(status == false){
			encoder -> data_valid = false;
		}
		// Nếu quá trình truyền nhận thành công thì set data valid  = true
		else{
			encoder -> data_valid = true;

		}
	}
}


// Khi gọi ra ta sẽ lưu giá trị trả về từ buffer lưu vào trường raw_angle
void encoder_dma_complete(encoder_t *encoder)
{
    if (encoder != NULL)
    {
        encoder->dma_busy = false;

        (void)encoder_read_angle(encoder);
    }

    // Set CS high để đánh dấu kết thúc truyền nhận

    SPI_Encoder_TransferComplete();
}

// Hàm lấy ra góc đã lưu trong struct
float encoder_read_angle(encoder_t *encoder)
{
    float angle = 0.0f;

    uint16_t raw_angle = 0U;

    uint8_t crc_calc = 0U;

    if (encoder == NULL)
    {
        return angle;
    }

	// Đọc ra từ 2 bytes 3 và 4 để lấy góc
	raw_angle = ((uint16_t)encoder->rx_buffer[2] << 8U) | (uint16_t)encoder->rx_buffer[3];

	// Đọc ra từ 3 bit thấp của byte 5 để lấy ra status
	encoder->status = encoder->rx_buffer[4] & 0x07U;

	// Đọc ra bytes 6 để lấy ra CRC
	encoder->crc = encoder->rx_buffer[5];

	crc_calc = encoder_crc_calc(&encoder->rx_buffer[2], 3U);

	if(crc_calc != encoder->crc){

		encoder->crc_fault = true;

		if (encoder->crc_fault_cnt < UINT16_MAX)
		{
			encoder->crc_fault_cnt++;
		}

		encoder->data_valid = false;
	}
	else{
		raw_angle >>= 1U;					// Shift 1 bit vì encoder 15 bit thôi

		encoder->raw_angle = raw_angle;

		// Chuyển góc 15 bit sang [0, 2*pi) radian
		angle = (float)encoder->raw_angle * 6.2831853072f / 32768.0f;

		encoder->crc_fault = false;

		encoder->data_valid = true;
	}

	return angle;
}


static uint8_t encoder_crc_calc(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0U;

    for (uint8_t i = 0; i < len; i++)
    {
        crc ^= data[i];

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 0x80U)
            {
                crc = (uint8_t)((crc << 1) ^ 0x07U);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

