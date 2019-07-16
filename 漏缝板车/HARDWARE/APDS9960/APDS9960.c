#include "APDS9960.h"
 /* Members */
 
gesture_data_type gesture_data_;
int gesture_ud_delta_  = 0;
int gesture_lr_delta_  = 0;
int gesture_ud_count_  = 0;
int gesture_lr_count_  = 0;
int gesture_near_count_= 0;
int gesture_far_count_ = 0;
int gesture_state_     = 0;
int gesture_motion_    = DIR_NONE;
volatile bool gesture_update;		//手势更新标志
volatile bool gesture_led_update;		//手势更新标志
apds_data APDS9960;
void handleGesture(void);
/*********************************************************************************************************
** Function name:       APDS_Mode
** Descriptions:        apds模式切换
** input parameters:    无
** output parameters:   无
** Returned value:      true： 切换成功
						false：切换失败
*********************************************************************************************************/
bool APDS_Mode(void)
{
	bool check;	
	switch(APDS9960.flag)
		{
			case APDS_GESTURE:	
			check = APDS9960_EnableProximitySensor(0);
			APDS9960_DisableLightSensor();
			check = APDS9960_EnableGestureSensor(1);//开启手势	
			break;
			case APDS_PROXIMITY:
			check = APDS9960_DisableGestureSensor();	//关闭手势
			check = APDS9960_DisableLightSensor();		//关闭环境光
			check = APDS9960_EnableProximitySensor(1);	break;//开启接近
			case APDS_LIGHT:	
				check = APDS9960_DisableGestureSensor();	//关闭手势
				check = APDS9960_EnableProximitySensor(0);	//关闭接近
				check = APDS9960_EnableLightSensor(1);		break;//开启环境光
			default:
				check = APDS9960_DisableGestureSensor();	//关闭手势
				check = APDS9960_EnableProximitySensor(0);	//关闭接近
				check = APDS9960_DisableLightSensor();		//关闭环境光
				break;
		}
		return check;
}


/*********************************************************************************************************
** Function name:       APDS_SCAN
** Descriptions:        APDS扫描读取数据
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void APDS_SCAN(void)
{
	switch(APDS9960.flag)
	{
		case APDS_GESTURE:if(isr_flag){handleGesture();isr_flag=0;Clear_Fifo();} break;//扫描手势
		case APDS_PROXIMITY:APDS9960_ReadProximity(&APDS9960.proximity);	break;//扫描接近
		case APDS_LIGHT:	APDS9960_ReadAmbientLight(&APDS9960.whitelight); 
							APDS9960_ReadRedLight(&APDS9960.redlight);
							APDS9960_ReadGreenLight(&APDS9960.greenlight);
							APDS9960_ReadBlueLight(&APDS9960.bluelight);	break;//扫描环境光
	}
}

/*********************************************************************************************************
** Function name:       handleGesture
** Descriptions:        手势获取数据处理函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void handleGesture(void)
{ 
	u8  select;
	static u8 Pre_select;
	if( APDS9960_IsGestureAvailable() ){
		
	    select= APDS9960_ReadGesture();
		
		if(Pre_select!=select)
		{
			gesture_update=1;
		}
		Pre_select=select;
		
		gesture_led_update=1;
		
		switch(select){
			case DIR_UP:
				APDS9960.gesture = DIR_UP;
				break;
			case DIR_DOWN:
				APDS9960.gesture = DIR_DOWN;
				break;
			case DIR_LEFT:
				APDS9960.gesture = DIR_LEFT;
				break;
			case DIR_RIGHT:
				APDS9960.gesture = DIR_RIGHT;
				break;
			case DIR_NEAR:
				APDS9960.gesture = DIR_NEAR;
				break;
			case DIR_FAR:
				APDS9960.gesture = DIR_FAR;
				break;
			default:
				APDS9960.gesture = DIR_NONE;
					break;
		}
	}
	
	
}

/*********************************************************************************************************
** Function name:       Infrared_Init
** Descriptions:        红外初始化
** input parameters:    无
** output parameters:   无
** Returned value:      true： 初始化成功
						false：初始化失败
*********************************************************************************************************/
bool Infrared_Init(void)
{
	bool flag = 0;

	flag = APDS9960_Init();								// 红外传感器APDS9960初始化
	
	if(true == flag)									// 判断传感器是否初始化成功
	{
		APDS9960_EXTIX_Init();							// 主芯片引脚外部中断初始化
		return flag;									// 返回true，表示初始化成功
	}
	else{
		NVIC_SystemReset();
	}
	return flag;										// 返回false，表示初始化失败
}

/*clears GFIFO,GINT,GVALID,GFIFO_OV and GFIFO_LVL*/
bool Clear_Fifo(void)
{
	uint8_t clear_value;
	
	if(!APDS9960_ReadDataByte(APDS9960_GCONF4, &clear_value)){
		return false;
	}
	
	clear_value |= (0x01<<2);
	
	if(!APDS9960_WriteDataByte(APDS9960_GCONF4, clear_value)){
		return false;
	}
	
	delay_ms(2);
	
	clear_value &= ~(0x01<<2);
	
	if(!APDS9960_WriteDataByte(APDS9960_GCONF4, clear_value)){
		return false;
	}
	
	delay_ms(2);
	
	return true;
}

/****************************************************************************************************/

/**
 * @brief Determines if there is a gesture available for reading
 *
 * @return True if gesture available. False otherwise.
 */
