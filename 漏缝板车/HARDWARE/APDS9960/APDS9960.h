#ifndef __APDS9960_H
#define __APDS9960_H
#include "sys.h"



/* APDS模式*/
#define APDS_GESTURE	1
#define APDS_PROXIMITY	2
#define APDS_LIGHT		3

/* APDS-9960 I2C address */
#define APDS9960_I2C_ADDR       0x39

/* Acceptable device IDs */
#define APDS9960_ID_1           0xAB
#define APDS9960_ID_2           0x9C 


/* Gesture parameters */
#define GESTURE_THRESHOLD_OUT   10
#define GESTURE_SENSITIVITY_1   50
#define GESTURE_SENSITIVITY_2   20

/* Error code for returned values */
#define APDS9960_ERROR          0xFF





/* Misc parameters */
#define FIFO_PAUSE_TIME         30      // Wait period (ms) between FIFO reads


/* APDS-9960 register addresses */
#define APDS9960_ENABLE         0x80			//<PON       0x80[0]  >Power ON
																					//<AEN       0x80[1]  >ALS Enable
                                          //<PEN       0x80[2]  >Proximity Enable
																					//<WEN       0x80[3]  >Wait Enable
																					//<AIEN      0x80[4]  >ALS Interrupt Enable
																					//<PIEN      0x80[5]  >Proximity Interrupt Enable 
																					//<GEN       0x80[6]  >Gesture Enable
#define APDS9960_ATIME          0x81			//ALS ADC Integration Time
#define APDS9960_WTIME          0x83			//Wait Time
#define APDS9960_AILTL          0x84			//ALS low threshold, lower byte
#define APDS9960_AILTH          0x85			//ALS low threshold, upper byte
#define APDS9960_AIHTL          0x86			//ALS high threshold, lower byte
#define APDS9960_AIHTH          0x87			//ALS high threshold, upper byte
#define APDS9960_PILT           0x89			//Proximity low threshold
#define APDS9960_PIHT           0x8B			//Proximity high threshold
#define APDS9960_PERS           0x8C			//<PPERS     0x8C[7:4]>Proximity Interrupt Persistence
																					//<APERS		 0x8C[3:0]>ALS Interrupt Persistence
#define APDS9960_CONFIG1        0x8D			//<WLONG     0x8D[1]  >Wait Long Enable
#define APDS9960_PPULSE         0x8E			//<PPLEN     0x8E[7:6]>Proximity Pulse Length
																					//<PPULSE    0x8E[5:0]>Proximity Pulse Count;
#define APDS9960_CONTROL        0x8F			//<AGAIN     0x8F[1:0]>ALS Gain Control
																					//<PGAIN     0x8F[3:2]>Proximity Gain Control
																					//<LDRIVE    0x8F<7:6]>LED Drive Strength
#define APDS9960_CONFIG2        0x90			//<PSIEN     0x90[7]  >Proximity Saturation Interrupt Enable
																					//<CPSIEN    0x90[6]  >Clear diode Saturation Interrupt Enable
																					//<LEDBOOST  0x90[5:4]>Proximity/Gesture LED Boost																					
#define APDS9960_ID             0x92			//
#define APDS9960_STATUS         0x93			//<CPSAT     0x93[7]  >Clear Diode Saturation
																					//<PGSAT     0x93[6]  >Proximity Saturation
																					//<PGSAT		 0x93[6]  >Gesture Saturation
																					//<PINT      0x93[5]  >Proximity Interrupt
																					//<AINT      0x93[4]  >ALS Interrupt
																					//<PVALID    0x93[1]  >Proximity Valid
																					//<AVALID    0x93[0]  >ALS Valid
#define APDS9960_CDATAL         0x94			//Clear Data, Low byte
#define APDS9960_CDATAH         0x95			//Clear Data, High byte
#define APDS9960_RDATAL         0x96			//Red Data, Low byte
#define APDS9960_RDATAH         0x97			//Red Data, High byte
#define APDS9960_GDATAL         0x98			//Green Data, Low byte
#define APDS9960_GDATAH         0x99			//Green Data, High byte
#define APDS9960_BDATAL         0x9A			//Blue Data, Low byte
#define APDS9960_BDATAH         0x9B			//Blue Data, High byte
#define APDS9960_PDATA          0x9C			//Proximity Data
#define APDS9960_POFFSET_UR     0x9D			//Proximity Offset UP/RIGHT
#define APDS9960_POFFSET_DL     0x9E			//Proximity Offset DOWN/LEFT
#define APDS9960_CONFIG3        0x9F			//<PCMP      0x9F[5]  >Proximity Gain Compensation Enable
																					//<PMSK_U    0x9F[3]  >Proximity Mask UP Enable
																					//<PMSK_D    0x9F[2]  >Proximity Mask DOWN Enable
																					//<PMSK_L    0x9F[1]  >Proximity Mask LEFT Enable
																					//<PMSK_R    0x9F[0]  >Proximity Mask RIGHT Enable
