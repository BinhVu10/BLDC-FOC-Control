#ifndef GPIO_PORT_H
#define GPIO_PORT_H

#include <stdbool.h>

// Enable driver
void drv_enable(void);

void drv_disable(void);

bool drv_fault(void);




#endif
