
/***********************************************************************************
  Copyright 2007 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
***********************************************************************************/

/***********************************************************************************
  Filename:     hal_mcu.c
    
  Description:  hal mcu library 
    
***********************************************************************************/

#include "hal_foundation.h"
//#include <intrins.h>
#include "hal_cpu.h"
#include "hal_mcu.h"
#include "cm3/device/stm32f10x/stm32f10x.h"

GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;
NVIC_InitTypeDef NVIC_InitStruct;
EXTI_InitTypeDef EXTI_InitStructure;  
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

static void _RCC_Configuration(void);
static void _GPIO_Configuration(void);
static void _NVIC_Configuration(void);

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures System Clocks
*******************************************************************************/
void _RCC_Configuration(void)
{
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    RCC_PCLK1Config(RCC_HCLK_Div2);
    FLASH_SetLatency(FLASH_Latency_2);
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)	;
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
  }
   
  /* Enable GPIOA|B*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  /* TIM1 clock enable */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /* TIM2 clock enable */
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* ADC1 clock enable */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* GPIOA, GPIOB and SPI1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  /* SPI2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/*******************************************************************************
* Function Name  : _GPIO_Configuration
* Description    : Configures GPIO Modes
*******************************************************************************/
void _GPIO_Configuration()
{
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : _NVIC_Configuration
* Description    : Configures NVIC Modes for T1
*******************************************************************************/
void _NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  //NVIC_SetVectorTableNVIC_SetVectorTableNVIC_SetVectorTableNVIC_SetVectorTableNVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  /* Configure the NVIC Preemption Priority Bits[�������ȼ���] */  
 // todo
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the TIM1 gloabal Interrupt [����TIM1ȫ���ж�]*/
/*  
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/  

  /* Enable the TIM2 gloabal Interrupt [����TIM2ȫ���ж�]*/
/*
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/
  /* Enable the RTC Interrupt */
/*
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/
  /* Configure INT IO  PC9 enable exti9_5*/
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);  
  EXTI_InitStructure.EXTI_Line=EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
/*
  NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQChannel;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
  NVIC_Init(&NVIC_InitStruct);
*/
  /* Configure INT IO  PE4 enable exti4*/
/*
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);  
  EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQChannel;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
  NVIC_Init(&NVIC_InitStruct);
*/
  /* Configure INT IO  PE5 enable exti9_5*/
/*
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);  
  EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQChannel;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
  NVIC_Init(&NVIC_InitStruct);
*/
}
  
/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures the USART1.
*******************************************************************************/

/*******************************************************************************
* Function Name  : BSP_Init
* Description    : Configures RCC,GPIO,NVIC
*******************************************************************************/
void mcu_init()
{
  /* Configure the system clocks */
  _RCC_Configuration();
    
  /* GPIO Configuration */
  _GPIO_Configuration();

  /* NVIC Configuration */
  _NVIC_Configuration();
}

/*******************************************************************************
* Function Name  : delay_nus
* Description    : delay n us
*******************************************************************************/
void delay_nus(unsigned long n)  //��ʱn us: n>=6,��С��ʱ��λ6us
{ 
// todo: should adjust hardware settings to guarntee the time duration is correct
#ifdef CONFIG_TARGETBOARD_OPENNODE2010
	unsigned long j;
	while (n--)
	{
		j=0;
		while (j--) cpu_nop();
	}
#endif

#ifndef CONFIG_TARGETBOARD_OPENNODE2010
  unsigned long j;
  while(n--)              // �ⲿ����8M��PLL��9��8M*9=72MHz
  {
    j=8;				  // ΢����������֤��ʱ�ľ���
	while(j--);
  }
#endif  
}

/*******************************************************************************
* Function Name  : delay_nms
* Description    : delay n ms
*******************************************************************************/
void delay_nms(unsigned long n)  //��ʱn ms
{
  while(n--)		   // �ⲿ����8M��PLL��9��8M*9=72MHz
    delay_nus(1100);   // 1ms��ʱ����
}





