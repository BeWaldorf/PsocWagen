/*******************************************************************************
* File Name: VermogenLinks.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_VermogenLinks_H)
#define CY_PWM_VermogenLinks_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 VermogenLinks_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define VermogenLinks_Resolution                     (8u)
#define VermogenLinks_UsingFixedFunction             (0u)
#define VermogenLinks_DeadBandMode                   (0u)
#define VermogenLinks_KillModeMinTime                (0u)
#define VermogenLinks_KillMode                       (0u)
#define VermogenLinks_PWMMode                        (0u)
#define VermogenLinks_PWMModeIsCenterAligned         (0u)
#define VermogenLinks_DeadBandUsed                   (0u)
#define VermogenLinks_DeadBand2_4                    (0u)

#if !defined(VermogenLinks_PWMUDB_genblk8_stsreg__REMOVED)
    #define VermogenLinks_UseStatus                  (0u)
#else
    #define VermogenLinks_UseStatus                  (0u)
#endif /* !defined(VermogenLinks_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(VermogenLinks_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define VermogenLinks_UseControl                 (1u)
#else
    #define VermogenLinks_UseControl                 (0u)
#endif /* !defined(VermogenLinks_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define VermogenLinks_UseOneCompareMode              (1u)
#define VermogenLinks_MinimumKillTime                (1u)
#define VermogenLinks_EnableMode                     (0u)

#define VermogenLinks_CompareMode1SW                 (0u)
#define VermogenLinks_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define VermogenLinks__B_PWM__DISABLED 0
#define VermogenLinks__B_PWM__ASYNCHRONOUS 1
#define VermogenLinks__B_PWM__SINGLECYCLE 2
#define VermogenLinks__B_PWM__LATCHED 3
#define VermogenLinks__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define VermogenLinks__B_PWM__DBMDISABLED 0
#define VermogenLinks__B_PWM__DBM_2_4_CLOCKS 1
#define VermogenLinks__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define VermogenLinks__B_PWM__ONE_OUTPUT 0
#define VermogenLinks__B_PWM__TWO_OUTPUTS 1
#define VermogenLinks__B_PWM__DUAL_EDGE 2
#define VermogenLinks__B_PWM__CENTER_ALIGN 3
#define VermogenLinks__B_PWM__DITHER 5
#define VermogenLinks__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define VermogenLinks__B_PWM__LESS_THAN 1
#define VermogenLinks__B_PWM__LESS_THAN_OR_EQUAL 2
#define VermogenLinks__B_PWM__GREATER_THAN 3
#define VermogenLinks__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define VermogenLinks__B_PWM__EQUAL 0
#define VermogenLinks__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!VermogenLinks_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!VermogenLinks_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!VermogenLinks_PWMModeIsCenterAligned) */
        #if (VermogenLinks_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (VermogenLinks_UseStatus) */

        /* Backup for Deadband parameters */
        #if(VermogenLinks_DeadBandMode == VermogenLinks__B_PWM__DBM_256_CLOCKS || \
            VermogenLinks_DeadBandMode == VermogenLinks__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(VermogenLinks_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (VermogenLinks_KillModeMinTime) */

        /* Backup control register */
        #if(VermogenLinks_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (VermogenLinks_UseControl) */

    #endif /* (!VermogenLinks_UsingFixedFunction) */

}VermogenLinks_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    VermogenLinks_Start(void) ;
void    VermogenLinks_Stop(void) ;

#if (VermogenLinks_UseStatus || VermogenLinks_UsingFixedFunction)
    void  VermogenLinks_SetInterruptMode(uint8 interruptMode) ;
    uint8 VermogenLinks_ReadStatusRegister(void) ;
#endif /* (VermogenLinks_UseStatus || VermogenLinks_UsingFixedFunction) */

#define VermogenLinks_GetInterruptSource() VermogenLinks_ReadStatusRegister()

#if (VermogenLinks_UseControl)
    uint8 VermogenLinks_ReadControlRegister(void) ;
    void  VermogenLinks_WriteControlRegister(uint8 control)
          ;
#endif /* (VermogenLinks_UseControl) */

#if (VermogenLinks_UseOneCompareMode)
   #if (VermogenLinks_CompareMode1SW)
       void    VermogenLinks_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (VermogenLinks_CompareMode1SW) */
#else
    #if (VermogenLinks_CompareMode1SW)
        void    VermogenLinks_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (VermogenLinks_CompareMode1SW) */
    #if (VermogenLinks_CompareMode2SW)
        void    VermogenLinks_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (VermogenLinks_CompareMode2SW) */
