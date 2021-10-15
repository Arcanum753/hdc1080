#include "i2c.h"
#include "hdc1080.h"

I2C_HandleTypeDef* hi2c_hdc1080;

bool hdc1080_init(I2C_HandleTypeDef* _hi2c_hdc1080, Temp_Reso Temperature_Resolution_x_bit, Humi_Reso Humidity_Resolution_x_bit)	{
	/* Temperature and Humidity are acquired in sequence,  Temperature first
	 * Default:   Temperature resolution = 14 bit,
	 *            Humidity resolution = 14 bit
	 */

	/* Set the acquisition mode to measure both temperature and humidity by setting Bit[12] to 1 */
	uint16_t config_reg_value=0x1000;
	uint8_t data_send[2];
	hi2c_hdc1080 =  _hi2c_hdc1080;
	if(Temperature_Resolution_x_bit == Temperature_Resolution_11_bit)
	{
		config_reg_value |= (1<<10); //11 bit
	}

	switch(Humidity_Resolution_x_bit)
	{
		case Humidity_Resolution_11_bit:
			config_reg_value|= (1<<8);
			break;
		case Humidity_Resolution_8_bit:
			config_reg_value|= (1<<9);
			break;
		case Humidity_Resolution_14_bit:
			break;
	}

	data_send[0]= (config_reg_value>>8);
	data_send[1]= (config_reg_value&0x00ff);


	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_I2C_IsDeviceReady(hi2c_hdc1080,  HDC_1080_ADD  ,  10,  100);
	if(status == HAL_OK)
	{
		HAL_I2C_Mem_Write(hi2c_hdc1080, HDC_1080_ADD, Configuration_register_add, I2C_MEMADD_SIZE_8BIT, data_send, 2, 1000);
		return true;
	}
	else
	{
		return false;
	}
}


int8_t hdc1080_start_measurement( float* temperature,  uint8_t* humidity)	{
	HAL_StatusTypeDef res = HAL_TIMEOUT;
	uint8_t receive_data[4];
	uint16_t temp_x, humi_x;
	uint8_t send_data = Temperature_register_add;

	HAL_I2C_Master_Transmit(hi2c_hdc1080, HDC_1080_ADD, &send_data, 1, 1000);

	HAL_Delay(20);
//	while(HAL_I2C_GetState(&I2C_INSTANCE) != HAL_I2C_STATE_READY) {	}

	/* Read temperature and humidity */
	res = HAL_I2C_Master_Receive(hi2c_hdc1080, HDC_1080_ADD, receive_data, 4, 1000);

	if (res != HAL_OK){
		return 0;
	}
	temp_x =((receive_data[0]<<8)|receive_data[1]);
	humi_x =((receive_data[2]<<8)|receive_data[3]);

	*temperature=((temp_x/65536.0)*165.0)-40.0;
	*humidity=(uint8_t)((humi_x/65536.0)*100.0);
	return 1;

}