/***********************************************************************************
* INCLUDES
*/
#include "hal_configall.h"
#include "hal_foundation.h"
#include "hal_mcu.h"
//#include "hal_board.h"

/***********************************************************************************
* @fn          halMcuInit
*
* @brief       Turn off watchdog and set up system clock. Set system clock to 
*              8 MHz 
*
* @param       none
*             
* @return      none
*/
void halMcuInit(void)
{
/* todo
    uint16 i;
    
    // Stop watchdog
    WDTCTL = WDTPW + WDTHOLD;
    
    // Wait for xtal to stabilize
    while (IFG1 & OFIFG)
    {
        // Clear oscillator fault flag
        IFG1 &= ~OFIFG;
        for (i = 0x4800; i > 0; i--) asm("NOP");
    }
    
    // Set clock source to DCO @ 8 MHz 
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
    BCSCTL1 |= XT2OFF;
    
    // Wait for DCO to synchronize with ACLK (at least 28*32 ACLK cycles)
    for (i = 0x1C00; i > 0; i--) asm("NOP");
*/
}


/***********************************************************************************
* @fn          halMcuWaitUs
*
* @brief       Busy wait function. Waits the specified number of microseconds. Use
*              assumptions about number of clock cycles needed for the various 
*              instructions. The duration of one cycle depends on MCLK. In this HAL
*              , it is set to 8 MHz, thus 8 cycles per usec.
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint16 usec - number of microseconds delay
*             
* @return      none
*/
// todo	 important
//#pragma optimize=none
void halMcuWaitUs(uint16 usec) // 5 cycles for calling
{
	int j;

#ifdef CONFIG_TOOLCHAIN_MDK
/*
	// The least we can wait is 3 usec:
    // ~1 one cycle for call, 1 for first compare and 1 for return 
    while(usec > 3)       // 2 cycles for compare
    {                     // 2 cycles for jump
        __asm("NOP");       // 1 cycles for nop
        __asm("NOP");       // 1 cycles for nop
        __asm("NOP");       // 1 cycles for nop
        __asm("NOP");       // 1 cycles for nop
        __asm("NOP");       // 1 cycles for nop
        __asm("NOP");       // 1 cycles for nop
        __asm("NOP");       // 1 cycles for nop
        __asm("NOP");       // 1 cycles for nop
        usec -= 2;        // 1 cycles for optimized decrement
    }
*/
	while(usec > 1)       
	{            
		for ( j=0;j<70;j++)
			 __nop();
		usec --;            // 1 cycles for optimized decrement
	}
#else
	#error "unsupported compiler"
#endif

}                         // 4 cycles for returning


/***********************************************************************************
* @fn          halMcuWaitMs
*
* @brief       Busy wait function. Waits the specified number of milliseconds. Use
*              assumptions about number of clock cycles needed for the various 
*              instructions.
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint16 millisec - number of milliseconds delay
*             
* @return      none
*/
// todo
// #pragma optimize=none
void halMcuWaitMs(uint16 msec)
{
    while(msec-- > 0)
    {
        halMcuWaitUs(1000);
    }
}


/***********************************************************************************
* @fn          halMcuSetLowPowerMode
*
* @brief      Sets the MCU in a low power mode. Will turn global interrupts on at
*             the same time as entering the LPM mode. The MCU must be waken from
*             an interrupt (status register on stack must be modified).
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint8 mode - power mode
*             
* @return      none
*/
void halMcuSetLowPowerMode(uint8 mode)
{
/* todo
    switch (mode)
    {
    case HAL_MCU_LPM_0:
        __low_power_mode_0();
        break;
    case HAL_MCU_LPM_1:
        __low_power_mode_1();
        break;
    case HAL_MCU_LPM_2:
        __low_power_mode_2();
        break;
    case HAL_MCU_LPM_3:
        __low_power_mode_3();
        break;
    case HAL_MCU_LPM_4:
        __low_power_mode_4();
        break;
    }
*/
}
