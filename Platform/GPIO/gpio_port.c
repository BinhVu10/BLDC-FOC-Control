#include "gpio_port.h"
#include "main.h"


void drv_enable(void)
{
    HAL_GPIO_WritePin(nSleep_GPIO_Port, nSleep_Pin, GPIO_PIN_SET);
}

void drv_disable(void)
{
    HAL_GPIO_WritePin(nSleep_GPIO_Port, nSleep_Pin, GPIO_PIN_RESET);
}

bool drv_fault(void)
{
    return (HAL_GPIO_ReadPin(nFault_GPIO_Port, nFault_Pin) == GPIO_PIN_RESET);
}
