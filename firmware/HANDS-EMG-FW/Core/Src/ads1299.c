#include <stdint.h>

#include "ads1299.h"

volatile uint8_t RX_BUFF[BUFF_SIZE];

extern UART_HandleTypeDef huart2;

ads1299_error_t ads1299_device_init(SPI_HandleTypeDef *hspi, uint8_t init_regs) {
	/* Power cycle ADS1299 */
//	HAL_GPIO_WritePin(ADS1299_NPWDN_GPIO_Port, ADS1299_NPWDN_Pin, GPIO_PIN_RESET);
//	HAL_Delay(2);
//	HAL_GPIO_WritePin(ADS1299_NPWDN_GPIO_Port, ADS1299_NPWDN_Pin, GPIO_PIN_SET);

	/* Allow oscillator warm-up */
	HAL_Delay(1500);

	HAL_GPIO_WritePin(ADS1299_NRESET_GPIO_Port, ADS1299_NRESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(ADS1299_NRESET_GPIO_Port, ADS1299_NRESET_Pin, GPIO_PIN_SET);

	HAL_Delay(1500);


	/* Tell chip to exit continuous data mode */
	ads1299_send_byte(hspi, ADS1299_COMMAND_SDATAC);
	/* Stop taking conversions; apparently not done automatically */
	ads1299_send_byte(hspi, ADS1299_COMMAND_STOP);

	if (init_regs)
	{

		/* Write to GPIO register, set all pins to driven-low output */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_GPIO, ADS1299_REG_GPIO_GPIOC4_OUTPUT |
		ADS1299_REG_GPIO_GPIOD4_LOW    |
		ADS1299_REG_GPIO_GPIOC3_OUTPUT |
		ADS1299_REG_GPIO_GPIOD3_LOW    |
		ADS1299_REG_GPIO_GPIOC2_OUTPUT |
		ADS1299_REG_GPIO_GPIOD2_LOW    |
		ADS1299_REG_GPIO_GPIOC1_OUTPUT |
		ADS1299_REG_GPIO_GPIOD1_LOW    );

		/* Write to CONFIG1, set data rate to 2000 Hz */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CONFIG1, ADS1299_REG_CONFIG1_RESERVED_VALUE |
		ADS1299_REG_CONFIG1_FMOD_DIV_BY_512);

		/* Write to CONFIG2 register, generate test signal internally */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CONFIG2, ADS1299_REG_CONFIG2_RESERVED_VALUE |
		ADS1299_REG_CONFIG2_CAL_INT        |
		ADS1299_REG_CONFIG2_CAL_PULSE_FCLK_DIV_2_21);

		/* Write to CONFIG3, enable internal reference buffer, bias internally generated, bias buffer enabled */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CONFIG3, ADS1299_REG_CONFIG3_REFBUF_ENABLED |
		ADS1299_REG_CONFIG3_RESERVED_VALUE |
		ADS1299_REG_CONFIG3_BIAS_MEAS_ENABLED |
		ADS1299_REG_CONFIG3_BIASREF_INT    |
		ADS1299_REG_CONFIG3_BIASBUF_ENABLED);

		/* Reference settling time */
		HAL_Delay(150);

		/* Write to CH1 settings register, set as normal input, gain 24 */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CH1SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_24		|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);
		/* Write to CH2 settings register, set as normal input, gain 24 */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CH2SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_24			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);
		/* Write to CH3 settings register, set as normal input, gain 24 */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CH3SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_24			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);
		/* Write to CH4 settings register, set as normal input, gain 24 */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CH4SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_24			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);
		/* Write to CH5 settings register, set as normal input, gain 24 */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CH5SET, ADS1299_REG_CHNSET_CHANNEL_OFF			|
		ADS1299_REG_CHNSET_GAIN_24			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);
		/* Write to CH6 settings register, set as normal input, gain 24 */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CH6SET, ADS1299_REG_CHNSET_CHANNEL_OFF			|
		ADS1299_REG_CHNSET_GAIN_24			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);
		/* Write to CH7 settings register, set as normal input, gain 24 */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CH7SET, ADS1299_REG_CHNSET_CHANNEL_OFF			|
		ADS1299_REG_CHNSET_GAIN_24			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);
		/* Write to CH8 settings register, set as normal input, gain 24 */
		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_CH8SET, ADS1299_REG_CHNSET_CHANNEL_OFF			|
		ADS1299_REG_CHNSET_GAIN_24			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);

		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_BIAS_SENSP, ADS1299_REG_BIAS_SENSP_BIASP1    |
		ADS1299_REG_BIAS_SENSP_BIASP2    |
		ADS1299_REG_BIAS_SENSP_BIASP3    |
		ADS1299_REG_BIAS_SENSP_BIASP4   );

		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_BIAS_SENSN, ADS1299_REG_BIAS_SENSN_BIASN1    |
		ADS1299_REG_BIAS_SENSN_BIASN2   |
		ADS1299_REG_BIAS_SENSN_BIASN3   |
		ADS1299_REG_BIAS_SENSN_BIASN4   );


		/* Write to MISC1 register, SRB1 on (ref electrode) */
//		ads1299_wreg(hspi, ADS1299_ADDRESS_REG_MISC1, ADS1299_REG_MISC1_SRB1_OFF);
	}
	return ADS1299_STATUS_OK;
}


ads1299_error_t ads1299_rreg(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t* read_reg_val_ptr)
{
	uint8_t read_data[1];

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

	/* First byte: read command for specified register */
	/* Second byte: Read only 1 register (send n-1, where n is number of registers to read) */
	HAL_SPI_Transmit(hspi, (uint8_t[]){ ADS1299_COMMAND_RREG | reg_addr, 0x00 }, 2, HAL_MAX_DELAY);

	/* Read the data */
	HAL_SPI_Receive(hspi, read_data, 1, HAL_MAX_DELAY);

	HAL_Delay(10);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

	*read_reg_val_ptr = (uint8_t)read_data[1];

	return ADS1299_STATUS_OK;
}

ads1299_error_t ads1299_wreg(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t reg_val_to_write)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

	/* First byte: write command for specified register */
	/* Second byte: number of registers to write (1) */
	/* Third byte: write register value */
	HAL_SPI_Transmit(hspi, (uint8_t[]){ ADS1299_COMMAND_WREG | reg_addr, 0x00, reg_val_to_write }, 3, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

	return ADS1299_STATUS_OK;
}

ads1299_error_t ads1299_send_command(SPI_HandleTypeDef *hspi, uint8_t command)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(hspi, (uint8_t[]){command}, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

	return ADS1299_STATUS_OK;
}

ads1299_error_t ads1299_read_data_IT(SPI_HandleTypeDef *hspi)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(hspi, (uint8_t[]){ADS1299_COMMAND_RDATA}, 1, HAL_MAX_DELAY);
	uint8_t tx_buf[BUFF_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	HAL_SPI_TransmitReceive_IT(hspi, tx_buf, (uint8_t *)RX_BUFF, BUFF_SIZE);

	return ADS1299_STATUS_OK;
}

float ads1299_data_to_float(uint32_t raw_value)
{
	// Check if the 24th bit (MSB) is set (negative number in two's complement)
	int32_t signedValue = (raw_value & 0x800000) ? (0xFF000000 | raw_value) : raw_value;

	// Convert to voltage using the full-scale range and gain
	return signedValue * ADS1299_ONE_LSB;

}