#endif /* (VermogenLinks_UseOneCompareMode) */

#if (!VermogenLinks_UsingFixedFunction)
    uint8   VermogenLinks_ReadCounter(void) ;
    uint8 VermogenLinks_ReadCapture(void) ;

    #if (VermogenLinks_UseStatus)
            void VermogenLinks_ClearFIFO(void) ;
    #endif /* (VermogenLinks_UseStatus) */

    void    VermogenLinks_WriteCounter(uint8 counter)
            ;
#endif /* (!VermogenLinks_UsingFixedFunction) */

void    VermogenLinks_WritePeriod(uint8 period)
        ;
uint8 VermogenLinks_ReadPeriod(void) ;

#if (VermogenLinks_UseOneCompareMode)
    void    VermogenLinks_WriteCompare(uint8 compare)
            ;
    uint8 VermogenLinks_ReadCompare(void) ;
#else
    void    VermogenLinks_WriteCompare1(uint8 compare)
            ;
    uint8 VermogenLinks_ReadCompare1(void) ;
    void    VermogenLinks_WriteCompare2(uint8 compare)
            ;
    uint8 VermogenLinks_ReadCompare2(void) ;
#endif /* (VermogenLinks_UseOneCompareMode) */


#if (VermogenLinks_DeadBandUsed)
    void    VermogenLinks_WriteDeadTime(uint8 deadtime) ;
    uint8   VermogenLinks_ReadDeadTime(void) ;
#endif /* (VermogenLinks_DeadBandUsed) */

#if ( VermogenLinks_KillModeMinTime)
    void VermogenLinks_WriteKillTime(uint8 killtime) ;
    uint8 VermogenLinks_ReadKillTime(void) ;
#endif /* ( VermogenLinks_KillModeMinTime) */

void VermogenLinks_Init(void) ;
void VermogenLinks_Enable(void) ;
void VermogenLinks_Sleep(void) ;
void VermogenLinks_Wakeup(void) ;
void VermogenLinks_SaveConfig(void) ;
void VermogenLinks_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define VermogenLinks_INIT_PERIOD_VALUE          (99u)
#define VermogenLinks_INIT_COMPARE_VALUE1        (50u)
#define VermogenLinks_INIT_COMPARE_VALUE2        (63u)
#define VermogenLinks_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    VermogenLinks_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VermogenLinks_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VermogenLinks_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VermogenLinks_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define VermogenLinks_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  VermogenLinks_CTRL_CMPMODE2_SHIFT)
#define VermogenLinks_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  VermogenLinks_CTRL_CMPMODE1_SHIFT)
#define VermogenLinks_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (VermogenLinks_UsingFixedFunction)
   #define VermogenLinks_PERIOD_LSB              (*(reg16 *) VermogenLinks_PWMHW__PER0)
   #define VermogenLinks_PERIOD_LSB_PTR          ( (reg16 *) VermogenLinks_PWMHW__PER0)
   #define VermogenLinks_COMPARE1_LSB            (*(reg16 *) VermogenLinks_PWMHW__CNT_CMP0)
   #define VermogenLinks_COMPARE1_LSB_PTR        ( (reg16 *) VermogenLinks_PWMHW__CNT_CMP0)
   #define VermogenLinks_COMPARE2_LSB            (0x00u)
   #define VermogenLinks_COMPARE2_LSB_PTR        (0x00u)
   #define VermogenLinks_COUNTER_LSB             (*(reg16 *) VermogenLinks_PWMHW__CNT_CMP0)
   #define VermogenLinks_COUNTER_LSB_PTR         ( (reg16 *) VermogenLinks_PWMHW__CNT_CMP0)
   #define VermogenLinks_CAPTURE_LSB             (*(reg16 *) VermogenLinks_PWMHW__CAP0)
   #define VermogenLinks_CAPTURE_LSB_PTR         ( (reg16 *) VermogenLinks_PWMHW__CAP0)
   #define VermogenLinks_RT1                     (*(reg8 *)  VermogenLinks_PWMHW__RT1)
   #define VermogenLinks_RT1_PTR                 ( (reg8 *)  VermogenLinks_PWMHW__RT1)