#define APDS9960_GPENTH         0xA0			//Gesture Proximity Entry Threshold
#define APDS9960_GEXTH          0xA1			//Gesture Exit Threshold
#define APDS9960_GCONF1         0xA2			//<GFIFOTH   0xA2[7:6]>Gesture FIFO Threshold
																					//<GEXMSK    0xA2[5:2]>Gesture Exit Mask
																					//<GEXPERS	 0xA2[1:0]>Gesture Exit Persistence
#define APDS9960_GCONF2         0xA3			//<GGAIN		 0xA3[6:5]>Gesture Gain Control
																					//<GLDRIVE	 0xA3[4:3]>Gesture LED Drive Strength
																					//<GWTIME		 0xA3[2:0]>Gesture Wait Time
#define APDS9960_GOFFSET_U      0xA4			//Gesture Offset, UP
#define APDS9960_GOFFSET_D      0xA5			//Gesture Offset, DOWN
#define APDS9960_GOFFSET_L      0xA7			//Gesture Offset, LEFT
#define APDS9960_GOFFSET_R      0xA9			//Gesture Offset, RIGHT
#define APDS9960_GPULSE         0xA6			//<GPULSE    0xA6[5:0]>Pulse Count
																					//<GPLEN     0xA6[7:6]>Gesture Pulse Length	
#define APDS9960_GCONF3         0xAA			//<GDIMS     0xAA[1:0]>Gesture Dimension Select
#define APDS9960_GCONF4         0xAB			//<GIEN      0xAB[1]  >Gesture Interrupt Enable
																					//<GMODE     0xAB[0]  >Gesture Mode
#define APDS9960_GFLVL          0xAE			//Gesture FIFO Level
#define APDS9960_GSTATUS        0xAF			//<GFOV      0xAF[1]  >Gesture FIFO Overflow
																					//<GVALID    0xAF[0]  >Gesture Valid
#define APDS9960_IFORCE         0xE4			//
#define APDS9960_PICLEAR        0xE5			//Proximity Interrupt Clear
#define APDS9960_CICLEAR        0xE6			//ALS clear channel interrupt clear
#define APDS9960_AICLEAR        0xE7			//All Non-Gesture Interrupt Clear
#define APDS9960_GFIFO_U        0xFC			//Gesture FIFO Data, UP
#define APDS9960_GFIFO_D        0xFD			//Gesture FIFO Data, DOWN
#define APDS9960_GFIFO_L        0xFE			//Gesture FIFO Data, LEFT
#define APDS9960_GFIFO_R        0xFF			//Gesture FIFO Data, RIGHT


/* Bit fields */
#define APDS9960_PON            0x01
#define APDS9960_AEN            0x02
#define APDS9960_PEN            0x04
#define APDS9960_WEN            0x08
#define APSD9960_AIEN           0x10
#define APDS9960_PIEN           0x20
#define APDS9960_GEN            0x40
#define APDS9960_GVALID         0x01

/* On/Off definitions */
#define OFF                     0
#define ON                      1


/* Acceptable parameters for setMode */
#define POWER                   0
#define AMBIENT_LIGHT           1
#define PROXIMITY               2
#define WAIT                    3
#define AMBIENT_LIGHT_INT       4
#define PROXIMITY_INT           5
#define GESTURE                 6
#define ALL                     7

/* LED Drive values */
#define LED_DRIVE_100MA         0
#define LED_DRIVE_50MA          1
#define LED_DRIVE_25MA          2
#define LED_DRIVE_12_5MA        3

/* Proximity Gain (PGAIN) values */
#define PGAIN_1X                0
#define PGAIN_2X                1
#define PGAIN_4X                2
#define PGAIN_8X                3

/* ALS Gain (AGAIN) values */
#define AGAIN_1X                0
#define AGAIN_4X                1
#define AGAIN_16X               2
#define AGAIN_64X               3

/* Gesture Gain (GGAIN) values */
#define GGAIN_1X                0
#define GGAIN_2X                1
#define GGAIN_4X                2
#define GGAIN_8X                3

/* LED Boost values */
#define LED_BOOST_100           0
#define LED_BOOST_150           1
#define LED_BOOST_200           2
#define LED_BOOST_300           3    

