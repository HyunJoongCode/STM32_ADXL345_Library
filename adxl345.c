/**
 *******************************************************************************
 *
 *  @file        adxl345.c
 *  @author      HyunJoong Kim (Github: Hyunjoongcode)
 *  @date        2025-01-22
 *  @brief       ADXL345 Accelerometer Driver Library (adxl345.c)
 *  @version     1.0 (Initial version)
 *
 *******************************************************************************
 *
 *  @usage
 *  '''c
 *  ADXL_InitType adxlConfig = {
 *      .LP_MODE = LP_NORMAL,
 *      .BWRATE = BWRATE_100,
 *      .LINK_MODE = LINKMODE_OFF,
 *      .AUTOSLEEP_MODE = AUTOSLEEPMODE_OFF,
 *      .MEASURE_SET = MEASURE_ON,
 *      .FULL_RES = FULL_RESOLUTION,
 *      .RANGE = RANGE_4G,
 *      .FIFO_MODE = FIFO_STREAM
 *  };
 *
 *
 *  ADXL_InitType adxlIntConfig = {
 *      .DATA_READY = DATA_READY_OFF,
 *      .SINGLE_TAP = SINGLE_TAP_OFF,
 *      .DOUBLE_TAP = DOUBLE_TAP_OFF,
 *      .ACTIVITY = ACTIVITY_OFF,
 *      .INACTIVITY = INACTIVITY_OFF,
 *      .FREE_FALL = FREE_FALL_OFF,
 *      .WATERMARK = WATERMARK_OFF,
 *      .OVERRUN = OVERRUN_OFF
 *  };
 *
 *  adxlInit(&adxlConfig);
 *
 *  INT_Enable(&adxlIntConfig);
 *  configureTapAndFreefall(&adxlIntConfig);
 *
 *  int16_t x = read_X();
 *  int16_t y = read_Y();
 *  int16_t z = read_Z();
 *  '''
 *
 *  @note
 *   - The data format of ADXL345 is 16-bit Two's Complement.
 *   - If FIFO mode is enabled, the 'FIFO_STATUS' register must be checked.
 *
 *  @version history
 *   - v1.0: Initial version (2025-01-22)
 *
 *******************************************************************************
 */

#include "adxl345.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

/* --------------------------------------------------
 * Global Variables
 * --------------------------------------------------*/
static uint8_t power_ctl = 0;
static uint8_t data_format = 0;
static uint8_t fifo_ctl = 0;
static uint8_t bw_rate = 0;
static uint8_t int_enable = 0;
static uint8_t axis_data[6];
static uint8_t int_source;
static uint8_t act_tap_status = 0;
static uint8_t test;

/* --------------------------------------------------
 * Register Handling Functions
 * --------------------------------------------------*/

/**
 * @brief  Writes a value to a register in ADXL345.
 * @param  reg_address Register address to write to.
 * @param  value The value to be written to the register.
 * @return None
 * @note   This function uses I2C communication.
 *         If the write operation fails, an error message is printed.
 */
void writeRegister(uint8_t reg_address, uint8_t value){
    uint8_t data[2] = {reg_address, value};

    if (HAL_I2C_Master_Transmit(&hi2c1, ADXL_ADDRESS, data, 2, TIMEOUT) != HAL_OK) {
    	printf("Error: Failed to write register 0x%02X\r\n", reg_address);
    }
    else{
    	printf("Success: Written to register: 0x%02X\r\n", reg_address);
    }
}


/**
 * @brief  Reads a value from a register in ADXL345.
 * @param  reg_address: Register address
 * @param  value: Pointer to store read value
 * @param  num: Number of bytes to read
 * @return None
 */
void readRegister(uint8_t reg_address, uint8_t *value, uint8_t num){
	//* I2C_MEMADD_SIZE_8BIT: 8Bits memory size address
    if (HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDRESS, reg_address, I2C_MEMADD_SIZE_8BIT, value, num, TIMEOUT) != HAL_OK) {
    	printf("Error: Failed to read from register 0x%02X\r\n", reg_address);
    }
    else{
    	printf("Success: Read from register: 0x%02X\r\n", reg_address);
    }
}


/**
 * @brief  Reads acceleration data from a register.
 * @param  reg_address: Register address
 * @return None
 */
void readValue(uint8_t reg_address){
	//* I2C_MEMADD_SIZE_8BIT: 8Bits memory size address
    if (HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDRESS, reg_address, I2C_MEMADD_SIZE_8BIT, (uint8_t*)axis_data, 6, TIMEOUT) != HAL_OK) {
    	printf("Error: Failed to read from register 0x%02X\r\n", reg_address);
    }
}