#else
   #if (VermogenLinks_Resolution == 8u) /* 8bit - PWM */

       #if(VermogenLinks_PWMModeIsCenterAligned)
           #define VermogenLinks_PERIOD_LSB      (*(reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define VermogenLinks_PERIOD_LSB_PTR  ((reg8 *)   VermogenLinks_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define VermogenLinks_PERIOD_LSB      (*(reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define VermogenLinks_PERIOD_LSB_PTR  ((reg8 *)   VermogenLinks_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (VermogenLinks_PWMModeIsCenterAligned) */

       #define VermogenLinks_COMPARE1_LSB        (*(reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define VermogenLinks_COMPARE1_LSB_PTR    ((reg8 *)   VermogenLinks_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define VermogenLinks_COMPARE2_LSB        (*(reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define VermogenLinks_COMPARE2_LSB_PTR    ((reg8 *)   VermogenLinks_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define VermogenLinks_COUNTERCAP_LSB      (*(reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define VermogenLinks_COUNTERCAP_LSB_PTR  ((reg8 *)   VermogenLinks_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define VermogenLinks_COUNTER_LSB         (*(reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define VermogenLinks_COUNTER_LSB_PTR     ((reg8 *)   VermogenLinks_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define VermogenLinks_CAPTURE_LSB         (*(reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define VermogenLinks_CAPTURE_LSB_PTR     ((reg8 *)   VermogenLinks_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(VermogenLinks_PWMModeIsCenterAligned)
               #define VermogenLinks_PERIOD_LSB      (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define VermogenLinks_PERIOD_LSB_PTR  ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define VermogenLinks_PERIOD_LSB      (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define VermogenLinks_PERIOD_LSB_PTR  ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (VermogenLinks_PWMModeIsCenterAligned) */

            #define VermogenLinks_COMPARE1_LSB       (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define VermogenLinks_COMPARE1_LSB_PTR   ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define VermogenLinks_COMPARE2_LSB       (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define VermogenLinks_COMPARE2_LSB_PTR   ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define VermogenLinks_COUNTERCAP_LSB     (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define VermogenLinks_COUNTERCAP_LSB_PTR ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define VermogenLinks_COUNTER_LSB        (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define VermogenLinks_COUNTER_LSB_PTR    ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define VermogenLinks_CAPTURE_LSB        (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define VermogenLinks_CAPTURE_LSB_PTR    ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(VermogenLinks_PWMModeIsCenterAligned)
               #define VermogenLinks_PERIOD_LSB      (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define VermogenLinks_PERIOD_LSB_PTR  ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define VermogenLinks_PERIOD_LSB      (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define VermogenLinks_PERIOD_LSB_PTR  ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (VermogenLinks_PWMModeIsCenterAligned) */

            #define VermogenLinks_COMPARE1_LSB       (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define VermogenLinks_COMPARE1_LSB_PTR   ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define VermogenLinks_COMPARE2_LSB       (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define VermogenLinks_COMPARE2_LSB_PTR   ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define VermogenLinks_COUNTERCAP_LSB     (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define VermogenLinks_COUNTERCAP_LSB_PTR ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define VermogenLinks_COUNTER_LSB        (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define VermogenLinks_COUNTER_LSB_PTR    ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define VermogenLinks_CAPTURE_LSB        (*(reg16 *) VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define VermogenLinks_CAPTURE_LSB_PTR    ((reg16 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define VermogenLinks_AUX_CONTROLDP1          (*(reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define VermogenLinks_AUX_CONTROLDP1_PTR      ((reg8 *)   VermogenLinks_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (VermogenLinks_Resolution == 8) */

   #define VermogenLinks_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define VermogenLinks_AUX_CONTROLDP0          (*(reg8 *)  VermogenLinks_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define VermogenLinks_AUX_CONTROLDP0_PTR      ((reg8 *)   VermogenLinks_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (VermogenLinks_UsingFixedFunction) */

#if(VermogenLinks_KillModeMinTime )
    #define VermogenLinks_KILLMODEMINTIME        (*(reg8 *)  VermogenLinks_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define VermogenLinks_KILLMODEMINTIME_PTR    ((reg8 *)   VermogenLinks_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (VermogenLinks_KillModeMinTime ) */

#if(VermogenLinks_DeadBandMode == VermogenLinks__B_PWM__DBM_256_CLOCKS)
    #define VermogenLinks_DEADBAND_COUNT         (*(reg8 *)  VermogenLinks_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define VermogenLinks_DEADBAND_COUNT_PTR     ((reg8 *)   VermogenLinks_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define VermogenLinks_DEADBAND_LSB_PTR       ((reg8 *)   VermogenLinks_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define VermogenLinks_DEADBAND_LSB           (*(reg8 *)  VermogenLinks_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(VermogenLinks_DeadBandMode == VermogenLinks__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (VermogenLinks_UsingFixedFunction)
        #define VermogenLinks_DEADBAND_COUNT         (*(reg8 *)  VermogenLinks_PWMHW__CFG0)
        #define VermogenLinks_DEADBAND_COUNT_PTR     ((reg8 *)   VermogenLinks_PWMHW__CFG0)
        #define VermogenLinks_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << VermogenLinks_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define VermogenLinks_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define VermogenLinks_DEADBAND_COUNT         (*(reg8 *)  VermogenLinks_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define VermogenLinks_DEADBAND_COUNT_PTR     ((reg8 *)   VermogenLinks_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define VermogenLinks_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << VermogenLinks_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define VermogenLinks_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (VermogenLinks_UsingFixedFunction) */
