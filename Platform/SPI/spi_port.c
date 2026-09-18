#include <spi_port.h>
#include <spi.h>

bool SPI_Encoder_TransferDMA(uint8_t* tx, uint8_t *rx ,uint16_t length)
{
    HAL_StatusTypeDef status;

    // Set CS low to transfer data
    HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_RESET);

    status= HAL_SPI_TransmitReceive_DMA(&hspi3,tx,rx,length);

    if (status != HAL_OK) {

        HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_SET);
    }

    return (status == HAL_OK);
}

bool SPI_Encoder_TransmitReceive(uint8_t* tx, uint8_t *rx ,uint16_t length){

	HAL_StatusTypeDef status;

	// SET CS Low to begin transfer frame
    HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_RESET);

    // Bắt đầu truyền nhận frame (Polling)
    status = HAL_SPI_TransmitReceive(&hspi3, tx, rx, length, 1U);

    if(status != HAL_OK){
    	// Nếu truyền nhận không ok thì tắt CS đi
    	HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_SET);

    	return false;
    }

	// Nếu truyền nhận thành công status ok thì  bật CS lên high để kết thúc frame
    HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_SET);

    return (status == HAL_OK);
}


void SPI_Encoder_TransferComplete(void)
{
	// Set CS high
    HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_SET);
}
