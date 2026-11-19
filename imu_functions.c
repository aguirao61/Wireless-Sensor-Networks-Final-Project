/***************************************************************************//**
 * @brief
 *    Reads the raw acceleration value and converts to g value based on
 *    the actual resolution
 *
 * @param[out] accel
 *    A 3-element array of float numbers containing the acceleration values
 *    for the x, y and z axes in g units.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_accelDataRead(float *accel)
{
  uint8_t rawData[6];
  float accelRes;
  int16_t temp;

  /* Retrieve the current resolution */
  ICM20648_accelResolutionGet(&accelRes);

  /* Read the six raw data registers into data array */
  ICM20648_registerRead(ICM20648_REG_ACCEL_XOUT_H_SH, 6, &rawData[0]);

  /* Convert the MSB and LSB into a signed 16-bit value and multiply by the resolution to get the G value */
  temp = ( (int16_t) rawData[0] << 8) | rawData[1];
  accel[0] = (float) temp * accelRes;
  temp = ( (int16_t) rawData[2] << 8) | rawData[3];
  accel[1] = (float) temp * accelRes;
  temp = ( (int16_t) rawData[4] << 8) | rawData[5];
  accel[2] = (float) temp * accelRes;

  return ICM20648_OK;
}


/***************************************************************************//**
 * @brief
 *    Reads the raw gyroscope value and converts to deg/sec value based on
 *    the actual resolution
 *
 * @param[out] gyro
 *    A 3-element array of float numbers containing the gyroscope values
 *    for the x, y and z axes in deg/sec units.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_gyroDataRead(float *gyro)
{
  uint8_t rawData[6];
  float gyroRes;
  int16_t temp;

  /* Retrieve the current resolution */
  ICM20648_gyroResolutionGet(&gyroRes);

  /* Read the six raw data registers into data array */
  ICM20648_registerRead(ICM20648_REG_GYRO_XOUT_H_SH, 6, &rawData[0]);

  /* Convert the MSB and LSB into a signed 16-bit value and multiply by the resolution to get the dps value */
  temp = ( (int16_t) rawData[0] << 8) | rawData[1];
  gyro[0] = (float) temp * gyroRes;
  temp = ( (int16_t) rawData[2] << 8) | rawData[3];
  gyro[1] = (float) temp * gyroRes;
  temp = ( (int16_t) rawData[4] << 8) | rawData[5];
  gyro[2] = (float) temp * gyroRes;

  return ICM20648_OK;
}

/***************************************************************************//**
 * @brief
 *    Gets the actual resolution of the accelerometer
 *
 * @param[out] accelRes
 *    The resolution in g/bit units
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_accelResolutionGet(float *accelRes)
{
  uint8_t reg;

  /* Read the actual acceleration full scale setting */
  ICM20648_registerRead(ICM20648_REG_ACCEL_CONFIG, 1, &reg);
  reg &= ICM20648_MASK_ACCEL_FULLSCALE;

  /* Calculate the resolution */
  switch ( reg ) {
    case ICM20648_ACCEL_FULLSCALE_2G:
      *accelRes = 2.0 / 32768.0;
      break;

    case ICM20648_ACCEL_FULLSCALE_4G:
      *accelRes = 4.0 / 32768.0;
      break;

    case ICM20648_ACCEL_FULLSCALE_8G:
      *accelRes = 8.0 / 32768.0;
      break;

    case ICM20648_ACCEL_FULLSCALE_16G:
      *accelRes = 16.0 / 32768.0;
      break;
  }

  return ICM20648_OK;
}

/***************************************************************************//**
 * @brief
 *    Gets the actual resolution of the gyroscope
 *
 * @param[out] gyroRes
 *    The actual resolution in (deg/sec)/bit units
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_gyroResolutionGet(float *gyroRes)
{
  uint8_t reg;

  /* Read the actual gyroscope full scale setting */
  ICM20648_registerRead(ICM20648_REG_GYRO_CONFIG_1, 1, &reg);
  reg &= ICM20648_MASK_GYRO_FULLSCALE;

  /* Calculate the resolution */
  switch ( reg ) {
    case ICM20648_GYRO_FULLSCALE_250DPS:
      *gyroRes = 250.0 / 32768.0;
      break;

    case ICM20648_GYRO_FULLSCALE_500DPS:
      *gyroRes = 500.0 / 32768.0;
      break;

    case ICM20648_GYRO_FULLSCALE_1000DPS:
      *gyroRes = 1000.0 / 32768.0;
      break;

    case ICM20648_GYRO_FULLSCALE_2000DPS:
      *gyroRes = 2000.0 / 32768.0;
      break;
  }

  return ICM20648_OK;
}