bool APDS9960_IsGestureAvailable(void)
{
    uint8_t val;
    
    /* Read value from GSTATUS register */
    if( !APDS9960_ReadDataByte(APDS9960_GSTATUS, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out GVALID bit */
    val &= APDS9960_GVALID;
    
    /* Return true/false based on GVALID bit */
    if( val == 1) {
        return true;
    } else {
        return false;
    }
}


/**
 * @brief Configures I2C communications and initializes registers to defaults
 *
 * @return True if initialized successfully. False otherwise.
 */
bool APDS9960_Init(void)
{
    uint8_t id = 0;

    /* Initialize I2C */
    APDS9960_IIC_Init();
     
    /* Read ID register and check against known values for APDS-9960 */
    if( !APDS9960_ReadDataByte(APDS9960_ID, &id) ) {
        return false;
    }
	
//	printf("id:%d\n",id);//打印ID
	
    if( !(id == APDS9960_ID_1 || id == APDS9960_ID_2) ) {
        return false;
    }
     
    /* Set ENABLE register to 0 (disable all features) */
    if( !APDS9960_SetMode(ALL, OFF) ) {
        return false;
    }
    
    /* Set default values for ambient light and proximity registers */
    if( !APDS9960_WriteDataByte(APDS9960_ATIME, DEFAULT_ATIME) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_WTIME, DEFAULT_WTIME) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_PPULSE, DEFAULT_PROX_PPULSE) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_POFFSET_UR, DEFAULT_POFFSET_UR) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_POFFSET_DL, DEFAULT_POFFSET_DL) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_CONFIG1, DEFAULT_CONFIG1) ) {
        return false;
    }
    if( !APDS9960_SetLEDDrive(DEFAULT_LDRIVE) ) {
        return false;
    }
    if( !APDS9960_SetProximityGain(DEFAULT_PGAIN) ) {
        return false;
    }
    if( !APDS9960_SetAmbientLightGain(DEFAULT_AGAIN) ) {
        return false;
    }
    if( !APDS9960_SetProxIntLowThresh(DEFAULT_PILT) ) {
        return false;
    }
    if( !APDS9960_SetProxIntHighThresh(DEFAULT_PIHT) ) {
        return false;
    }
    if( !APDS9960_SetLightIntLowThreshold(DEFAULT_AILT) ) {
        return false;
    }
    if( !APDS9960_SetLightIntHighThreshold(DEFAULT_AIHT) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_PERS, DEFAULT_PERS) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_CONFIG2, DEFAULT_CONFIG2) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_CONFIG3, DEFAULT_CONFIG3) ) {
        return false;
    }
    
    /* Set default values for gesture sense registers */
    if( !APDS9960_SetGestureEnterThresh(DEFAULT_GPENTH) ) {
        return false;
    }
    if( !APDS9960_SetGestureExitThresh(DEFAULT_GEXTH) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_GCONF1, DEFAULT_GCONF1) ) {
        return false;
    }
    if( !APDS9960_SetGestureGain(DEFAULT_GGAIN) ) {
        return false;
    }
    if( !APDS9960_SetGestureLEDDrive(DEFAULT_GLDRIVE) ) {
        return false;
    }
    if( !APDS9960_SetGestureWaitTime(DEFAULT_GWTIME) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_GOFFSET_U, DEFAULT_GOFFSET) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_GOFFSET_D, DEFAULT_GOFFSET) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_GOFFSET_L, DEFAULT_GOFFSET) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_GOFFSET_R, DEFAULT_GOFFSET) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_GPULSE, DEFAULT_GPULSE) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_GCONF3, DEFAULT_GCONF3) ) {
        return false;
    }
    if( !APDS9960_SetGestureIntEnable(DEFAULT_GIEN) ) {
        return false;
    }
    
#if 0
    /* Gesture config register dump */
    uint8_t reg;
    uint8_t val;
  
    for(reg = 0x80; reg <= 0xAF; reg++) {
        if( (reg != 0x82) && \
            (reg != 0x8A) && \
            (reg != 0x91) && \
            (reg != 0xA8) && \
            (reg != 0xAC) && \
            (reg != 0xAD) )
        {
            wireReadDataByte(reg, val);
            Serial.print(reg, HEX);
            Serial.print(": 0x");
            Serial.println(val, HEX);
        }
    }

    for(reg = 0xE4; reg <= 0xE7; reg++) {
        wireReadDataByte(reg, val);
        Serial.print(reg, HEX);
        Serial.print(": 0x");
        Serial.println(val, HEX);
    }
#endif

    return true;
}



/*******************************************************************************
 * Public methods for controlling the APDS-9960
 ******************************************************************************/

/**
 * @brief Reads and returns the contents of the ENABLE register
 *
 * @return Contents of the ENABLE register. 0xFF if error.
 */
uint8_t APDS9960_GetMode(void)
{
    uint8_t enable_value;
    
    /* Read current ENABLE register */
    if( !APDS9960_ReadDataByte(APDS9960_ENABLE, &enable_value) ) {
        return APDS9960_ERROR;
    }
    
    return enable_value;
}


/**
 * @brief Enables or disables a feature in the APDS-9960
 *
 * @param[in] mode which feature to enable
 * @param[in] enable ON (1) or OFF (0)
 * @return True if operation success. False otherwise.
 */
bool APDS9960_SetMode(uint8_t mode, uint8_t enable)
{
    uint8_t reg_val;
	
    /* Read current ENABLE register */
    reg_val = APDS9960_GetMode();
    if( reg_val == APDS9960_ERROR ) {
        return false;
    }
    
    /* Change bit(s) in ENABLE register */
    enable = enable & 0x01;
    if(  mode <= 6 ) {
        if (enable) {
            reg_val |= (1 << mode);
        } else {
            reg_val &= ~(1 << mode);
        }
    } else if( mode == ALL ) {
        if (enable) {
            reg_val = 0x7F;
        } else {
            reg_val = 0x00;
        }
    }
        
    /* Write value back to ENABLE register */
    if( !APDS9960_WriteDataByte(APDS9960_ENABLE, reg_val) ) {
        return false;
    }
        
    return true;
}


/**
 * @brief Starts the light (R/G/B/Ambient) sensor on the APDS-9960
 *
 * @param[in] interrupts true to enable hardware interrupt on high or low light
 * @return True if sensor enabled correctly. False on error.
 */
bool APDS9960_EnableLightSensor(bool interrupts)
{
    
    /* Set default gain, interrupts, enable power, and enable sensor */
    if( !APDS9960_SetAmbientLightGain(DEFAULT_AGAIN) ) {
        return false;
    }
    if( interrupts ) {
        if( !APDS9960_SetAmbientLightIntEnable(1) ) {
            return false;
        }
    } else {
        if( !APDS9960_SetAmbientLightIntEnable(0) ) {
            return false;
        }
    }
    if( !APDS9960_EnablePower() ){
        return false;
    }
    if( !APDS9960_SetMode(AMBIENT_LIGHT, 1) ) {
        return false;
    }
    
    return true;

}


/**
 * @brief Ends the light sensor on the APDS-9960
 *
 * @return True if sensor disabled correctly. False on error.
 */