/* Gesture wait time values */
#define GWTIME_0MS              0
#define GWTIME_2_8MS            1
#define GWTIME_5_6MS            2
#define GWTIME_8_4MS            3
#define GWTIME_14_0MS           4
#define GWTIME_22_4MS           5
#define GWTIME_30_8MS           6
#define GWTIME_39_2MS           7

/* Default values */
#define DEFAULT_ATIME           219     // 103ms
#define DEFAULT_WTIME           246     // 27ms
#define DEFAULT_PROX_PPULSE     0x87    // 16us, 8 pulses
#define DEFAULT_GESTURE_PPULSE  0x89    // 16us, 10 pulses
#define DEFAULT_POFFSET_UR      0       // 0 offset
#define DEFAULT_POFFSET_DL      0       // 0 offset      
#define DEFAULT_CONFIG1         0x60    // No 12x wait (WTIME) factor
#define DEFAULT_LDRIVE          LED_DRIVE_100MA
#define DEFAULT_PGAIN           PGAIN_4X
#define DEFAULT_AGAIN           AGAIN_4X
#define DEFAULT_PILT            0       // Low proximity threshold
#define DEFAULT_PIHT            50      // High proximity threshold
#define DEFAULT_AILT            0xFFFF  // Force interrupt for calibration
#define DEFAULT_AIHT            0
#define DEFAULT_PERS            0x11    // 2 consecutive prox or ALS for int.
#define DEFAULT_CONFIG2         0x01    // No saturation interrupts or LED boost  
#define DEFAULT_CONFIG3         0       // Enable all photodiodes, no SAI
#define DEFAULT_GPENTH          40      // Threshold for entering gesture mode
#define DEFAULT_GEXTH           30      // Threshold for exiting gesture mode    
#define DEFAULT_GCONF1          0x40    // 4 gesture events for int., 1 for exit
#define DEFAULT_GGAIN           GGAIN_4X
#define DEFAULT_GLDRIVE         LED_DRIVE_100MA
#define DEFAULT_GWTIME          GWTIME_2_8MS
#define DEFAULT_GOFFSET         0       // No offset scaling for gesture mode
#define DEFAULT_GPULSE          0xC9    // 32us, 10 pulses
#define DEFAULT_GCONF3          0       // All photodiodes active during gesture
#define DEFAULT_GIEN            0       // Disable gesture interrupts



/* Direction definitions */
enum {
  DIR_NONE,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_UP,
  DIR_DOWN,
  DIR_NEAR,
  DIR_FAR,
  DIR_ALL
};


/* State definitions */
enum {
  NA_STATE,
  NEAR_STATE,
  FAR_STATE,
  ALL_STATE
};

/* Container for gesture data */
typedef struct gesture_data_type {
    uint8_t u_data[32];
    uint8_t d_data[32];
    uint8_t l_data[32];
    uint8_t r_data[32];
    uint8_t index;
    uint8_t total_gestures;
    uint8_t in_threshold;
    uint8_t out_threshold;
} gesture_data_type;


//apds传感变量
typedef struct apds_data{
	char flag;		 //APDS当前所处模式变量
	char gesture;	 //手势传感——手势变量
	 u8  proximity;  //接近传感——接近变量
	u16  whitelight; //感光传感——感光变量
	u16  redlight;	 //颜色传感——红光变量
	u16  greenlight; //颜色传感——绿光变量
	u16  bluelight;  //颜色传感——蓝光变量
}apds_data;


extern apds_data APDS9960;
extern volatile bool  gesture_update;
extern volatile bool gesture_led_update;
bool APDS9960_Init(void);

/*******************************************************************************
 * Public methods for controlling the APDS-9960
 ******************************************************************************/

uint8_t APDS9960_GetMode(void);

bool APDS9960_SetMode(uint8_t mode, uint8_t enable);

bool APDS9960_EnableLightSensor(bool interrupts);

bool APDS9960_DisableLightSensor(void);

bool APDS9960_EnableProximitySensor(bool interrupts);

bool APDS9960_EnableGestureSensor(bool interrupts);

bool APDS9960_DisableGestureSensor(void);

bool APDS9960_IsGestureAvailable(void);

int APDS9960_ReadGesture(void);

bool APDS9960_EnablePower(void);

bool APDS9960_DisablePower(void);

/*******************************************************************************
 * Ambient light and color sensor controls
 ******************************************************************************/

bool APDS9960_ReadAmbientLight(uint16_t *val);

bool APDS9960_ReadRedLight(uint16_t *val);

