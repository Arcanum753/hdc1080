#ifndef __HDC1080_H
#define __HDC1080_H

#include <stdbool.h>
#include "stm32f1xx_hal.h"

#define 		HDC_Temp_crit							-35
#define 		HDC_Humi_crit							99
#define         HDC_1080_ADD                            (0x40 << 1)
#define         Configuration_register_add              0x02
#define         Temperature_register_add                0x00
#define         Humidity_register_add                   0x01
#define 		DEV_ID									0x1050
#define 		DEV_ID_REG								0xFF


typedef enum
{
  Temperature_Resolution_14_bit = 0,
  Temperature_Resolution_11_bit = 1
}Temp_Reso;

typedef enum
{
  Humidity_Resolution_14_bit = 0,
  Humidity_Resolution_11_bit = 1,
  Humidity_Resolution_8_bit =2
}Humi_Reso;

bool hdc1080_init(I2C_HandleTypeDef* _hi2c_hdc1080, Temp_Reso Temperature_Resolution_x_bit, Humi_Reso Humidity_Resolution_x_bit);
int8_t hdc1080_start_measurement( float* temperature,  uint8_t* humidity);



#endif