/***************************************************************************//**
 * @brief
 *    Sets the full scale value of the accelerometer
 *
 * @param[in] accelFs
 *    The desired full scale value. Use the ICM20648_ACCEL_FULLSCALE_xG
 *    macros, which are defined in the icm20648.h file. The value of x can be
 *    2, 4, 8 or 16.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_accelFullscaleSet(uint8_t accelFs)
{
  uint8_t reg;

  accelFs &= ICM20648_MASK_ACCEL_FULLSCALE;
  ICM20648_registerRead(ICM20648_REG_ACCEL_CONFIG, 1, &reg);
  reg &= ~(ICM20648_MASK_ACCEL_FULLSCALE);
  reg |= accelFs;
  ICM20648_registerWrite(ICM20648_REG_ACCEL_CONFIG, reg);

  return ICM20648_OK;
}

/***************************************************************************//**
 * @brief
 *    Sets the full scale value of the gyroscope
 *
 * @param[in] gyroFs
 *    The desired full scale value. Use the ICM20648_GYRO_FULLSCALE_yDPS
 *    macros, which are defined in the icm20648.h file. The value of y can be
 *    250, 500, 1000 or 2000.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_gyroFullscaleSet(uint8_t gyroFs)
{
  uint8_t reg;

  gyroFs &= ICM20648_MASK_GYRO_FULLSCALE;
  ICM20648_registerRead(ICM20648_REG_GYRO_CONFIG_1, 1, &reg);
  reg &= ~(ICM20648_MASK_GYRO_FULLSCALE);
  reg |= gyroFs;
  ICM20648_registerWrite(ICM20648_REG_GYRO_CONFIG_1, reg);

  return ICM20648_OK;
}


/***************************************************************************//**
 * @brief
 *    Sets the sample rate both of the accelerometer and the gyroscope.
 *
 * @param[in] sampleRate
 *    The desired sample rate in Hz. Since the resolution of the sample rate
 *    divider is different in the accel and gyro stages it is possible that
 *    the two sensor will have different sample rate set.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_sampleRateSet(float sampleRate)
{
  ICM20648_gyroSampleRateSet(sampleRate);
  ICM20648_accelSampleRateSet(sampleRate);

  return ICM20648_OK;
}

/***************************************************************************//**
 * @brief
 *    Sets the sample rate of the accelerometer
 *
 * @param[in] sampleRate
 *    The desired sample rate in Hz
 *
 * @return
 *    The actual sample rate. May be different from the desired value because
 *    of the finite and discrete number of divider settings
 ******************************************************************************/
float ICM20648_gyroSampleRateSet(float sampleRate)
{
  uint8_t gyroDiv;
  float gyroSampleRate;

  /* Calculate the sample rate divider */
  gyroSampleRate = (1125.0 / sampleRate) - 1.0;

  /* Check if it fits in the divider register */
  if ( gyroSampleRate > 255.0 ) {
    gyroSampleRate = 255.0;
  }

  if ( gyroSampleRate < 0 ) {
    gyroSampleRate = 0.0;
  }

  /* Write the value to the register */
  gyroDiv = (uint8_t) gyroSampleRate;
  ICM20648_registerWrite(ICM20648_REG_GYRO_SMPLRT_DIV, gyroDiv);

  /* Calculate the actual sample rate from the divider value */
  gyroSampleRate = 1125.0 / (gyroDiv + 1);

  return gyroSampleRate;
}

/***************************************************************************//**
 * @brief
 *    Sets the sample rate of the gyroscope
 *
 * @param[in] sampleRate
 *    The desired sample rate in Hz
 *
 * @return
 *    The actual sample rate. May be different from the desired value because
 *    of the finite and discrete number of divider settings
 ******************************************************************************/
