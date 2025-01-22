# STM32 ADXL345 Library

## ADXL345 Accelerometer Library

**Author:** Hyunjoong Kim  
**Version:** 1.0 (Initial Version)  

### Introduction
This is a driver library for the **ADXL345 3-axis accelerometer**.  
It supports **I2C communication** and provides functions for sensor initialization, data acquisition, and interrupt configuration.

### Features
- **I2C communication support**
- **Reads acceleration data** for X, Y, and Z axes
- Configures **data rate, power mode, and resolution**
- Supports **FIFO buffer and interrupt handling**
- Provides **tap and free-fall detection**

---

## Getting Started

### Prerequisites
- **STM32 HAL Library** (for I2C communication)
- **An STM32 development board** (e.g., STM32F4)
- **ADXL345 module** connected via I2C

### Example Usage

#### **Initialization**
```c
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


Reading Acceleration Data
int16_t x, y, z = 0;

int main() {
    x = read_X();
    y = read_Y();
    z = read_Z();

    printf("%d, %d, %d\r\n", x, y, z);
}


Initialization Function in adxl345.c
Function: void adxlInit(ADXL_InitType *initConfig)
This function initializes the ADXL345 sensor and configures its settings.

void adxlInit(ADXL_InitType *initConfig) {
    if (initConfig == NULL) return;

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

    if (initConfig->AUTOSLEEP_MODE == AUTOSLEEPMODE_ON) configureAutosleep();
    writeRegister(POWER_CTL, power_ctl);

    /* Set DATA_FORMAT */
    data_format = initConfig->FULL_RES | initConfig->RANGE;
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

Optional Features
The following optional functions can be enabled inside adxlInit if needed:

Wake-up Mode: Wakeup(WAKEUP_8Hz);
Auto-Sleep Configuration: configureAutosleep();
Self-Test Mode: Self_Test(SELF_TEST_ON);
Interrupt Polarity Inversion: Int_Invert(INT_ACTIVELOW);
Data Justification (MSB alignment): Justify(JUSTIFY_MSB);
FIFO Trigger Source: FIFO_Trigger_bit(FIFO_TRIGGER_INT2);
FIFO Sample Configuration: FIFO_Samples(FIFO_SAMPLES_32);
These settings allow users to fine-tune the accelerometer’s behavior based on their requirements.


#####License
This project is licensed under the MIT License
