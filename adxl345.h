/**
 *******************************************************************************
 *
 *  @file        adxl345.h
 *  @author      HyunJoong Kim (Github: Hyunjoongcode)
 *  @date        2025-01-22
 *  @brief       ADXL345 Accelerometer Driver (adxl345.h)
 *  @version     1.0 (Initial version)
 *
 *******************************************************************************
 *
 *  @details
 *   - ADXL345 3-axis accelerometer driver library
 *   - Supports reading sensor data and configuring settings via I2C or SPI communication
 *   - Includes FIFO mode, interrupt mode, and tap detection functionality
 *
 *  @note
 *   - Default I2C address: 0x53
 *   - Configurable data rate and filter settings
 *   - FIFO mode allows storing multiple samples
 *
 *  @usage
 *   1. Create an 'ADXL_InitType' structure and specify the desired configuration values.
 *   2. Call the 'adxlInit(&Config);' function to initialize the sensor.
 *   3. Use 'read_X()', 'read_Y()', 'read_Z()' functions to retrieve accelerometer data.
 *
 *******************************************************************************
 *
 * @ attention
 *  - The I2C handle must be initialized in 'main.c'.
 *  - If interrupt handling is required, implement 'HAL_GPIO_EXTI_Callback()'.
 *
 *******************************************************************************
 *
 * @license MIT License
 *
 *******************************************************************************
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_
/* --------------------------------------------------
 * adxl345.h
 * --------------------------------------------------*/

#include "main.h"

/* --------------------------------------------------
 * 1. ADXL Init Typedef
 * --------------------------------------------------*/

typedef struct{
	uint8_t LP_MODE;
	uint8_t BWRATE;
	uint8_t LINK_MODE;
	uint8_t AUTOSLEEP_MODE;
	uint8_t MEASURE_SET;
	uint8_t SLEEP_MODE;
	uint8_t FULL_RES;
	uint8_t RANGE;
	uint8_t FIFO_MODE;
} ADXL_InitType;

typedef struct{
	uint8_t DATA_READY;
	uint8_t SINGLE_TAP;
	uint8_t DOUBLE_TAP;
	uint8_t ACTIVITY;
	uint8_t INACTIVITY;
	uint8_t FREE_FALL;
	uint8_t WATERMARK;
	uint8_t OVERRUN;
} ADXL_INTType;


/* --------------------------------------------------
 * 2. register address define
 * --------------------------------------------------*/

#define DEVID 0x00           //*Device ID
#define THRESH_TAP 0x1D      //*Tap threshold
#define OFSX 0x1E            //*X-axis offset
#define OFSY 0x1F            //*Y-axis offset
#define OFSZ 0x20            //*Z-axis offset
#define DUR 0x21             //*Tap duration
#define LATENT 0x22          //*Tap latency
#define WINDOW 0x23          //*Tap window
#define THRESH_ACT 0x24      //*Activity threshold
#define THRESH_INACT 0x25    //*Inactivity threshold
#define TIME_INACT 0x26      //*Inactivity time
#define ACT_INACT_CTL 0x27   //*Axis enable control for activity and inactivity detection
#define THRESH_FF 0x28       //*Free-fall threshold
#define TIME_FF 0x29         //*Free-fall time
#define TAP_AXES 0x2A        //*Axis control for single tap/double tap
#define ACT_TAP_STATUS 0x2B  //*Source of single tap/double tap
#define BW_RATE 0x2C         //*Data rate and power mode control
#define POWER_CTL 0x2D       //*Power-saving features control
#define INT_ENABLE 0x2E      //*Interrupt enable control
#define INT_MAP 0x2F         //*Interrupt mapping control
#define INT_SOURCE 0x30      //*Source of interrupts
#define DATA_FORMAT 0x31     //*Data format control
#define DATAX0 0x32          //*X-Axis Data0
#define DATAX1 0x33          //*X-Axis Data1
#define DATAY0 0x34          //*Y-Axis Data0
#define DATAY1 0x35          //*Y-Axis Data1
#define DATAZ0 0x36          //*Z-Axis Data0
#define DATAZ1 0x37          //*Z-Axis Data1
#define FIFO_CTL 0x38        //*FIFO control
#define FIFO_STATUS 0x39     //*FIFO status


/* --------------------------------------------------
 * 3. register setting value define
 * --------------------------------------------------*/
