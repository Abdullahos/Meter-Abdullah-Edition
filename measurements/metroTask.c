
/*******************************************************************************
* INCLUDE FILES:
*******************************************************************************/
#include "metroTask.h"
#include "metrology.h"
#include <stdint.h>
#include "pinout_definitions.h"

/** @addtogroup LEGAL
  * @{
  */

/*******************************************************************************
* CONSTANTS & MACROS:
*******************************************************************************/
extern METRO_Device_Config_t  p_Metro_Device_Config;

#define FACTOR_POWER_ON_ENERGY      (858)   // (3600 * 16000000 / 0x4000000) = 858.3...


       /*+------------------------------------------------------------------------------------+
         |                                        U32                                         |
         |---------------------|-------------------|-------------------|----------------------|
         |     STPM EXT4       |     STPM EXT3     |     STPM EXT2     |     STPM EXT1        |
         |---------------------|-------------------|-------------------|----------------------|
         |    u4   |     u4    |   u4    |   u4    |     u4  |     u4  |      u4   |  u4      |
         |---------|-----------|--------------------------------------------------------------|
         |CH masks | STPM type |CH masks |STPM type|CH masks |STPM type|  CH masks |STPM type |
         |---------|-----------|--------------------------------------------------------------|

        STPM CFG EXTx (u8):
        -----------------
        MSB u4 : Channel  Mask :  Channels affected to STPM
            0 : No Channel affected
            1 : Channel 1 affected
            2 : Channel 2 affected
            4 : Channel 3 affected
            8 : Channel 4 affected

        LSB u4 :  STPM type : 6 to 8
            0 : No STPM
            6 : STPM32
            7 : STPM33
            8 : STPM34

        EX : STPM EXT 1: One STPM34 with Channels 2 and 3 affected on it
        LSB u4 = 8 (STPM34)
        MSB u4 = 6 ( 4:Channel 3 + 2:Channel 2)

        STPM CONFIG : U32 = 0x00000068

        +------------------------------------------------------------------------------------+*/

const nvmLeg_t metroDefault = {
    0x00000017 // config
  ,
  {                 // data1[19] STPM (Config for CT)
	  0x040000A0,
	  0x242000A0,
	  0x000004E0,
	  0x00000000,
	  0x003FF800,
	  0x003FF913,
	  0x003FF6F0,
	  0x003FF800,
	  0x00000FFF,
	  0x00000FFF,
	  0x00000FFF,
	  0x00000FFF,
	  0x03270327,
	  0x03270327,
	  0x00000000,
	  0x00000000,
	  0x0207777F,
	  0x02007073,
	  0x00004007,
  },
  {                // powerFact[2]
    36704170,      // ch 1
	  36704170       // ch 2
  },
  {                // voltageFact[2]
    116274,        // ch 1
    116274         // ch 2
  },
  {                // currentFact[2]
    31388,         // ch 1
    25934          // ch 2
  }
};
 

const STPM_Com_port_t STPM_com_port ={  
 #ifdef UART_HAL
  .uart = USART2,    //USART used by device 1
#endif
#ifdef SPI_HAL
//.spi = spi0,
 // .spi = SPI1,    //spi instance
#endif
 // .cs_port = STPM33_CS_GPIO_Port,     //CS used by device 1
  .cs_pin = STPM33_CS_Pin,
  //.syn_port = STPM33_SYN_GPIO_Port,     //SYN used by device 1
  .syn_pin = STPM33_SYN_Pin,
  //.en_port = STPM33_EN_GPIO_Port,     //EN used by device 1
  .en_pin = STPM33_EN_Pin
};



/*******************************************************************************
* TYPES:
*******************************************************************************/

/*******************************************************************************
* GLOBAL VARIABLES:
*******************************************************************************/
metroData_t metroData;
//METRO_Device_Config_t Tab_METRO_Global_Devices_Config[NB_MAX_DEVICE];

extern METRO_Device_Config_t p_Metro_Device_Config;