bool APDS9960_DisableLightSensor(void)
{
    if( !APDS9960_SetAmbientLightIntEnable(0) ) {
        return false;
    }
    if( !APDS9960_SetMode(AMBIENT_LIGHT, 0) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Starts the proximity sensor on the APDS-9960
 *
 * @param[in] interrupts true to enable hardware external interrupt on proximity
 * @return True if sensor enabled correctly. False on error.
 */
bool APDS9960_EnableProximitySensor(bool interrupts)
{
    /* Set default gain, LED, interrupts, enable power, and enable sensor */
    if( !APDS9960_SetProximityGain(DEFAULT_PGAIN) ) {
        return false;
    }
    if( !APDS9960_SetLEDDrive(DEFAULT_LDRIVE) ) {
        return false;
    }
    if( interrupts ) {
        if( !APDS9960_SetProximityIntEnable(1) ) {
            return false;
        }
    } else {
        if( !APDS9960_SetProximityIntEnable(0) ) {
            return false;
        }
    }
    if( !APDS9960_EnablePower() ){
        return false;
    }
    if( !APDS9960_SetMode(PROXIMITY, 1) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Starts the gesture recognition engine on the APDS-9960
 *
 * @param[in] interrupts true to enable hardware external interrupt on gesture
 * @return True if engine enabled correctly. False on error.
 */
bool APDS9960_EnableGestureSensor(bool interrupts)
{
    
    /* Enable gesture mode
       Set ENABLE to 0 (power off)
       Set WTIME to 0xFF
       Set AUX to LED_BOOST_300
       Enable PON, WEN, PEN, GEN in ENABLE 
    */
    APDS9960_ResetGestureParameters();
    if( !APDS9960_WriteDataByte(APDS9960_WTIME, 0xFF) ) {
        return false;
    }
    if( !APDS9960_WriteDataByte(APDS9960_PPULSE, DEFAULT_GESTURE_PPULSE) ) {
        return false;
    }
    if( !APDS9960_SetLEDBoost(LED_BOOST_300) ) {
        return false;
    }
    if( interrupts ) {
        if( !APDS9960_SetGestureIntEnable(1) ) {
            return false;
        }
    } else {
        if( !APDS9960_SetGestureIntEnable(0) ) {
            return false;
        }
    }
    if( !APDS9960_SetGestureMode(1) ) {
        return false;
    }
    if( !APDS9960_EnablePower() ){
        return false;
    }
    if( !APDS9960_SetMode(WAIT, 1) ) {
        return false;
    }
    if( !APDS9960_SetMode(PROXIMITY, 1) ) {
        return false;
    }
    if( !APDS9960_SetMode(GESTURE, 1) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Ends the gesture recognition engine on the APDS-9960
 *
 * @return True if engine disabled correctly. False on error.
 */
bool APDS9960_DisableGestureSensor(void)
{
    APDS9960_ResetGestureParameters();
    if( !APDS9960_SetGestureIntEnable(0) ) {
        return false;
    }
    if( !APDS9960_SetGestureMode(0) ) {
        return false;
    }
    if( !APDS9960_SetMode(GESTURE, 0) ) {
        return false;
    }
    
    return true;
}








int APDS9960_ReadGesture(void)
{
	uint8_t fifo_level = 0;
    int     bytes_read = 0;
	uint8_t fifo_data[128];
    uint8_t gstatus;
    int motion;
//	OS_CPU_SR cpu_sr=0;

	  /* Make sure that power and gesture is on and data is valid */
    if( (!APDS9960_IsGestureAvailable()) ||!(APDS9960_GetMode()&0x41))
	{
        return DIR_NONE;
    }

		while(1)
		{
			/* Wait some time to collect next batch of FIFO data */
      delay_ms(FIFO_PAUSE_TIME);
        
        /* Get the contents of the STATUS register. Is data still valid? */
        if( !APDS9960_ReadDataByte(APDS9960_GSTATUS, &gstatus) ) {
            return APDS9960_ERROR;
        }
				
				/* If we have valid data, read in FIFO */
        if( (gstatus & APDS9960_GVALID) == APDS9960_GVALID ) {
      
         //   OS_ENTER_CRITICAL();	//进入临界区(关闭中断)
			/* Read the current FIFO level */
            if( !APDS9960_ReadDataByte(APDS9960_GFLVL, &fifo_level) ) {
                return APDS9960_ERROR;
            }
						
			if( fifo_level > 0) {
                bytes_read = APDS9960_ReadDataBlock( APDS9960_GFIFO_U, 
                                                (uint8_t*)fifo_data, 
                                                (fifo_level * 4) );
                if( bytes_read == -1 ) {
                    return ERROR;
                }

                /* If at least 1 set of data, sort the data into U/D/L/R */
                if( bytes_read >= 4 ) {
                    for( int i = 0; i < bytes_read; i += 4 ) {
                        gesture_data_.u_data[gesture_data_.index] = \
                                                            fifo_data[i + 0];
                        gesture_data_.d_data[gesture_data_.index] = \
                                                            fifo_data[i + 1];
                        gesture_data_.l_data[gesture_data_.index] = \
                                                            fifo_data[i + 2];
                        gesture_data_.r_data[gesture_data_.index] = \
                                                            fifo_data[i + 3];
                        gesture_data_.index++;
                        gesture_data_.total_gestures++;
                    }
				
							if( APDS9960_ProcessGestureData() ) {
									if( APDS9960_DecodeGesture() ) {
									}
							}
						
							/* Reset data */
							gesture_data_.index = 0;
							gesture_data_.total_gestures = 0;
						}
				    }	
			//		OS_EXIT_CRITICAL();		//退出临界区(开中断)		
				}else{
						/* Determine best guessed gesture and clean up */
            delay_ms(FIFO_PAUSE_TIME);
            APDS9960_DecodeGesture();
            motion = gesture_motion_;
			APDS9960_ResetGestureParameters();
            return motion;
				}
		}
	
}



#if 0
/**
 * @brief Processes a gesture event and returns best guessed gesture
 *
 * @return Number corresponding to gesture. -1 on error.
 */
int APDS9960_ReadGesture(void)
{
    uint8_t fifo_level = 0;
    int     bytes_read = 0;
    uint8_t fifo_data[128];
    uint8_t gstatus;
    int motion;
    int i;
		
    /* Make sure that power and gesture is on and data is valid */
    if( !APDS9960_IsGestureAvailable() || !(APDS9960_GetMode() & 0x41) ) {
			printf("id:%d\n",APDS9960_IsGestureAvailable());
        return DIR_NONE;
    }
    
    /* Keep looping as long as gesture data is valid */
    while(1) {
				//printf("ReadGesture\n");
        /* Wait some time to collect next batch of FIFO data */
        delay_ms(FIFO_PAUSE_TIME);
        
        /* Get the contents of the STATUS register. Is data still valid? */
        if( !APDS9960_ReadDataByte(APDS9960_GSTATUS, &gstatus) ) {
            return APDS9960_ERROR;
        }
        
        /* If we have valid data, read in FIFO */
        if( (gstatus & APDS9960_GVALID) == APDS9960_GVALID ) {
        
            /* Read the current FIFO level */
            if( !APDS9960_ReadDataByte(APDS9960_GFLVL, &fifo_level) ) {
                return APDS9960_ERROR;
            }

#if DEBUG
            Serial.print("FIFO Level: ");
            Serial.println(fifo_level);
#endif

            /* If there's stuff in the FIFO, read it into our data block */
						
            if( fifo_level > 0) {
                bytes_read = APDS9960_ReadDataBlock(  APDS9960_GFIFO_U, 
																										 (uint8_t*)fifo_data, 
																										 (fifo_level * 4) );
                if( bytes_read == -1 ) {
                    return APDS9960_ERROR;
                }
#if DEBUG
                Serial.print("FIFO Dump: ");
                for ( i = 0; i < bytes_read; i++ ) {
                    Serial.print(fifo_data[i]);
                    Serial.print(" ");
                }
                Serial.println();
#endif

                /* If at least 1 set of data, sort the data into U/D/L/R */
                if( bytes_read >= 4 ) {
                    for( i = 0; i < bytes_read; i += 4 ) {
                        gesture_data_.u_data[gesture_data_.index] = \
                                                            fifo_data[i + 0];
                        gesture_data_.d_data[gesture_data_.index] = \
                                                            fifo_data[i + 1];
                        gesture_data_.l_data[gesture_data_.index] = \
                                                            fifo_data[i + 2];
                        gesture_data_.r_data[gesture_data_.index] = \
                                                            fifo_data[i + 3];
                        gesture_data_.index++;
                        gesture_data_.total_gestures++;
                    }
                    
//#if DEBUG
//                Serial.print("Up Data: ");
//                for ( i = 0; i < gesture_data_.total_gestures; i++ ) {
//                    Serial.print(gesture_data_.u_data[i]);
//                    Serial.print(" ");
//                }
//                Serial.println();
//#endif

                    /* Filter and process gesture data. Decode near/far state */
                    if( APDS9960_ProcessGestureData() ) {
                        if( APDS9960_DecodeGesture() ) {
                            //***TODO: U-Turn Gestures
//#if DEBUG
//                            //Serial.println(gesture_motion_);
//#endif
                        }
                    }
                    
                    /* Reset data */
                    gesture_data_.index = 0;
                    gesture_data_.total_gestures = 0;
                }
            }
//							uint8_t clear_value;
//	
//							if(!APDS9960_ReadDataByte(APDS9960_GCONF4, &clear_value)){
//								return false;
//							}
//							
//							clear_value |= (0x01<<2);
//							
//							if(!APDS9960_WriteDataByte(APDS9960_GCONF4, clear_value)){
//								return false;
//							}
//							
//							delay_ms(2);
//							
//							clear_value &= ~(0x01<<2);
//							
//							if(!APDS9960_WriteDataByte(APDS9960_GCONF4, clear_value)){
//								return false;
//							}
        } else {
    
            /* Determine best guessed gesture and clean up */
            delay_ms(FIFO_PAUSE_TIME);
            APDS9960_DecodeGesture();
            motion = gesture_motion_;
#if DEBUG
            Serial.print("END: ");
            Serial.println(gesture_motion_);
#endif
            APDS9960_ResetGestureParameters();
            return motion;
        }
    }
}

#endif

/**
 * Turn the APDS-9960 on
 *
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_EnablePower(void)
{
    if( !APDS9960_SetMode(POWER, 1) ) {
        return false;
    }
    
    return true;
}

/**
 * Turn the APDS-9960 off
 *
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_DisablePower(void)
{
    if( !APDS9960_SetMode(POWER, 0) ) {
        return false;
    }
    
    return true;
}


/*******************************************************************************
 * Ambient light and color sensor controls
 ******************************************************************************/

/**
 * @brief Reads the ambient (clear) light level as a 16-bit value
 *
 * @param[out] val value of the light sensor.
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_ReadAmbientLight(uint16_t *val)
{
    uint8_t val_byte;
    *val = 0;
    
    /* Read value from clear channel, low byte register */
    if( !APDS9960_ReadDataByte(APDS9960_CDATAL, &val_byte) ) {
        return false;
    }
    *val = val_byte;
    
    /* Read value from clear channel, high byte register */
    if( !APDS9960_ReadDataByte(APDS9960_CDATAH, &val_byte) ) {
        return false;
    }
    *val = *val + ((uint16_t)val_byte << 8);
    
    return true;
}


/**
 * @brief Reads the red light level as a 16-bit value
 *
 * @param[out] val value of the light sensor.
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_ReadRedLight(uint16_t *val)
{
    uint8_t val_byte;
    *val = 0;
    
    /* Read value from clear channel, low byte register */
    if( !APDS9960_ReadDataByte(APDS9960_RDATAL, &val_byte) ) {
        return false;
    }
    *val = val_byte;
    
    /* Read value from clear channel, high byte register */
    if( !APDS9960_ReadDataByte(APDS9960_RDATAH, &val_byte) ) {
        return false;
    }
    *val = *val + ((uint16_t)val_byte << 8);
    
    return true;
}


/**
 * @brief Reads the green light level as a 16-bit value
 *
 * @param[out] val value of the light sensor.
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_ReadGreenLight(uint16_t *val)
{
    uint8_t val_byte;
    *val = 0;
    
    /* Read value from clear channel, low byte register */
    if( !APDS9960_ReadDataByte(APDS9960_GDATAL, &val_byte) ) {
        return false;
    }
    *val = val_byte;
    
    /* Read value from clear channel, high byte register */
    if( !APDS9960_ReadDataByte(APDS9960_GDATAH, &val_byte) ) {
        return false;
    }
    *val = *val + ((uint16_t)val_byte << 8);
    
    return true;
}


/**
 * @brief Reads the red light level as a 16-bit value
 *
 * @param[out] val value of the light sensor.
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_ReadBlueLight(uint16_t *val)
{
    uint8_t val_byte;
    *val = 0;
    
    /* Read value from clear channel, low byte register */
    if( !APDS9960_ReadDataByte(APDS9960_BDATAL, &val_byte) ) {
        return false;
    }
    *val = val_byte;
    
    /* Read value from clear channel, high byte register */
    if( !APDS9960_ReadDataByte(APDS9960_BDATAH, &val_byte) ) {
        return false;
    }
    *val = *val + ((uint16_t)val_byte << 8);
    
    return true;
}


/*******************************************************************************
 * Proximity sensor controls
 ******************************************************************************/

/**
 * @brief Reads the proximity level as an 8-bit value
 *
 * @param[out] val value of the proximity sensor.
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_ReadProximity(uint8_t *val)
{
    *val = 0;
    
    /* Read value from proximity data register */
    if( !APDS9960_ReadDataByte(APDS9960_PDATA, val) ) {
        return false;
    }
    
    return true;
}

/*******************************************************************************
 * High-level gesture controls
 ******************************************************************************/

/**
 * @brief Resets all the parameters in the gesture data member
 */
void APDS9960_ResetGestureParameters(void)
{
    gesture_data_.index          = 0;
    gesture_data_.total_gestures = 0;
    
    gesture_ud_delta_            = 0;
    gesture_lr_delta_            = 0;
    
    gesture_ud_count_            = 0;
    gesture_lr_count_            = 0;
    
    gesture_near_count_          = 0;
    gesture_far_count_           = 0;
    
    gesture_state_               = 0;
    gesture_motion_              = DIR_NONE;
}


/**
 * @brief Processes the raw gesture data to determine swipe direction
 *				处理未加工的手势数据去决定滑动的方向
 * @return True if near or far state seen. False otherwise.
 */
bool APDS9960_ProcessGestureData(void)
{
    uint8_t u_first = 0;
    uint8_t d_first = 0;
    uint8_t l_first = 0;
    uint8_t r_first = 0;
	
    uint8_t u_last = 0;
    uint8_t d_last = 0;
    uint8_t l_last = 0;
    uint8_t r_last = 0;
	
    int ud_ratio_first;
    int lr_ratio_first;
    int ud_ratio_last;
    int lr_ratio_last;
    int ud_delta;
    int lr_delta;
    int i;

    /* If we have less than 4 total gestures, that's not enough */
    if( gesture_data_.total_gestures <= 4 ) {
        return false;
    }
    
    /* Check to make sure our data isn't out of bounds */
    if( (gesture_data_.total_gestures <= 32) && \
        (gesture_data_.total_gestures > 0) ) {
        
        /* Find the first value in U/D/L/R above the threshold */
        for( i = 0; i < gesture_data_.total_gestures; i++ ) {
            if( (gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT) ) {
                
                u_first = gesture_data_.u_data[i];
                d_first = gesture_data_.d_data[i];
                l_first = gesture_data_.l_data[i];
                r_first = gesture_data_.r_data[i];
                break;
            }
        }
        
        /* If one of the _first values is 0, then there is no good data */
        if( (u_first == 0) || (d_first == 0) || \
            (l_first == 0) || (r_first == 0) ) {
            
            return false;
        }
        /* Find the last value in U/D/L/R above the threshold */
        for( i = gesture_data_.total_gestures - 1; i >= 0; i-- ) {

            if( (gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT) ) {
                
                u_last = gesture_data_.u_data[i];
                d_last = gesture_data_.d_data[i];
                l_last = gesture_data_.l_data[i];
                r_last = gesture_data_.r_data[i];
                break;
            }
        }
    }
    
    /* Calculate the first vs. last ratio of up/down and left/right */
    ud_ratio_first = ((u_first - d_first) * 100) / (u_first + d_first);
    lr_ratio_first = ((l_first - r_first) * 100) / (l_first + r_first);
    ud_ratio_last = ((u_last - d_last) * 100) / (u_last + d_last);
    lr_ratio_last = ((l_last - r_last) * 100) / (l_last + r_last);
       

       
    /* Determine the difference between the first and last ratios */
    ud_delta = ud_ratio_last - ud_ratio_first;
    lr_delta = lr_ratio_last - lr_ratio_first;
    


    /* Accumulate the UD and LR delta values */
    gesture_ud_delta_ += ud_delta;
    gesture_lr_delta_ += lr_delta;
    
    
    /* Determine U/D gesture */
    if( gesture_ud_delta_ >= GESTURE_SENSITIVITY_1 ) {
        gesture_ud_count_ = 1;
    } else if( gesture_ud_delta_ <= -GESTURE_SENSITIVITY_1 ) {
        gesture_ud_count_ = -1;
    } else {
        gesture_ud_count_ = 0;
    }
    
    /* Determine L/R gesture */
    if( gesture_lr_delta_ >= GESTURE_SENSITIVITY_1 ) {
        gesture_lr_count_ = 1;
    } else if( gesture_lr_delta_ <= -GESTURE_SENSITIVITY_1 ) {
        gesture_lr_count_ = -1;
    } else {
        gesture_lr_count_ = 0;
    }
    
    /* Determine Near/Far gesture */
    if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == 0) ) {
        if( (abs(ud_delta) < GESTURE_SENSITIVITY_2) && \
            (abs(lr_delta) < GESTURE_SENSITIVITY_2) ) {
            
            if( (ud_delta == 0) && (lr_delta == 0) ) {
                gesture_near_count_++;
            } else if( (ud_delta != 0) || (lr_delta != 0) ) {
                gesture_far_count_++;
            }
            
            if( (gesture_near_count_ >= 10) && (gesture_far_count_ >= 2) ) {//原本为10次到2次
                if( (ud_delta == 0) && (lr_delta == 0) ) {
                    gesture_state_ = NEAR_STATE;
                } else if( (ud_delta != 0) && (lr_delta != 0) ) {
                    gesture_state_ = FAR_STATE;
                }
                return true;
            }
        }
    } else {
        if( (abs(ud_delta) < GESTURE_SENSITIVITY_2) && \
            (abs(lr_delta) < GESTURE_SENSITIVITY_2) ) {
                
            if( (ud_delta == 0) && (lr_delta == 0) ) {
                gesture_near_count_++;
            }
            
            if( gesture_near_count_ >= 10 ) {
                gesture_ud_count_ = 0;
                gesture_lr_count_ = 0;
                gesture_ud_delta_ = 0;
                gesture_lr_delta_ = 0;
            }
        }
    }
    
    return false;
}