float ICM20648_accelSampleRateSet(float sampleRate)
{
  uint16_t accelDiv;
  float accelSampleRate;

  /* Calculate the sample rate divider */
  accelSampleRate = (1125.0 / sampleRate) - 1.0;

  /* Check if it fits in the divider registers */
  if ( accelSampleRate > 4095.0 ) {
    accelSampleRate = 4095.0;
  }

  if ( accelSampleRate < 0 ) {
    accelSampleRate = 0.0;
  }

  /* Write the value to the registers */
  accelDiv = (uint16_t) accelSampleRate;
  ICM20648_registerWrite(ICM20648_REG_ACCEL_SMPLRT_DIV_1, (uint8_t) (accelDiv >> 8) );
  ICM20648_registerWrite(ICM20648_REG_ACCEL_SMPLRT_DIV_2, (uint8_t) (accelDiv & 0xFF) );

  /* Calculate the actual sample rate from the divider value */
  accelSampleRate = 1125.0 / (accelDiv + 1);

  return accelSampleRate;
}

/***************************************************************************//**
 * @brief
 *    Sets the bandwidth of the gyroscope
 *
 * @param[in] gyroBw
 *    The desired bandwidth value. Use the ICM20648_GYRO_BW_xHZ macros, which
 *    are defined in the icm20648.h file. The value of x can be
 *    6, 12, 24, 51, 120, 150, 200, 360 or 12100.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_gyroBandwidthSet(uint8_t gyroBw)
{
  uint8_t reg;

  /* Read the GYRO_CONFIG_1 register */
  ICM20648_registerRead(ICM20648_REG_GYRO_CONFIG_1, 1, &reg);
  reg &= ~(ICM20648_MASK_GYRO_BW);

  /* Write the new bandwidth value to the gyro config register */
  reg |= (gyroBw & ICM20648_MASK_GYRO_BW);
  ICM20648_registerWrite(ICM20648_REG_GYRO_CONFIG_1, reg);

  return ICM20648_OK;
}

/***************************************************************************//**
 * @brief
 *    Sets the bandwidth of the accelerometer
 *
 * @param[in] accelBw
 *    The desired bandwidth value. Use the ICM20648_ACCEL_BW_yHZ macros, which
 *    are defined in the icm20648.h file. The value of y can be
 *    6, 12, 24, 50, 111, 246, 470 or 1210.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_accelBandwidthSet(uint8_t accelBw)
{
  uint8_t reg;

  /* Read the GYRO_CONFIG_1 register */
  ICM20648_registerRead(ICM20648_REG_ACCEL_CONFIG, 1, &reg);
  reg &= ~(ICM20648_MASK_ACCEL_BW);

  /* Write the new bandwidth value to the gyro config register */
  reg |= (accelBw & ICM20648_MASK_ACCEL_BW);
  ICM20648_registerWrite(ICM20648_REG_ACCEL_CONFIG, reg);

  return ICM20648_OK;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the cycle mode operation of the accel and gyro
 *
 * @param[in] enable
 *    If true both the accel and gyro sensors will operate in cycle mode. If
 *    false the senors working in continuous mode.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_cycleModeEnable(bool enable)
{
  uint8_t reg;

  reg = 0x00;

  if ( enable ) {
    reg = ICM20648_BIT_ACCEL_CYCLE | ICM20648_BIT_GYRO_CYCLE;
  }

  ICM20648_registerWrite(ICM20648_REG_LP_CONFIG, reg);

  return ICM20648_OK;
}