/** Others **/
#define ADXL_ADDRESS 0x53<<1 //*ADXL345 Slave address
#define TIMEOUT 100

/** 0x2A - TAP_AXES  **/

/** 0x2B - ACT_TAP_STATUS  **/

/** 0x2C - BW_RATE  **/

#define LP_NORMAL 0
#define LP_LOWPOWER 16

#define BWRATE_6_25 7
#define BWRATE_12_5 8
#define BWRATE_25 9
#define BWRATE_50 10
#define BWRATE_100 11
#define BWRATE_200 12
#define BWRATE_400 13
#define BWRATE_800 14
#define BWRATE_1600 15


/** 0x2D - POWER_CTL  **/

#define LINKMODE_ON 32
#define LINKMODE_OFF 0

#define AUTOSLEEPMODE_ON 16
#define AUTOSLEEPMODE_OFF 0

#define MEASURE_ON 8
#define MEASURE_OFF 0

#define SLEEPMODE_ON 4
#define SLEEPMODE_OFF 0

#define WAKEUP_8Hz 0
#define WAKEUP_4Hz 1
#define WAKEUP_2Hz 2
#define WAKEUP_1Hz 3


/** 0x2E - INT_ENABLE  **/

#define DATA_READY_ON 128
#define DATA_READY_OFF 0

#define SINGLE_TAP_ON 64
#define SINGLE_TAP_OFF 0

#define DOUBLE_TAP_ON 32
#define DOUBLE_TAP_OFF 0

#define ACTIVITY_ON 16
#define ACTIVITY_OFF 0

#define INACTIVITY_ON 8
#define INACTIVITY_OFF 0

#define FREE_FALL_ON 4
#define FREE_FALL_OFF 0

#define WATERMARK_ON 2
#define WATERMARK_OFF 0

#define OVERRUN_ON 1
#define OVERRUN_OFF 0


/** 0x2F - INT_MAP  **/

#define DATA_READY_INT 128
#define SINGLE_TAP_INT 64
#define DOUBLE_TAP_INT 32
#define ACTIVITY_INT 16
#define INACTIVITY_INT 8
#define FREE_FALL_INT 4
#define WATERMARK_INT 2
#define OVERRUN_INT 1


/** 0x30 - INT_SOURCE  **/

/** 0x31 - DATA_FORMAT  **/

#define SELF_TEST_ON 128
#define SELF_TEST_OFF 0

#define INT_ACTIVELOW 32
#define INT_ACTIVEHIGH 0

#define FULL_RESOLUTION 8
#define MODE_10BIT 0

#define JUSTIFY_MSB 4
#define JUSTIFY_SIGN 0

#define RANGE_2G 0
#define RANGE_4G 1
#define RANGE_8G 2
#define RANGE_16G 3


/** 0x38 - FIFO_CTL  **/

#define FIFO_BYPASS 0
#define FIFO_FIFO 64
#define FIFO_STREAM 128
#define FIFO_TRIGGER 192

#define FIFO_TRIGGER_INT2 32
#define FIFO_TRIGGER_INT1 0

#define FIFO_SAMPLES_32 31
#define FIFO_SAMPLES_16 15
#define FIFO_SAMPLES_10 9


/** 0x39 - FIFO_STATUS  **/


/* --------------------------------------------------
 * 4. function define
 * --------------------------------------------------*/

void writeRegister(uint8_t reg_address, uint8_t value);
void readRegister(uint8_t reg_address, uint8_t *value, uint8_t num);
void adxlInit(ADXL_InitType *initConfig);
void readAccel(void);
void resetRegisters(void);
void adxlTest(void);

void configureAutosleep(void);
void Self_Test(uint8_t self_test);
void Int_Invert(uint8_t int_invert);
void Justify(uint8_t justify);
void FIFO_Trigger_bit(uint8_t trigger_bit);
void FIFO_Samples(uint8_t samples);
void ActTapStatus();

void INT_Source(void);
void INT_Enable(ADXL_INTType *INTConfig);
void INT_Map(uint8_t interrupt_type, uint8_t pin);

void configureTapAndFreefall(ADXL_INTType *INTConfig);

int16_t read_X(void);
int16_t read_Y(void);
int16_t read_Z(void);

#endif /* INC_ADXL345_H_ */