/**
 * @brief Determines swipe direction or near/far state
 *
 * @return True if near/far event. False otherwise.
 */
bool APDS9960_DecodeGesture(void)
{
    /* Return if near or far event is detected */
    if( gesture_state_ == NEAR_STATE ) {
        gesture_motion_ = DIR_NEAR;
        return true;
    } else if ( gesture_state_ == FAR_STATE ) {
        gesture_motion_ = DIR_FAR;
        return true;
    }
    
    /* Determine swipe direction */
    if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == 0) ) {
        gesture_motion_ = DIR_UP;
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == 0) ) {
        gesture_motion_ = DIR_DOWN;
    } else if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == 1) ) {
        gesture_motion_ = DIR_RIGHT;
    } else if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == -1) ) {
        gesture_motion_ = DIR_LEFT;
    } else if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == 1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_UP;
        } else {
            gesture_motion_ = DIR_RIGHT;
        }
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == -1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_DOWN;
        } else {
            gesture_motion_ = DIR_LEFT;
        }
    } else if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == -1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_UP;
        } else {
            gesture_motion_ = DIR_LEFT;
        }
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == 1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_DOWN;
        } else {
            gesture_motion_ = DIR_RIGHT;
        }
    } else {
        return false;
    }
    
    return true;
}


/*******************************************************************************
 * Getters and setters for register values
 ******************************************************************************/