/***************************************************************************//**
 * @brief
 *    Accelerometer and gyroscope calibration function. Reads the gyroscope
 *    and accelerometer values while the device is at rest and in level. The
 *    resulting values are loaded to the accel and gyro bias registers to cancel
 *    the static offset error.
 *
 * @param[out] accelBiasScaled
 *    The mesured acceleration sensor bias in mg
 *
 * @param[out] gyroBiasScaled
 *    The mesured gyro sensor bias in deg/sec
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_accelGyroCalibrate(float *accelBiasScaled, float *gyroBiasScaled)
{
  uint8_t data[12];
  uint16_t i, packetCount, fifoCount;
  int32_t gyroBias[3] = { 0, 0, 0 };
  int32_t accelBias[3] = { 0, 0, 0 };
  int32_t accelTemp[3];
  int32_t gyroTemp[3];
  int32_t accelBiasFactory[3];
  int32_t gyroBiasStored[3];
  float gyroRes, accelRes;

  /* Enable the accelerometer and the gyro */
  ICM20648_sensorEnable(true, true, false);

  /* Set 1kHz sample rate */
  ICM20648_sampleRateSet(1100.0);

  /* 246Hz BW for the accelerometer and 200Hz for the gyroscope */
  ICM20648_accelBandwidthSet(ICM20648_ACCEL_BW_246HZ);
  ICM20648_gyroBandwidthSet(ICM20648_GYRO_BW_12HZ);

  /* Set the most sensitive range: 2G full scale and 250dps full scale */
  ICM20648_accelFullscaleSet(ICM20648_ACCEL_FULLSCALE_2G);
  ICM20648_gyroFullscaleSet(ICM20648_GYRO_FULLSCALE_250DPS);

  /* Retrieve the resolution per bit */
  ICM20648_accelResolutionGet(&accelRes);
  ICM20648_gyroResolutionGet(&gyroRes);

  /* The accel sensor needs max 30ms, the gyro max 35ms to fully start */
  /* Experiments show that the gyro needs more time to get reliable results */
  UTIL_delay(50);

  /* Disable the FIFO */
  ICM20648_registerWrite(ICM20648_REG_USER_CTRL, ICM20648_BIT_FIFO_EN);
  ICM20648_registerWrite(ICM20648_REG_FIFO_MODE, 0x0F);

  /* Enable accelerometer and gyro to store the data in FIFO */
  ICM20648_registerWrite(ICM20648_REG_FIFO_EN_2, ICM20648_BIT_ACCEL_FIFO_EN | ICM20648_BITS_GYRO_FIFO_EN);

  /* Reset the FIFO */
  ICM20648_registerWrite(ICM20648_REG_FIFO_RST, 0x0F);
  ICM20648_registerWrite(ICM20648_REG_FIFO_RST, 0x00);

  /* Enable the FIFO */
  ICM20648_registerWrite(ICM20648_REG_USER_CTRL, ICM20648_BIT_FIFO_EN);

  /* The max FIFO size is 4096 bytes, one set of measurements takes 12 bytes */
  /* (3 axes, 2 sensors, 2 bytes each value ) 340 samples use 4080 bytes of FIFO */
  /* Loop until at least 4080 samples gathered */
  fifoCount = 0;
  while ( fifoCount < 4080 ) {
    UTIL_delay(5);
    /* Read FIFO sample count */
    ICM20648_registerRead(ICM20648_REG_FIFO_COUNT_H, 2, &data[0]);
    /* Convert to a 16 bit value */
    fifoCount = ( (uint16_t) (data[0] << 8) | data[1]);
  }

  /* Disable accelerometer and gyro to store the data in FIFO */
  ICM20648_registerWrite(ICM20648_REG_FIFO_EN_2, 0x00);

  /* Read FIFO sample count */
  ICM20648_registerRead(ICM20648_REG_FIFO_COUNT_H, 2, &data[0]);

  /* Convert to a 16 bit value */
  fifoCount = ( (uint16_t) (data[0] << 8) | data[1]);

  /* Calculate the number of data sets (3 axis of accel an gyro, two bytes each = 12 bytes) */
  packetCount = fifoCount / 12;

  /* Retrieve the data from the FIFO */
  for ( i = 0; i < packetCount; i++ ) {
    ICM20648_registerRead(ICM20648_REG_FIFO_R_W, 12, &data[0]);
    /* Convert to 16 bit signed accel and gyro x,y and z values */
    accelTemp[0] = ( (int16_t) (data[0] << 8) | data[1]);
    accelTemp[1] = ( (int16_t) (data[2] << 8) | data[3]);
    accelTemp[2] = ( (int16_t) (data[4] << 8) | data[5]);
    gyroTemp[0] = ( (int16_t) (data[6] << 8) | data[7]);
    gyroTemp[1] = ( (int16_t) (data[8] << 8) | data[9]);
    gyroTemp[2] = ( (int16_t) (data[10] << 8) | data[11]);

    /* Sum the values */
    accelBias[0] += accelTemp[0];
    accelBias[1] += accelTemp[1];
    accelBias[2] += accelTemp[2];
    gyroBias[0] += gyroTemp[0];
    gyroBias[1] += gyroTemp[1];
    gyroBias[2] += gyroTemp[2];
  }

  /* Divide by packet count to get the average */
  accelBias[0] /= packetCount;
  accelBias[1] /= packetCount;
  accelBias[2] /= packetCount;
  gyroBias[0] /= packetCount;
  gyroBias[1] /= packetCount;
  gyroBias[2] /= packetCount;

  /* Acceleormeter: add or remove (depending on the orientation of the chip) 1G (gravity) from the Z axis value */
  if ( accelBias[2] > 0L ) {
    accelBias[2] -= (int32_t) (1.0 / accelRes);
  } else {
    accelBias[2] += (int32_t) (1.0 / accelRes);
  }

  /* Convert the values to degrees per sec for displaying */
  gyroBiasScaled[0] = (float) gyroBias[0] * gyroRes;
  gyroBiasScaled[1] = (float) gyroBias[1] * gyroRes;
  gyroBiasScaled[2] = (float) gyroBias[2] * gyroRes;

  /* Read stored gyro trim values. After reset these values are all 0 */
  ICM20648_registerRead(ICM20648_REG_XG_OFFS_USRH, 2, &data[0]);
  gyroBiasStored[0] = ( (int16_t) (data[0] << 8) | data[1]);
  ICM20648_registerRead(ICM20648_REG_YG_OFFS_USRH, 2, &data[0]);
  gyroBiasStored[1] = ( (int16_t) (data[0] << 8) | data[1]);
  ICM20648_registerRead(ICM20648_REG_ZG_OFFS_USRH, 2, &data[0]);
  gyroBiasStored[2] = ( (int16_t) (data[0] << 8) | data[1]);

  /* The gyro bias should be stored in 1000dps full scaled format. We measured in 250dps to get */
  /* the best sensitivity, so need to divide by 4 */
  /* Substract from the stored calibration value */
  gyroBiasStored[0] -= gyroBias[0] / 4;
  gyroBiasStored[1] -= gyroBias[1] / 4;
  gyroBiasStored[2] -= gyroBias[2] / 4;

  /* Split the values into two bytes */
  data[0] = (gyroBiasStored[0] >> 8) & 0xFF;
  data[1] = (gyroBiasStored[0]) & 0xFF;
  data[2] = (gyroBiasStored[1] >> 8) & 0xFF;
  data[3] = (gyroBiasStored[1]) & 0xFF;
  data[4] = (gyroBiasStored[2] >> 8) & 0xFF;
  data[5] = (gyroBiasStored[2]) & 0xFF;

  /* Write the  gyro bias values to the chip */
  ICM20648_registerWrite(ICM20648_REG_XG_OFFS_USRH, data[0]);
  ICM20648_registerWrite(ICM20648_REG_XG_OFFS_USRL, data[1]);
  ICM20648_registerWrite(ICM20648_REG_YG_OFFS_USRH, data[2]);
  ICM20648_registerWrite(ICM20648_REG_YG_OFFS_USRL, data[3]);
  ICM20648_registerWrite(ICM20648_REG_ZG_OFFS_USRH, data[4]);
  ICM20648_registerWrite(ICM20648_REG_ZG_OFFS_USRL, data[5]);

  /* Calculate the accelerometer bias values to store in the hardware accelerometer bias registers. These registers contain */
  /* factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold */
  /* non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature */
  /* compensation calculations(? the datasheet is not clear). Accelerometer bias registers expect bias input */
  /* as 2048 LSB per g, so that the accelerometer biases calculated above must be divided by 8. */

  /* Read factory accelerometer trim values */
  ICM20648_registerRead(ICM20648_REG_XA_OFFSET_H, 2, &data[0]);
  accelBiasFactory[0] = ( (int16_t) (data[0] << 8) | data[1]);
  ICM20648_registerRead(ICM20648_REG_YA_OFFSET_H, 2, &data[0]);
  accelBiasFactory[1] = ( (int16_t) (data[0] << 8) | data[1]);
  ICM20648_registerRead(ICM20648_REG_ZA_OFFSET_H, 2, &data[0]);
  accelBiasFactory[2] = ( (int16_t) (data[0] << 8) | data[1]);

  /* Construct total accelerometer bias, including calculated average accelerometer bias from above */
  /* Scale the 2g full scale (most sensitive range) results to 16g full scale - divide by 8 */
  /* Clear the last bit (temperature compensation? - the datasheet is not clear) */
  /* Substract from the factory calibration value */

  accelBiasFactory[0] -= ( (accelBias[0] / 8) & ~1);
  accelBiasFactory[1] -= ( (accelBias[1] / 8) & ~1);
  accelBiasFactory[2] -= ( (accelBias[2] / 8) & ~1);

  /* Split the values into two bytes */
  data[0] = (accelBiasFactory[0] >> 8) & 0xFF;
  data[1] = (accelBiasFactory[0]) & 0xFF;
  data[2] = (accelBiasFactory[1] >> 8) & 0xFF;
  data[3] = (accelBiasFactory[1]) & 0xFF;
  data[4] = (accelBiasFactory[2] >> 8) & 0xFF;
  data[5] = (accelBiasFactory[2]) & 0xFF;

  /* Store them in the accelerometer offset registers */
  ICM20648_registerWrite(ICM20648_REG_XA_OFFSET_H, data[0]);
  ICM20648_registerWrite(ICM20648_REG_XA_OFFSET_L, data[1]);
  ICM20648_registerWrite(ICM20648_REG_YA_OFFSET_H, data[2]);
  ICM20648_registerWrite(ICM20648_REG_YA_OFFSET_L, data[3]);
  ICM20648_registerWrite(ICM20648_REG_ZA_OFFSET_H, data[4]);
  ICM20648_registerWrite(ICM20648_REG_ZA_OFFSET_L, data[5]);

  /* Convert the values to G for displaying */
  accelBiasScaled[0] = (float) accelBias[0] * accelRes;
  accelBiasScaled[1] = (float) accelBias[1] * accelRes;
  accelBiasScaled[2] = (float) accelBias[2] * accelRes;

  /* Turn off FIFO */
  ICM20648_registerWrite(ICM20648_REG_USER_CTRL, 0x00);

  /* Disable all sensors */
  ICM20648_sensorEnable(false, false, false);

  return ICM20648_OK;
}