#endif /* (VermogenLinks_DeadBandMode == VermogenLinks__B_PWM__DBM_256_CLOCKS) */



#if (VermogenLinks_UsingFixedFunction)
    #define VermogenLinks_STATUS                 (*(reg8 *) VermogenLinks_PWMHW__SR0)
    #define VermogenLinks_STATUS_PTR             ((reg8 *) VermogenLinks_PWMHW__SR0)
    #define VermogenLinks_STATUS_MASK            (*(reg8 *) VermogenLinks_PWMHW__SR0)
    #define VermogenLinks_STATUS_MASK_PTR        ((reg8 *) VermogenLinks_PWMHW__SR0)
    #define VermogenLinks_CONTROL                (*(reg8 *) VermogenLinks_PWMHW__CFG0)
    #define VermogenLinks_CONTROL_PTR            ((reg8 *) VermogenLinks_PWMHW__CFG0)
    #define VermogenLinks_CONTROL2               (*(reg8 *) VermogenLinks_PWMHW__CFG1)
    #define VermogenLinks_CONTROL3               (*(reg8 *) VermogenLinks_PWMHW__CFG2)
    #define VermogenLinks_GLOBAL_ENABLE          (*(reg8 *) VermogenLinks_PWMHW__PM_ACT_CFG)
    #define VermogenLinks_GLOBAL_ENABLE_PTR      ( (reg8 *) VermogenLinks_PWMHW__PM_ACT_CFG)
    #define VermogenLinks_GLOBAL_STBY_ENABLE     (*(reg8 *) VermogenLinks_PWMHW__PM_STBY_CFG)
    #define VermogenLinks_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) VermogenLinks_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define VermogenLinks_BLOCK_EN_MASK          (VermogenLinks_PWMHW__PM_ACT_MSK)
    #define VermogenLinks_BLOCK_STBY_EN_MASK     (VermogenLinks_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define VermogenLinks_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define VermogenLinks_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define VermogenLinks_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define VermogenLinks_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define VermogenLinks_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define VermogenLinks_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define VermogenLinks_CTRL_ENABLE            (uint8)((uint8)0x01u << VermogenLinks_CTRL_ENABLE_SHIFT)
    #define VermogenLinks_CTRL_RESET             (uint8)((uint8)0x01u << VermogenLinks_CTRL_RESET_SHIFT)
    #define VermogenLinks_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << VermogenLinks_CTRL_CMPMODE2_SHIFT)
    #define VermogenLinks_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << VermogenLinks_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define VermogenLinks_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define VermogenLinks_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << VermogenLinks_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define VermogenLinks_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define VermogenLinks_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define VermogenLinks_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define VermogenLinks_STATUS_TC_INT_EN_MASK_SHIFT            (VermogenLinks_STATUS_TC_SHIFT - 4u)
    #define VermogenLinks_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define VermogenLinks_STATUS_CMP1_INT_EN_MASK_SHIFT          (VermogenLinks_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define VermogenLinks_STATUS_TC              (uint8)((uint8)0x01u << VermogenLinks_STATUS_TC_SHIFT)
    #define VermogenLinks_STATUS_CMP1            (uint8)((uint8)0x01u << VermogenLinks_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define VermogenLinks_STATUS_TC_INT_EN_MASK              (uint8)((uint8)VermogenLinks_STATUS_TC >> 4u)
    #define VermogenLinks_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)VermogenLinks_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define VermogenLinks_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define VermogenLinks_RT1_MASK              (uint8)((uint8)0x03u << VermogenLinks_RT1_SHIFT)
    #define VermogenLinks_SYNC                  (uint8)((uint8)0x03u << VermogenLinks_RT1_SHIFT)
    #define VermogenLinks_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define VermogenLinks_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << VermogenLinks_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define VermogenLinks_SYNCDSI_EN            (uint8)((uint8)0x0Fu << VermogenLinks_SYNCDSI_SHIFT)