/*******************************************************************************
* LOCAL FUNCTION PROTOTYPES:
*******************************************************************************/
static void METRO_UpdateData(void);
static void Metro_com_port_device(void);

/*******************************************************************************
* LOCAL VARIABLES:
*******************************************************************************/

/*******************************************************************************
*
*                       IMPLEMENTATION: Public functions
*
*******************************************************************************/


/*******************************************************************************
*
*                       IMPLEMENTATION: Private functions
*
*******************************************************************************/

/**
  * @brief  This function implements the Metrology init
  * @param  None
  * @retval None
  */
void METRO_Init()
{
  /* initialization device type and number of channel */
  Metro_Setup((uint32_t)metroDefault.config); 
  
  /* initialization device communication port */ 
  Metro_com_port_device();

  /*power STPM properly with EN pin to set it in UART or SPI mode*/
  Metro_power_up_device();
  
  /* initialization steps for STPM device */
  Metro_Init();
  
  /* Change UART speed for STPM communication between Host and EXT1*/
#ifdef UART_HAL
  Metro_UartSpeed(USART_SPEED); 
#endif
  
  /* Write configuration to STPM device and read back configuration from STPM device */   
  Metro_ApplyConfig((uint32_t)metroDefault.config,(uint32_t)metroDefault.data1);

/* Initialize the factors for the computation */
  for(int i=0;i<NB_MAX_CHANNEL;i++)
  {
  Metro_Set_Hardware_Factors( (METRO_Channel_t)(CHANNEL_1+i), (uint32_t)metroDefault.powerFact[i], (uint32_t)metroDefault.powerFact[i]/ FACTOR_POWER_ON_ENERGY,(uint32_t)metroDefault.voltageFact[i],(uint32_t)metroDefault.currentFact[i]);
  }
   
  if(p_Metro_Device_Config.device != 0)
  {
    /* Set default latch device type inside Metro struct for Ext chips */
     Metro_Register_Latch_device_Config_type( LATCH_SYN_SCS);
  }
}


/**
  * @brief  This function implements the Metrology update data
  *         Pickup the data 
  * @param  None
  * @retval None
  */
void METRO_Update_Measures()
{
  if(p_Metro_Device_Config.device != 0)
  {
    METRO_UpdateData();
  }
}

/**
  * @brief  This function implements the Metrology latch device
  *         Set the HW latch for next update
  * @param  None
  * @retval None
  */
void METRO_Latch_Measures()
{
  if(p_Metro_Device_Config.device != 0)
  {
    Metro_Set_Latch_device_type(LATCH_SYN_SCS);
  }
}

/**
  * @brief  This function implements the Metrology get DSP data inside device
  * @param  None
  * @retval None
  */
void METRO_Get_Measures()
{
  if(p_Metro_Device_Config.device != 0)
  {
    Metro_Get_Data_device();
  }
}

/**
  * @brief  This function updates the Metro measurements values
  * @param  None
  * @retval None
  */
static void METRO_UpdateData(void)
{
  
  metroData.energyActive   = Metro_Read_energy(CHANNEL_1, E_W_ACTIVE);
  metroData.energyReactive = Metro_Read_energy(CHANNEL_1, E_REACTIVE);
  metroData.energyApparent = Metro_Read_energy(CHANNEL_1, E_APPARENT);
  
  metroData.powerActive = Metro_Read_Power(CHANNEL_1, W_ACTIVE);
  metroData.powerReactive = Metro_Read_Power(CHANNEL_1, REACTIVE);
  metroData.powerApparent = Metro_Read_Power(CHANNEL_1, APPARENT_RMS);
    
  Metro_Read_RMS(CHANNEL_1,&metroData.rmsvoltage,&metroData.rmscurrent,1);

}

/**
  * @brief  Initialize the Port to communicated to Metrology peripheral
  *         
  * @param[in]   None
  * @retval None
  */

static void Metro_com_port_device(void)
{
  if(p_Metro_Device_Config.device != 0)
  {
    p_Metro_Device_Config.STPM_com_port= STPM_com_port;
  }
}



/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