/***************************************************************************//**
 * @brief
 *    Gyroscope calibration function. Reads the gyroscope
 *    values while the device is at rest and in level. The
 *    resulting values are loaded to the gyro bias registers to cancel
 *    the static offset error.
 *
 * @param[out] gyroBiasScaled
 *    The mesured gyro sensor bias in deg/sec
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t ICM20648_gyroCalibrate(float *gyroBiasScaled)
{
  uint8_t data[12];
  uint16_t i, packetCount, fifoCount;
  int32_t gyroBias[3] = { 0, 0, 0 };
  int32_t gyroTemp[3];
  int32_t gyroBiasStored[3];
  float gyroRes;

  /* Enable the accelerometer and the gyro */
  ICM20648_sensorEnable(true, true, false);

  /* Set 1kHz sample rate */
  ICM20648_sampleRateSet(1100.0);

  /* Configure bandwidth for gyroscope to 12Hz */
  ICM20648_gyroBandwidthSet(ICM20648_GYRO_BW_12HZ);

  /* Configure sensitivity to 250dps full scale */
  ICM20648_gyroFullscaleSet(ICM20648_GYRO_FULLSCALE_250DPS);

  /* Retrieve the resolution per bit */
  ICM20648_gyroResolutionGet(&gyroRes);

  /* The accel sensor needs max 30ms, the gyro max 35ms to fully start */
  /* Experiments show that the gyro needs more time to get reliable results */
  UTIL_delay(50);

  /* Disable the FIFO */
  ICM20648_registerWrite(ICM20648_REG_USER_CTRL, ICM20648_BIT_FIFO_EN);
  ICM20648_registerWrite(ICM20648_REG_FIFO_MODE, 0x0F);

  /* Enable accelerometer and gyro to store the data in FIFO */
  ICM20648_registerWrite(ICM20648_REG_FIFO_EN_2, ICM20648_BITS_GYRO_FIFO_EN);

  /* Reset the FIFO */
  ICM20648_registerWrite(ICM20648_REG_FIFO_RST, 0x0F);
  ICM20648_registerWrite(ICM20648_REG_FIFO_RST, 0x00);

  /* Enable the FIFO */
  ICM20648_registerWrite(ICM20648_REG_USER_CTRL, ICM20648_BIT_FIFO_EN);

  /* The max FIFO size is 4096 bytes, one set of measurements takes 12 bytes */
  /* (3 axes, 2 sensors, 2 bytes each value ) 340 samples use 4080 bytes of FIFO */
  /* Loop until at least 4080 samples gathered */
  fifoCount = 0;
  while ( fifoCount < 4080 ) {
    UTIL_delay(5);

    /* Read FIFO sample count */
    ICM20648_registerRead(ICM20648_REG_FIFO_COUNT_H, 2, &data[0]);

    /* Convert to a 16 bit value */
    fifoCount = ( (uint16_t) (data[0] << 8) | data[1]);
  }

  /* Disable accelerometer and gyro to store the data in FIFO */
  ICM20648_registerWrite(ICM20648_REG_FIFO_EN_2, 0x00);

  /* Read FIFO sample count */
  ICM20648_registerRead(ICM20648_REG_FIFO_COUNT_H, 2, &data[0]);

  /* Convert to a 16 bit value */
  fifoCount = ( (uint16_t) (data[0] << 8) | data[1]);

  /* Calculate the number of data sets (3 axis of accel an gyro, two bytes each = 12 bytes) */
  packetCount = fifoCount / 12;

  /* Retrieve the data from the FIFO */
  for ( i = 0; i < packetCount; i++ ) {
    ICM20648_registerRead(ICM20648_REG_FIFO_R_W, 12, &data[0]);
    /* Convert to 16 bit signed accel and gyro x,y and z values */
    gyroTemp[0] = ( (int16_t) (data[6] << 8) | data[7]);
    gyroTemp[1] = ( (int16_t) (data[8] << 8) | data[9]);
    gyroTemp[2] = ( (int16_t) (data[10] << 8) | data[11]);

    /* Sum the values */
    gyroBias[0] += gyroTemp[0];
    gyroBias[1] += gyroTemp[1];
    gyroBias[2] += gyroTemp[2];
  }

  /* Divide by packet count to get the average */
  gyroBias[0] /= packetCount;
  gyroBias[1] /= packetCount;
  gyroBias[2] /= packetCount;

  /* Convert the values to degrees per sec for displaying */
  gyroBiasScaled[0] = (float) gyroBias[0] * gyroRes;
  gyroBiasScaled[1] = (float) gyroBias[1] * gyroRes;
  gyroBiasScaled[2] = (float) gyroBias[2] * gyroRes;

  /* Read stored gyro trim values. After reset these values are all 0 */
  ICM20648_registerRead(ICM20648_REG_XG_OFFS_USRH, 2, &data[0]);
  gyroBiasStored[0] = ( (int16_t) (data[0] << 8) | data[1]);

  ICM20648_registerRead(ICM20648_REG_YG_OFFS_USRH, 2, &data[0]);
  gyroBiasStored[1] = ( (int16_t) (data[0] << 8) | data[1]);

  ICM20648_registerRead(ICM20648_REG_ZG_OFFS_USRH, 2, &data[0]);
  gyroBiasStored[2] = ( (int16_t) (data[0] << 8) | data[1]);

  /* The gyro bias should be stored in 1000dps full scaled format. We measured in 250dps to get */
  /* the best sensitivity, so need to divide by 4 */
  /* Substract from the stored calibration value */
  gyroBiasStored[0] -= gyroBias[0] / 4;
  gyroBiasStored[1] -= gyroBias[1] / 4;
  gyroBiasStored[2] -= gyroBias[2] / 4;

  /* Split the values into two bytes */
  data[0] = (gyroBiasStored[0] >> 8) & 0xFF;
  data[1] = (gyroBiasStored[0]) & 0xFF;
  data[2] = (gyroBiasStored[1] >> 8) & 0xFF;
  data[3] = (gyroBiasStored[1]) & 0xFF;
  data[4] = (gyroBiasStored[2] >> 8) & 0xFF;
  data[5] = (gyroBiasStored[2]) & 0xFF;

  /* Write the  gyro bias values to the chip */
  ICM20648_registerWrite(ICM20648_REG_XG_OFFS_USRH, data[0]);
  ICM20648_registerWrite(ICM20648_REG_XG_OFFS_USRL, data[1]);
  ICM20648_registerWrite(ICM20648_REG_YG_OFFS_USRH, data[2]);
  ICM20648_registerWrite(ICM20648_REG_YG_OFFS_USRL, data[3]);
  ICM20648_registerWrite(ICM20648_REG_ZG_OFFS_USRH, data[4]);
  ICM20648_registerWrite(ICM20648_REG_ZG_OFFS_USRL, data[5]);

  /* Turn off FIFO */
  ICM20648_registerWrite(ICM20648_REG_USER_CTRL, 0x00);

  /* Disable all sensors */
  ICM20648_sensorEnable(false, false, false);

  return ICM20648_OK;
}