/**
 * @brief  Initializes ADXL345 with user-defined settings.
 * @param  initConfig: Pointer to ADXL_InitType structure
 * @return None
 */
void adxlInit(ADXL_InitType *initConfig){
	if(initConfig == NULL) return;

	resetRegisters();

	/* Set BW_RATE */
	bw_rate = initConfig->LP_MODE | initConfig->BWRATE;
	writeRegister(BW_RATE, bw_rate);


	/* Configure POWER_CTL */
	power_ctl = initConfig->LINK_MODE |
			initConfig->AUTOSLEEP_MODE |
			initConfig->MEASURE_SET;
	/* Optional */
	//Wakeup(WAKEUP_8Hz);

	/* Setting */
	if(initConfig->AUTOSLEEP_MODE == AUTOSLEEPMODE_ON) configureAutosleep();

	writeRegister(POWER_CTL, power_ctl);



	/* Set DATA_FORMAT*/
	data_format = initConfig->FULL_RES |
			initConfig->RANGE;
	/* Optional */
	//Self_Test(SELF_TEST_ON);
	//Int_Invert(INT_ACTIVELOW);
	//Justify(JUSTIFY_MSB);

	writeRegister(DATA_FORMAT, data_format);



	/* Configure FIFO_CTL */
	fifo_ctl = initConfig->FIFO_MODE;
	/* Optional */
	//FIFO_Trigger_bit(FIFO_TRIGGER_INT2);
	//FIFO_Samples(FIFO_SAMPLES_32);

	writeRegister(FIFO_CTL, fifo_ctl);

}


/**
 * @brief  Resets ADXL345 registers to default values.
 * @return None
 */
void resetRegisters(){
	writeRegister(BW_RATE, 0x00);
	writeRegister(POWER_CTL, 0x00);
	writeRegister(DATA_FORMAT, 0x00);
	writeRegister(FIFO_CTL, 0x00);
}


/**
 * @brief  Reads the device ID to verify ADXL345 communication.
 * @return None
 */
void adxlTest(){
	readRegister(DEVID, &test, 1); //*check reading 0xE5(229)
}

/* --------------------------------------------------
 * adxl345.c Initialization Function
 * --------------------------------------------------*/

/**
 * @brief  Configures auto-sleep parameters.
 * @return None
 */
void configureAutosleep(){
	writeRegister(THRESH_ACT, 0x10);    //* THRESH_ACT = 1g
	writeRegister(THRESH_INACT, 0x04);  //* THRESH_INACT = 250mg
	writeRegister(TIME_INACT, 0x05);    //* TIME_INACT = 5sec
	writeRegister(ACT_INACT_CTL, 0XFF); //* ACT_INACT_CTL = X,Y,Z-axis enable
}

/**
 * @brief  Configures power control settings.
 * @param  wakeup: Wake-up mode settings
 * @return None
 */
void WakeUp(uint8_t wakeup){
	power_ctl |= wakeup;
}

/**
 * @brief  Enables self-test mode.
 * @param  self_test: Self-test mode setting
 * @return None
 */
void Self_Test(uint8_t self_test){
	data_format |= self_test;
}

/**
 * @brief  Inverts interrupt polarity.
 * @param  int_invert: Interrupt inversion setting
 * @return None
 */
void Int_Invert(uint8_t int_invert){
	data_format |= int_invert;
}

/**
 * @brief  Configures data justification mode.
 * @param  justify: Justification setting
 * @return None
 */
void Justify(uint8_t justify){
	data_format |= justify;
}

/**
 * @brief  Sets FIFO trigger bit.
 * @param  trigger_bit: FIFO trigger setting
 * @return None
 */
void FIFO_Trigger_bit(uint8_t trigger_bit){
	fifo_ctl |= trigger_bit;
}

/**
 * @brief  Configures FIFO sample size.
 * @param  samples: Number of FIFO samples
 * @return None
 */
void FIFO_Samples(uint8_t samples){
	fifo_ctl |= samples;
}

/**
 * @brief Reads the activity and tap status from the ADXL345 sensor.
 * @return None
 */
void ActTapStatus(){
	readRegister(ACT_TAP_STATUS, &act_tap_status, 1);
}
/* --------------------------------------------------
 * adxl345.c Initialization Function
 * --------------------------------------------------*/

/**
 * @brief  Enables the specified interrupts.
 * @param  INTConfig: Pointer to ADXL_INTType structure
 * @return None
 */