bool APDS9960_ReadGreenLight(uint16_t *val);

bool APDS9960_ReadBlueLight(uint16_t *val);

/*******************************************************************************
 * Proximity sensor controls
 ******************************************************************************/

bool APDS9960_ReadProximity(uint8_t *val);

/*******************************************************************************
 * High-level gesture controls
 ******************************************************************************/

void APDS9960_ResetGestureParameters(void);

bool APDS9960_ProcessGestureData(void);

bool APDS9960_DecodeGesture(void);

/*******************************************************************************
 * Getters and setters for register values
 ******************************************************************************/

uint8_t APDS9960_GetProxIntLowThresh(void);

bool APDS9960_SetProxIntLowThresh(uint8_t threshold);

uint8_t APDS9960_GetProxIntHighThresh(void);

bool APDS9960_SetProxIntHighThresh(uint8_t threshold);

uint8_t APDS9960_GetLEDDrive(void);

bool APDS9960_SetLEDDrive(uint8_t drive);

uint8_t APDS9960_GetProximityGain(void);

bool APDS9960_SetProximityGain(uint8_t drive);

uint8_t APDS9960_GetAmbientLightGain(void);

bool APDS9960_SetAmbientLightGain(uint8_t drive);

uint8_t APDS9960_GetLEDBoost(void);

bool APDS9960_SetLEDBoost(uint8_t boost);

uint8_t APDS9960_GetProxGainCompEnable(void);

 bool APDS9960_SetProxGainCompEnable(uint8_t enable);
 
 uint8_t APDS9960_GetProxPhotoMask(void);
 
 bool APDS9960_SetProxPhotoMask(uint8_t mask);
 
 uint8_t APDS9960_GetGestureEnterThresh(void);
 
 bool APDS9960_SetGestureEnterThresh(uint8_t threshold);
 
 uint8_t APDS9960_GetGestureExitThresh(void);
 
 bool APDS9960_SetGestureExitThresh(uint8_t threshold);
 
 uint8_t APDS9960_GetGestureGain(void);
 
 bool APDS9960_SetGestureGain(uint8_t gain);
 
 uint8_t APDS9960_GetGestureLEDDrive(void);
 
 bool APDS9960_SetGestureLEDDrive(uint8_t drive);
 
 uint8_t APDS9960_GetGestureWaitTime(void);
 
 bool APDS9960_SetGestureWaitTime(uint8_t time);
 
 bool APDS9960_GetLightIntLowThreshold(uint16_t *threshold);
 
 bool APDS9960_SetLightIntLowThreshold(uint16_t threshold);
 
 bool APDS9960_GetLightIntHighThreshold(uint16_t *threshold);
 
 bool APDS9960_SetLightIntHighThreshold(uint16_t threshold);
 
 bool APDS9960_GetProximityIntLowThreshold(uint8_t *threshold);
 
 bool APDS9960_SetProximityIntLowThreshold(uint8_t threshold);
 
 bool APDS9960_GetProximityIntHighThreshold(uint8_t *threshold);
 
 bool APDS9960_SetProximityIntHighThreshold(uint8_t threshold);
 
 uint8_t APDS9960_GetAmbientLightIntEnable(void);
 
 bool APDS9960_SetAmbientLightIntEnable(uint8_t enable);
 
 uint8_t APDS9960_GetProximityIntEnable(void);
 
 bool APDS9960_SetProximityIntEnable(uint8_t enable);
 
 uint8_t APDS9960_GetGestureIntEnable(void);
 
 bool APDS9960_SetGestureIntEnable(uint8_t enable);
 
 bool APDS9960_ClearAmbientLightInt(void);
 
 bool APDS9960_ClearProximityInt(void);
 
 uint8_t APDS9960_GetGestureMode(void);
 
 bool APDS9960_SetGestureMode(uint8_t mode);


/*******************************************************************************
 * Raw I2C Reads and Writes
 ******************************************************************************/

bool APDS9960_WriteByte(uint8_t val);

bool APDS9960_WriteDataByte(uint8_t reg, uint8_t val);

bool APDS9960_WriteDataBlock(uint8_t reg, uint8_t *val, unsigned int len);

bool APDS9960_ReadDataByte(uint8_t reg, uint8_t *val);

int APDS9960_ReadDataBlock(uint8_t reg, uint8_t *val, unsigned int len);


/*******************************************************************************
 * ?????????????????????
 ******************************************************************************/
bool Clear_Fifo(void);
void APDS_SCAN(void);
bool APDS_Mode(void);
bool Infrared_Init(void);
#endif

