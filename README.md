# STM32_ADXL345_Library

# ADXL345 Accelerometer Library
author: Hyunjoong Kim
brief: ADXL345 Accelerometer Driver Library
version: 1.0 (Initial Version)

## Introduction
This is a driver library for the ADXL345 3-axis accelerometer. It supports communication via I2C and provides functions for sensor initialization, data acquisition, and interrupt configuration.

## Features
- Supports I2C communication
- Reads acceleration data for X, Y, and Z axes
- Configures data rate, power mode, and resolution
- Supports FIFO buffer and interrupt handling
- Provides tap and free-fall detection

## Getting Started

### Prerequisites
- STM32 HAL Library (for I2C communication)
- An STM32 development board (e.g., STM32F4)
- ADXL345 module connected via I2C


#### example
ADXL_InitType ADXL_InitStruct = {
	    .LP_MODE = LP_NORMAL,
	    .BWRATE = BWRATE_100,
	    .LINK_MODE = LINKMODE_OFF,
		.MEASURE_SET = MEASURE_ON,
	    .AUTOSLEEP_MODE = AUTOSLEEPMODE_OFF,
	    .FULL_RES = MODE_10BIT,
	    .RANGE = RANGE_2G,
	    .FIFO_MODE = FIFO_BYPASS
};

ADXL_INTType ADXL_INTStruct = {
		.DATA_READY = DATA_READY_OFF,
		.SINGLE_TAP = SINGLE_TAP_OFF,
		.DOUBLE_TAP = DOUBLE_TAP_OFF,
		.ACTIVITY = ACTIVITY_OFF,
		.INACTIVITY = INACTIVITY_OFF,
		.FREE_FALL = FREE_FALL_OFF,
		.WATERMARK = WATERMARK_OFF,
		.OVERRUN = OVERRUN_OFF
};

int16_t x,y,z = 0;


---------------------------------------
main()
		  x = read_X();
		  y = read_Y();
		  z = read_Z();
		  //INT_Source();
		  printf("%d, %d, %d\r\n",x, y, z);