#else
    #define VermogenLinks_STATUS                (*(reg8 *)   VermogenLinks_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define VermogenLinks_STATUS_PTR            ((reg8 *)    VermogenLinks_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define VermogenLinks_STATUS_MASK           (*(reg8 *)   VermogenLinks_PWMUDB_genblk8_stsreg__MASK_REG)
    #define VermogenLinks_STATUS_MASK_PTR       ((reg8 *)    VermogenLinks_PWMUDB_genblk8_stsreg__MASK_REG)
    #define VermogenLinks_STATUS_AUX_CTRL       (*(reg8 *)   VermogenLinks_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define VermogenLinks_CONTROL               (*(reg8 *)   VermogenLinks_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define VermogenLinks_CONTROL_PTR           ((reg8 *)    VermogenLinks_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define VermogenLinks_CTRL_ENABLE_SHIFT      (0x07u)
    #define VermogenLinks_CTRL_RESET_SHIFT       (0x06u)
    #define VermogenLinks_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define VermogenLinks_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define VermogenLinks_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define VermogenLinks_CTRL_ENABLE            (uint8)((uint8)0x01u << VermogenLinks_CTRL_ENABLE_SHIFT)
    #define VermogenLinks_CTRL_RESET             (uint8)((uint8)0x01u << VermogenLinks_CTRL_RESET_SHIFT)
    #define VermogenLinks_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << VermogenLinks_CTRL_CMPMODE2_SHIFT)
    #define VermogenLinks_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << VermogenLinks_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define VermogenLinks_STATUS_KILL_SHIFT          (0x05u)
    #define VermogenLinks_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define VermogenLinks_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define VermogenLinks_STATUS_TC_SHIFT            (0x02u)
    #define VermogenLinks_STATUS_CMP2_SHIFT          (0x01u)
    #define VermogenLinks_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define VermogenLinks_STATUS_KILL_INT_EN_MASK_SHIFT          (VermogenLinks_STATUS_KILL_SHIFT)
    #define VermogenLinks_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (VermogenLinks_STATUS_FIFONEMPTY_SHIFT)
    #define VermogenLinks_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (VermogenLinks_STATUS_FIFOFULL_SHIFT)
    #define VermogenLinks_STATUS_TC_INT_EN_MASK_SHIFT            (VermogenLinks_STATUS_TC_SHIFT)
    #define VermogenLinks_STATUS_CMP2_INT_EN_MASK_SHIFT          (VermogenLinks_STATUS_CMP2_SHIFT)
    #define VermogenLinks_STATUS_CMP1_INT_EN_MASK_SHIFT          (VermogenLinks_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define VermogenLinks_STATUS_KILL            (uint8)((uint8)0x00u << VermogenLinks_STATUS_KILL_SHIFT )
    #define VermogenLinks_STATUS_FIFOFULL        (uint8)((uint8)0x01u << VermogenLinks_STATUS_FIFOFULL_SHIFT)
    #define VermogenLinks_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << VermogenLinks_STATUS_FIFONEMPTY_SHIFT)
    #define VermogenLinks_STATUS_TC              (uint8)((uint8)0x01u << VermogenLinks_STATUS_TC_SHIFT)
    #define VermogenLinks_STATUS_CMP2            (uint8)((uint8)0x01u << VermogenLinks_STATUS_CMP2_SHIFT)
    #define VermogenLinks_STATUS_CMP1            (uint8)((uint8)0x01u << VermogenLinks_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define VermogenLinks_STATUS_KILL_INT_EN_MASK            (VermogenLinks_STATUS_KILL)
    #define VermogenLinks_STATUS_FIFOFULL_INT_EN_MASK        (VermogenLinks_STATUS_FIFOFULL)
    #define VermogenLinks_STATUS_FIFONEMPTY_INT_EN_MASK      (VermogenLinks_STATUS_FIFONEMPTY)
    #define VermogenLinks_STATUS_TC_INT_EN_MASK              (VermogenLinks_STATUS_TC)
    #define VermogenLinks_STATUS_CMP2_INT_EN_MASK            (VermogenLinks_STATUS_CMP2)
    #define VermogenLinks_STATUS_CMP1_INT_EN_MASK            (VermogenLinks_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define VermogenLinks_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define VermogenLinks_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define VermogenLinks_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define VermogenLinks_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define VermogenLinks_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* VermogenLinks_UsingFixedFunction */

#endif  /* CY_PWM_VermogenLinks_H */


/* [] END OF FILE */