/**
 * @brief Returns the lower threshold for proximity detection
 *
 * @return lower threshold
 */
uint8_t APDS9960_GetProxIntLowThresh(void)
{
    uint8_t val;
    
    /* Read value from PILT register */
    if( !APDS9960_ReadDataByte(APDS9960_PILT, &val) ) {
        val = 0;
    }
    
    return val;
}


/**
 * @brief Sets the lower threshold for proximity detection
 *
 * @param[in] threshold the lower proximity threshold
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetProxIntLowThresh(uint8_t threshold)
{
    if( !APDS9960_WriteDataByte(APDS9960_PILT, threshold) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Returns the high threshold for proximity detection
 *
 * @return high threshold
 */
uint8_t APDS9960_GetProxIntHighThresh(void)
{
    uint8_t val;
    
    /* Read value from PIHT register */
    if( !APDS9960_ReadDataByte(APDS9960_PIHT, &val) ) {
        val = 0;
    }
    
    return val;
}

/**
 * @brief Sets the high threshold for proximity detection
 *
 * @param[in] threshold the high proximity threshold
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetProxIntHighThresh(uint8_t threshold)
{
    if( !APDS9960_WriteDataByte(APDS9960_PIHT, threshold) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Returns LED drive strength for proximity and ALS
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @return the value of the LED drive strength. 0xFF on failure.
 */
uint8_t APDS9960_GetLEDDrive(void)
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !APDS9960_ReadDataByte(APDS9960_CONTROL, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out LED drive bits */
    val = (val >> 6) & 0x03;
    
    return val;
}


/**
 * @brief Sets the LED drive strength for proximity and ALS
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @param[in] drive the value (0-3) for the LED drive strength
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetLEDDrive(uint8_t drive)
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !APDS9960_ReadDataByte(APDS9960_CONTROL, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    drive &= 0x03;
    drive = drive << 6;
    val &= (~(0x03<<6));
    val |= drive;
    
    /* Write register value back into CONTROL register */
    if( !APDS9960_WriteDataByte(APDS9960_CONTROL, val) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Returns receiver gain for proximity detection
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @return the value of the proximity gain. 0xFF on failure.
 */
uint8_t APDS9960_GetProximityGain(void)
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !APDS9960_ReadDataByte(APDS9960_CONTROL, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out PDRIVE bits */
    val = (val >> 2) & 0x03;
    
    return val;
}


/**
 * @brief Sets the receiver gain for proximity detection
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @param[in] drive the value (0-3) for the gain
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetProximityGain(uint8_t drive)
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !APDS9960_ReadDataByte(APDS9960_CONTROL, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    drive &= 0x03;
    drive = drive << 2;
    val &= (~(0x03<<2));
    val |= drive;
    
    /* Write register value back into CONTROL register */
    if( !APDS9960_WriteDataByte(APDS9960_CONTROL, val) ) {
        return false;
    }
    
    return true;
}