void INT_Enable(ADXL_INTType *INTConfig){
    int_enable = INTConfig->DATA_READY | INTConfig->SINGLE_TAP | INTConfig->DOUBLE_TAP
    		| INTConfig->ACTIVITY | INTConfig->INACTIVITY | INTConfig->FREE_FALL |
			INTConfig->WATERMARK | INTConfig->OVERRUN;

    writeRegister(INT_ENABLE, int_enable);
}

/**
 * @brief  Configures interrupt mapping (assigns interrupt to INT1 or INT2).
 * @param  interrupt_mask: Type of interrupt
 * @param  pin: INT1 (1) or INT2 (2)
 * @return None
 */
void INT_Map(uint8_t interrupt_mask, uint8_t pin){
	uint8_t int_map;

	readRegister(INT_MAP, &int_map, 1);

    if (pin == 1) {
        int_map &= ~interrupt_mask;
    }
    else if (pin == 2) {
        int_map |= interrupt_mask;
    }
    else {
        printf("Error: Invalid pin. Use Pin1 (1) or 2 Pin2 (2).\r\n");
        return;
    }
	writeRegister(INT_MAP, int_map);
}

/**
 * @brief  Reads the interrupt source register and prints detected interrupts.
 * @return None
 */
void INT_Source(void){
	readRegister(INT_SOURCE, &int_source, 1);

    if (int_source & DATA_READY_INT) {
        printf("Interrupt: Data Ready\n");
    }
    if (int_source & SINGLE_TAP_INT) {
        printf("Interrupt: Single Tap Detected\n");
    }
    if (int_source & DOUBLE_TAP_INT) {
        printf("Interrupt: Double Tap Detected\n");
    }
    if (int_source & ACTIVITY_INT) {
        printf("Interrupt: Activity Detected\n");
    }
    if (int_source & INACTIVITY_INT) {
        printf("Interrupt: Inactivity Detected\n");
    }
    if (int_source & FREE_FALL_INT) {
        printf("Interrupt: Free-Fall Detected\n");
    }
    if (int_source & WATERMARK_INT) {
        printf("Interrupt: FIFO Watermark Reached\n");
    }
    if (int_source & OVERRUN_INT) {
        printf("Interrupt: FIFO Overrun\n");
    }
}


/**
 * @brief  Configures Single Tap, Double Tap, and Free-Fall detection settings.
 * @param  INTConfig: Pointer to ADXL_INTType structure
 * @return None
 */
void configureTapAndFreefall(ADXL_INTType *INTConfig){
	/* -------------------------------
	 * Configure Tap (Single/Double)
	 * -------------------------------*/
	if(INTConfig->SINGLE_TAP == SINGLE_TAP_ON || INTConfig->DOUBLE_TAP == DOUBLE_TAP_ON){
		writeRegister(THRESH_TAP, 0x30);      //* Tap threshold
		writeRegister(DUR, 0x20);             //* Tap duration
		writeRegister(TAP_AXES, 0x07);        //* Enable tap detection on X,Y,Z axes

		if(INTConfig->DOUBLE_TAP == DOUBLE_TAP_ON){
			writeRegister(LATENT, 0x05);      //* Delay between taps
			writeRegister(WINDOW, 0x50);      //* Max time between taps
		}
	}

	/* -------------------------------
	 * Configure Free-Fall Detection
	 * -------------------------------*/
	if(INTConfig->FREE_FALL == FREE_FALL_ON){
		writeRegister(THRESH_FF, 0x07);       //* Free-fall threshold (0.44g)
		writeRegister(TIME_FF, 0x08);         //* Free-fall time (50ms)
		writeRegister(ACT_INACT_CTL, 0X77);
	}
}

/* --------------------------------------------------
 * Read Axis Data
 * --------------------------------------------------*/

/**
 * @brief  Reads X-axis acceleration data.
 * @return X-axis acceleration value
 */
int16_t read_X(void){
	readValue(DATAX0);
	return ((axis_data[1] << 8) | axis_data[0]);
}

/**
 * @brief  Reads Y-axis acceleration data.
 * @return Y-axis acceleration value
 */
int16_t read_Y(void){
	readValue(DATAX0);
	return ((axis_data[3] << 8) | axis_data[2]);
}

/**
 * @brief  Reads Z-axis acceleration data.
 * @return Z-axis acceleration value
 */
int16_t read_Z(void){
	readValue(DATAX0);
	return ((axis_data[5] << 8) | axis_data[4]);
}