/**
 * @brief Returns receiver gain for the ambient light sensor (ALS)
 *
 * Value    Gain
 *   0        1x
 *   1        4x
 *   2       16x
 *   3       64x
 *
 * @return the value of the ALS gain. 0xFF on failure.
 */
uint8_t APDS9960_GetAmbientLightGain(void)
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !APDS9960_ReadDataByte(APDS9960_CONTROL, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out ADRIVE bits */
    val &= 0x03;
    
    return val;
}


/**
 * @brief Sets the receiver gain for the ambient light sensor (ALS)
 *
 * Value    Gain
 *   0        1x
 *   1        4x
 *   2       16x
 *   3       64x
 *
 * @param[in] drive the value (0-3) for the gain
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetAmbientLightGain(uint8_t drive)
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !APDS9960_ReadDataByte(APDS9960_CONTROL, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    drive &= 0x03;
    val   &= (~(0x03<<0));
    val   |= drive;
    
    /* Write register value back into CONTROL register */
    if( !APDS9960_WriteDataByte(APDS9960_CONTROL, val) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Get the current LED boost value
 * 
 * Value  Boost Current
 *   0        100%
 *   1        150%
 *   2        200%
 *   3        300%
 *
 * @return The LED boost value. 0xFF on failure.
 */
uint8_t APDS9960_GetLEDBoost(void)
{
    uint8_t val;
    
    /* Read value from CONFIG2 register */
    if( !APDS9960_ReadDataByte(APDS9960_CONFIG2, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out LED_BOOST bits */
    val = (val >> 4) & 0x03;
    
    return val;
}


/**
 * @brief Sets the LED current boost value
 *
 * Value  Boost Current
 *   0        100%
 *   1        150%
 *   2        200%
 *   3        300%
 *
 * @param[in] drive the value (0-3) for current boost (100-300%)
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetLEDBoost(uint8_t boost)
{
    uint8_t val;
    
    /* Read value from CONFIG2 register */
    if( !APDS9960_ReadDataByte(APDS9960_CONFIG2, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    boost &= 0x03;
    boost = boost << 4;
    val &= (~(0x03<<4));
    val |= boost;
    
    /* Write register value back into CONFIG2 register */
    if( !APDS9960_WriteDataByte(APDS9960_CONFIG2, val) ) {
        return false;
    }
    
    return true;
}    
   
/**
 * @brief Gets proximity gain compensation enable
 *
 * @return 1 if compensation is enabled. 0 if not. 0xFF on error.
 */
uint8_t APDS9960_GetProxGainCompEnable(void)
{
    uint8_t val;
    
    /* Read value from CONFIG3 register */
    if( !APDS9960_ReadDataByte(APDS9960_CONFIG3, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out PCMP bits */
    val = (val >> 5) & 0x01;
    
    return val;
}


/**
 * @brief Sets the proximity gain compensation enable
 *
 * @param[in] enable 1 to enable compensation. 0 to disable compensation.
 * @return True if operation successful. False otherwise.
 */
 bool APDS9960_SetProxGainCompEnable(uint8_t enable)
{
    uint8_t val;
    
    /* Read value from CONFIG3 register */
    if( !APDS9960_ReadDataByte(APDS9960_CONFIG3, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    enable &= 0x01;
    enable = enable << 5;
    val &= (~(0x01<<5));
    val |= enable;
    
    /* Write register value back into CONFIG3 register */
    if( !APDS9960_WriteDataByte(APDS9960_CONFIG3, val) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets the current mask for enabled/disabled proximity photodiodes
 *
 * 1 = disabled, 0 = enabled
 * Bit    Photodiode
 *  3       UP
 *  2       DOWN
 *  1       LEFT
 *  0       RIGHT
 *
 * @return Current proximity mask for photodiodes. 0xFF on error.
 */
uint8_t APDS9960_GetProxPhotoMask(void)
{
    uint8_t val;
    
    /* Read value from CONFIG3 register */
    if( !APDS9960_ReadDataByte(APDS9960_CONFIG3, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Mask out photodiode enable mask bits */
    val &= 0x0F;
    
    return val;
}


/**
 * @brief Sets the mask for enabling/disabling proximity photodiodes
 *
 * 1 = disabled, 0 = enabled
 * Bit    Photodiode
 *  3       UP
 *  2       DOWN
 *  1       LEFT
 *  0       RIGHT
 *
 * @param[in] mask 4-bit mask value
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetProxPhotoMask(uint8_t mask)
{
    uint8_t val;
    
    /* Read value from CONFIG3 register */
    if( !APDS9960_ReadDataByte(APDS9960_CONFIG3, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    mask &= 0x0F;
    val  &= 0xF0;
    val  |= mask;
    
    /* Write register value back into CONFIG3 register */
    if( !APDS9960_WriteDataByte(APDS9960_CONFIG3, val) ) {
        return false;
    }
    
    return true;
}

/**
 * @brief Gets the entry proximity threshold for gesture sensing
 *
 * @return Current entry proximity threshold.
 */
uint8_t APDS9960_GetGestureEnterThresh(void)
{
    uint8_t val;
    
    /* Read value from GPENTH register */
    if( !APDS9960_ReadDataByte(APDS9960_GPENTH, &val) ) {
        val = 0;
    }
    
    return val;
}


/**
 * @brief Sets the entry proximity threshold for gesture sensing
 *
 * @param[in] threshold proximity value needed to start gesture mode
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetGestureEnterThresh(uint8_t threshold)
{
    if( !APDS9960_WriteDataByte(APDS9960_GPENTH, threshold) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets the exit proximity threshold for gesture sensing
 *
 * @return Current exit proximity threshold.
 */
uint8_t APDS9960_GetGestureExitThresh(void)
{
    uint8_t val;
    
    /* Read value from GEXTH register */
    if( !APDS9960_ReadDataByte(APDS9960_GEXTH, &val) ) {
        val = 0;
    }
    
    return val;
}


/**
 * @brief Sets the exit proximity threshold for gesture sensing
 *
 * @param[in] threshold proximity value needed to end gesture mode
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetGestureExitThresh(uint8_t threshold)
{
    if( !APDS9960_WriteDataByte(APDS9960_GEXTH, threshold) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets the gain of the photodiode during gesture mode
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @return the current photodiode gain. 0xFF on error.
 */
uint8_t APDS9960_GetGestureGain(void)
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF2, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out GGAIN bits */
    val = (val >> 5) & 0x03;
    
    return val;
}


/**
 * @brief Sets the gain of the photodiode during gesture mode
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @param[in] gain the value for the photodiode gain
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetGestureGain(uint8_t gain)
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF2, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    gain &= 0x03;
    gain  = gain << 5;
    val  &= (~(0x03<<5));
    val  |= gain;
    
    /* Write register value back into GCONF2 register */
    if( !APDS9960_WriteDataByte(APDS9960_GCONF2, val) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets the drive current of the LED during gesture mode
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @return the LED drive current value. 0xFF on error.
 */
uint8_t APDS9960_GetGestureLEDDrive(void)
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF2, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out GLDRIVE bits */
    val = (val >> 3) & 0x03;
    
    return val;
}


/**
 * @brief Sets the LED drive current during gesture mode
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @param[in] drive the value for the LED drive current
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetGestureLEDDrive(uint8_t drive)
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF2, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    drive &= 0x03;
    drive = drive << 3;
    val &= (~(0x03<<3));
    val |= drive;
    
    /* Write register value back into GCONF2 register */
    if( !APDS9960_WriteDataByte(APDS9960_GCONF2, val) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets the time in low power mode between gesture detections
 *
 * Value    Wait time
 *   0          0 ms
 *   1          2.8 ms
 *   2          5.6 ms
 *   3          8.4 ms
 *   4         14.0 ms
 *   5         22.4 ms
 *   6         30.8 ms
 *   7         39.2 ms
 *
 * @return the current wait time between gestures. 0xFF on error.
 */
uint8_t APDS9960_GetGestureWaitTime(void)
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF2, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Mask out GWTIME bits */
    val &= 0x07;
    
    return val;
}


/**
 * @brief Sets the time in low power mode between gesture detections
 *
 * Value    Wait time
 *   0          0 ms
 *   1          2.8 ms
 *   2          5.6 ms
 *   3          8.4 ms
 *   4         14.0 ms
 *   5         22.4 ms
 *   6         30.8 ms
 *   7         39.2 ms
 *
 * @param[in] the value for the wait time
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetGestureWaitTime(uint8_t time)
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF2, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    time &= 0x07;
    val  &= 0xF8;
    val  |= time;
    
    /* Write register value back into GCONF2 register */
    if( !APDS9960_WriteDataByte(APDS9960_GCONF2, val) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets the low threshold for ambient light interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_GetLightIntLowThreshold(uint16_t *threshold)
{
    uint8_t val_byte;
    *threshold = 0;
    
    /* Read value from ambient light low threshold, low byte register */
    if( !APDS9960_ReadDataByte(APDS9960_AILTL, &val_byte) ) {
        return false;
    }
    *threshold = val_byte;
    
    /* Read value from ambient light low threshold, high byte register */
    if( !APDS9960_ReadDataByte(APDS9960_AILTH, &val_byte) ) {
        return false;
    }
    *threshold = *threshold + ((uint16_t)val_byte << 8);
    
    return true;
}


/**
 * @brief Sets the low threshold for ambient light interrupts
 *
 * @param[in] threshold low threshold value for interrupt to trigger
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetLightIntLowThreshold(uint16_t threshold)
{
    uint8_t val_low;
    uint8_t val_high;
    
    /* Break 16-bit threshold into 2 8-bit values */
    val_low = threshold & 0x00FF;
    val_high = (threshold & 0xFF00) >> 8;
    
    /* Write low byte */
    if( !APDS9960_WriteDataByte(APDS9960_AILTL, val_low) ) {
        return false;
    }
    
    /* Write high byte */
    if( !APDS9960_WriteDataByte(APDS9960_AILTH, val_high) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets the high threshold for ambient light interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_GetLightIntHighThreshold(uint16_t *threshold)
{
    uint8_t val_byte;
    *threshold = 0;
    
    /* Read value from ambient light high threshold, low byte register */
    if( !APDS9960_ReadDataByte(APDS9960_AIHTL, &val_byte) ) {
        return false;
    }
    *threshold = val_byte;
    
    /* Read value from ambient light high threshold, high byte register */
    if( !APDS9960_ReadDataByte(APDS9960_AIHTH, &val_byte) ) {
        return false;
    }
    *threshold = *threshold + ((uint16_t)val_byte << 8);
    
    return true;
}


/**
 * @brief Sets the high threshold for ambient light interrupts
 *
 * @param[in] threshold high threshold value for interrupt to trigger
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetLightIntHighThreshold(uint16_t threshold)
{
    uint8_t val_low;
    uint8_t val_high;
    
    /* Break 16-bit threshold into 2 8-bit values */
    val_low = threshold & 0x00FF;
    val_high = (threshold & 0xFF00) >> 8;
    
    /* Write low byte */
    if( !APDS9960_WriteDataByte(APDS9960_AIHTL, val_low) ) {
        return false;
    }
    
    /* Write high byte */
    if( !APDS9960_WriteDataByte(APDS9960_AIHTH, val_high) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets the low threshold for proximity interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_GetProximityIntLowThreshold(uint8_t *threshold)
{
    *threshold = 0;
    
    /* Read value from proximity low threshold register */
    if( !APDS9960_ReadDataByte(APDS9960_PILT, threshold) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Sets the low threshold for proximity interrupts
 *
 * @param[in] threshold low threshold value for interrupt to trigger
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetProximityIntLowThreshold(uint8_t threshold)
{
    
    /* Write threshold value to register */
    if( !APDS9960_WriteDataByte(APDS9960_PILT, threshold) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets the high threshold for proximity interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_GetProximityIntHighThreshold(uint8_t *threshold)
{
    *threshold = 0;
    
    /* Read value from proximity high threshold register */
    if( !APDS9960_ReadDataByte(APDS9960_PIHT, threshold) ) {
        return false;
    }
    
    return true;
}

/**
 * @brief Sets the high threshold for proximity interrupts
 *
 * @param[in] threshold high threshold value for interrupt to trigger
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetProximityIntHighThreshold(uint8_t threshold)
{
    
    /* Write threshold value to register */
    if( !APDS9960_WriteDataByte(APDS9960_PIHT, threshold) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets if ambient light interrupts are enabled or not
 *
 * @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
 */
uint8_t APDS9960_GetAmbientLightIntEnable(void)
{
    uint8_t val;
    
    /* Read value from ENABLE register */
    if( !APDS9960_ReadDataByte(APDS9960_ENABLE, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out AIEN bit */
    val = (val >> 4) & 0x01;
    
    return val;
}


/**
 * @brief Turns ambient light interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetAmbientLightIntEnable(uint8_t enable)
{
    uint8_t val;
    
    /* Read value from ENABLE register */
    if( !APDS9960_ReadDataByte(APDS9960_ENABLE, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    enable &= 0x01;
    enable  = enable << 4;
    val    &= ~(0x01<<4);
    val    |= enable;
    
    /* Write register value back into ENABLE register */
    if( !APDS9960_WriteDataByte(APDS9960_ENABLE, val) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets if proximity interrupts are enabled or not
 *
 * @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
 */
uint8_t APDS9960_GetProximityIntEnable(void)
{
    uint8_t val;
    
    /* Read value from ENABLE register */
    if( !APDS9960_ReadDataByte(APDS9960_ENABLE, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out PIEN bit */
    val = (val >> 5) & 0x01;
    
    return val;
}


/**
 * @brief Turns proximity interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetProximityIntEnable(uint8_t enable)
{
    uint8_t val;
    
    /* Read value from ENABLE register */
    if( !APDS9960_ReadDataByte(APDS9960_ENABLE, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    enable &= 0x01;
    enable  = enable << 5;
    val    &= (~(0x01<<5));
    val    |= enable;
    
    /* Write register value back into ENABLE register */
    if( !APDS9960_WriteDataByte(APDS9960_ENABLE, val) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Gets if gesture interrupts are enabled or not
 *
 * @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
 */
uint8_t APDS9960_GetGestureIntEnable(void)
{
    uint8_t val;
    
    /* Read value from GCONF4 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF4, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Shift and mask out GIEN bit */
    val = (val >> 1) & 0x01;
    
    return val;
}



/**
 * @brief Turns gesture-related interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetGestureIntEnable(uint8_t enable)
{
    uint8_t val;
    
    /* Read value from GCONF4 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF4, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    enable &= 0x01;
    enable  = enable << 1;
    val &= (~(0x01<<1));
    val |= enable;
    
    /* Write register value back into GCONF4 register */
    if( !APDS9960_WriteDataByte(APDS9960_GCONF4, val) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Clears the ambient light interrupt
 *
 * @return True if operation completed successfully. False otherwise.
 */
bool APDS9960_ClearAmbientLightInt(void)
{
    uint8_t throwaway;
    if( !APDS9960_ReadDataByte(APDS9960_AICLEAR, &throwaway) ) {
        return false;
    }
    
    return true;
}

/**
 * @brief Clears the proximity interrupt
 *
 * @return True if operation completed successfully. False otherwise.
 */
bool APDS9960_ClearProximityInt(void)
{
    uint8_t throwaway;
    if( !APDS9960_ReadDataByte(APDS9960_PICLEAR, &throwaway) ) {
        return false;
    }
    
    return true;
}


/**
 * @brief Tells if the gesture state machine is currently running
 *
 * @return 1 if gesture state machine is running, 0 if not. 0xFF on error.
 */
uint8_t APDS9960_GetGestureMode(void)
{
    uint8_t val;
    
    /* Read value from GCONF4 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF4, &val) ) {
        return APDS9960_ERROR;
    }
    
    /* Mask out GMODE bit */
    val &= 0x01;
    
    return val;
}


/**
 * @brief Tells the state machine to either enter or exit gesture state machine
 *
 * @param[in] mode 1 to enter gesture state machine, 0 to exit.
 * @return True if operation successful. False otherwise.
 */
bool APDS9960_SetGestureMode(uint8_t mode)
{
    uint8_t val;
    
    /* Read value from GCONF4 register */
    if( !APDS9960_ReadDataByte(APDS9960_GCONF4, &val) ) {
        return false;
    }
    
    /* Set bits in register to given value */
    mode &= 0x01;
    val  &= (~(0x01<<0));
    val  |= mode;
    
    /* Write register value back into GCONF4 register */
    if( !APDS9960_WriteDataByte(APDS9960_GCONF4, val) ) {
        return false;
    }
    
    return true;
}






/*******************************************************************************
 * Raw I2C Reads and Writes
 ******************************************************************************/

/**
 * @brief Writes a single byte to the I2C device (no register)
 *
 * @param[in] val the 1-byte value to write to the I2C device
 * @return True if successful write operation. False otherwise.
 */

bool APDS9960_WriteByte(uint8_t val)
{
			APDS9960_IIC_Start();
			APDS9960_IIC_Send_Byte(APDS9960_I2C_ADDR<<1|write_bit);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			APDS9960_IIC_Send_Byte(val);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			APDS9960_IIC_Stop();
			return true;
}

/**
 * @brief Writes a single byte to the I2C device and specified register
 *
 * @param[in] reg the register in the I2C device to write to
 * @param[in] val the 1-byte value to write to the I2C device
 * @return True if successful write operation. False otherwise.
 */

bool APDS9960_WriteDataByte(uint8_t reg, uint8_t val)
{
			APDS9960_IIC_Start();
			APDS9960_IIC_Send_Byte(APDS9960_I2C_ADDR<<1|write_bit);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			APDS9960_IIC_Send_Byte(reg);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			APDS9960_IIC_Send_Byte(val);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			APDS9960_IIC_Stop();
			return true;
}

/**
 * @brief Writes a block (array) of bytes to the I2C device and register
 *
 * @param[in] reg the register in the I2C device to write to
 * @param[in] val pointer to the beginning of the data byte array
 * @param[in] len the length (in bytes) of the data to write
 * @return True if successful write operation. False otherwise.
 */
bool APDS9960_WriteDataBlock(  uint8_t reg, 
                               uint8_t *val, 
                               unsigned int len)
{
			unsigned int i;
			APDS9960_IIC_Start();
			APDS9960_IIC_Send_Byte(APDS9960_I2C_ADDR<<1|write_bit);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			APDS9960_IIC_Send_Byte(reg);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			for(i=0;i<len;i++)
			{
				APDS9960_IIC_Send_Byte(val[i]);
				if(APDS9960_IIC_Wait_Ack())
				{
					return false;
				}
			}
			APDS9960_IIC_Stop();
			return true;
}

/**
 * @brief Reads a single byte from the I2C device and specified register
 *
 * @param[in] reg the register to read from
 * @param[out] the value returned from the register
 * @return True if successful read operation. False otherwise.
 */
bool APDS9960_ReadDataByte(uint8_t reg, uint8_t *val)
{
			
			APDS9960_IIC_Start();
			APDS9960_IIC_Send_Byte(APDS9960_I2C_ADDR<<1|write_bit);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			APDS9960_IIC_Send_Byte(reg);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			APDS9960_IIC_Start();
			APDS9960_IIC_Send_Byte(APDS9960_I2C_ADDR<<1|read_bit);
			if(APDS9960_IIC_Wait_Ack())
			{
				return false;
			}
			*val = APDS9960_IIC_Read_Byte(send_ack);
			APDS9960_IIC_Stop();
			return true;
}

/**
 * @brief Reads a block (array) of bytes from the I2C device and register
 *
 * @param[in] reg the register to read from
 * @param[out] val pointer to the beginning of the data
 * @param[in] len number of bytes to read
 * @return Number of bytes read. -1 on read error.
 */
int APDS9960_ReadDataBlock(   uint8_t reg, 
                              uint8_t *val, 
                              unsigned int len)
{
	
			unsigned char i = 0;
			APDS9960_IIC_Start();
			APDS9960_IIC_Send_Byte(APDS9960_I2C_ADDR<<1|write_bit);
			if(APDS9960_IIC_Wait_Ack())
			{
				return -1;
			}
			APDS9960_IIC_Send_Byte(reg);
			if(APDS9960_IIC_Wait_Ack())
			{
				return -1;
			}
			APDS9960_IIC_Start();
			APDS9960_IIC_Send_Byte(APDS9960_I2C_ADDR<<1|read_bit);
			if(APDS9960_IIC_Wait_Ack())
			{
				return -1;
			}
			for(i=0;i<len;i++)
			{
				val[i] = APDS9960_IIC_Read_Byte(send_ack);
			}
			APDS9960_IIC_Stop();
			return i;
}










