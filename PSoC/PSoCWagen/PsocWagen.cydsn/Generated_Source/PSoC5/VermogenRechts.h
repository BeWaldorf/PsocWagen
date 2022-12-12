/*******************************************************************************
* File Name: VermogenRechts.h
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

#if !defined(CY_PWM_VermogenRechts_H)
#define CY_PWM_VermogenRechts_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 VermogenRechts_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define VermogenRechts_Resolution                     (8u)
#define VermogenRechts_UsingFixedFunction             (0u)
#define VermogenRechts_DeadBandMode                   (0u)
#define VermogenRechts_KillModeMinTime                (0u)
#define VermogenRechts_KillMode                       (0u)
#define VermogenRechts_PWMMode                        (0u)
#define VermogenRechts_PWMModeIsCenterAligned         (0u)
#define VermogenRechts_DeadBandUsed                   (0u)
#define VermogenRechts_DeadBand2_4                    (0u)

#if !defined(VermogenRechts_PWMUDB_genblk8_stsreg__REMOVED)
    #define VermogenRechts_UseStatus                  (0u)
#else
    #define VermogenRechts_UseStatus                  (0u)
#endif /* !defined(VermogenRechts_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(VermogenRechts_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define VermogenRechts_UseControl                 (1u)
#else
    #define VermogenRechts_UseControl                 (0u)
#endif /* !defined(VermogenRechts_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define VermogenRechts_UseOneCompareMode              (1u)
#define VermogenRechts_MinimumKillTime                (1u)
#define VermogenRechts_EnableMode                     (0u)

#define VermogenRechts_CompareMode1SW                 (0u)
#define VermogenRechts_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define VermogenRechts__B_PWM__DISABLED 0
#define VermogenRechts__B_PWM__ASYNCHRONOUS 1
#define VermogenRechts__B_PWM__SINGLECYCLE 2
#define VermogenRechts__B_PWM__LATCHED 3
#define VermogenRechts__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define VermogenRechts__B_PWM__DBMDISABLED 0
#define VermogenRechts__B_PWM__DBM_2_4_CLOCKS 1
#define VermogenRechts__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define VermogenRechts__B_PWM__ONE_OUTPUT 0
#define VermogenRechts__B_PWM__TWO_OUTPUTS 1
#define VermogenRechts__B_PWM__DUAL_EDGE 2
#define VermogenRechts__B_PWM__CENTER_ALIGN 3
#define VermogenRechts__B_PWM__DITHER 5
#define VermogenRechts__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define VermogenRechts__B_PWM__LESS_THAN 1
#define VermogenRechts__B_PWM__LESS_THAN_OR_EQUAL 2
#define VermogenRechts__B_PWM__GREATER_THAN 3
#define VermogenRechts__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define VermogenRechts__B_PWM__EQUAL 0
#define VermogenRechts__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!VermogenRechts_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!VermogenRechts_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!VermogenRechts_PWMModeIsCenterAligned) */
        #if (VermogenRechts_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (VermogenRechts_UseStatus) */

        /* Backup for Deadband parameters */
        #if(VermogenRechts_DeadBandMode == VermogenRechts__B_PWM__DBM_256_CLOCKS || \
            VermogenRechts_DeadBandMode == VermogenRechts__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(VermogenRechts_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (VermogenRechts_KillModeMinTime) */

        /* Backup control register */
        #if(VermogenRechts_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (VermogenRechts_UseControl) */

    #endif /* (!VermogenRechts_UsingFixedFunction) */

}VermogenRechts_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    VermogenRechts_Start(void) ;
void    VermogenRechts_Stop(void) ;

#if (VermogenRechts_UseStatus || VermogenRechts_UsingFixedFunction)
    void  VermogenRechts_SetInterruptMode(uint8 interruptMode) ;
    uint8 VermogenRechts_ReadStatusRegister(void) ;
#endif /* (VermogenRechts_UseStatus || VermogenRechts_UsingFixedFunction) */

#define VermogenRechts_GetInterruptSource() VermogenRechts_ReadStatusRegister()

#if (VermogenRechts_UseControl)
    uint8 VermogenRechts_ReadControlRegister(void) ;
    void  VermogenRechts_WriteControlRegister(uint8 control)
          ;
#endif /* (VermogenRechts_UseControl) */

#if (VermogenRechts_UseOneCompareMode)
   #if (VermogenRechts_CompareMode1SW)
       void    VermogenRechts_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (VermogenRechts_CompareMode1SW) */
#else
    #if (VermogenRechts_CompareMode1SW)
        void    VermogenRechts_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (VermogenRechts_CompareMode1SW) */
    #if (VermogenRechts_CompareMode2SW)
        void    VermogenRechts_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (VermogenRechts_CompareMode2SW) */
#endif /* (VermogenRechts_UseOneCompareMode) */

#if (!VermogenRechts_UsingFixedFunction)
    uint8   VermogenRechts_ReadCounter(void) ;
    uint8 VermogenRechts_ReadCapture(void) ;

    #if (VermogenRechts_UseStatus)
            void VermogenRechts_ClearFIFO(void) ;
    #endif /* (VermogenRechts_UseStatus) */

    void    VermogenRechts_WriteCounter(uint8 counter)
            ;
#endif /* (!VermogenRechts_UsingFixedFunction) */

void    VermogenRechts_WritePeriod(uint8 period)
        ;
uint8 VermogenRechts_ReadPeriod(void) ;

#if (VermogenRechts_UseOneCompareMode)
    void    VermogenRechts_WriteCompare(uint8 compare)
            ;
    uint8 VermogenRechts_ReadCompare(void) ;
#else
    void    VermogenRechts_WriteCompare1(uint8 compare)
            ;
    uint8 VermogenRechts_ReadCompare1(void) ;
    void    VermogenRechts_WriteCompare2(uint8 compare)
            ;
    uint8 VermogenRechts_ReadCompare2(void) ;
#endif /* (VermogenRechts_UseOneCompareMode) */


#if (VermogenRechts_DeadBandUsed)
    void    VermogenRechts_WriteDeadTime(uint8 deadtime) ;
    uint8   VermogenRechts_ReadDeadTime(void) ;
#endif /* (VermogenRechts_DeadBandUsed) */

#if ( VermogenRechts_KillModeMinTime)
    void VermogenRechts_WriteKillTime(uint8 killtime) ;
    uint8 VermogenRechts_ReadKillTime(void) ;
#endif /* ( VermogenRechts_KillModeMinTime) */

void VermogenRechts_Init(void) ;
void VermogenRechts_Enable(void) ;
void VermogenRechts_Sleep(void) ;
void VermogenRechts_Wakeup(void) ;
void VermogenRechts_SaveConfig(void) ;
void VermogenRechts_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define VermogenRechts_INIT_PERIOD_VALUE          (99u)
#define VermogenRechts_INIT_COMPARE_VALUE1        (50u)
#define VermogenRechts_INIT_COMPARE_VALUE2        (63u)
#define VermogenRechts_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    VermogenRechts_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VermogenRechts_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VermogenRechts_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VermogenRechts_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define VermogenRechts_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  VermogenRechts_CTRL_CMPMODE2_SHIFT)
#define VermogenRechts_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  VermogenRechts_CTRL_CMPMODE1_SHIFT)
#define VermogenRechts_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (VermogenRechts_UsingFixedFunction)
   #define VermogenRechts_PERIOD_LSB              (*(reg16 *) VermogenRechts_PWMHW__PER0)
   #define VermogenRechts_PERIOD_LSB_PTR          ( (reg16 *) VermogenRechts_PWMHW__PER0)
   #define VermogenRechts_COMPARE1_LSB            (*(reg16 *) VermogenRechts_PWMHW__CNT_CMP0)
   #define VermogenRechts_COMPARE1_LSB_PTR        ( (reg16 *) VermogenRechts_PWMHW__CNT_CMP0)
   #define VermogenRechts_COMPARE2_LSB            (0x00u)
   #define VermogenRechts_COMPARE2_LSB_PTR        (0x00u)
   #define VermogenRechts_COUNTER_LSB             (*(reg16 *) VermogenRechts_PWMHW__CNT_CMP0)
   #define VermogenRechts_COUNTER_LSB_PTR         ( (reg16 *) VermogenRechts_PWMHW__CNT_CMP0)
   #define VermogenRechts_CAPTURE_LSB             (*(reg16 *) VermogenRechts_PWMHW__CAP0)
   #define VermogenRechts_CAPTURE_LSB_PTR         ( (reg16 *) VermogenRechts_PWMHW__CAP0)
   #define VermogenRechts_RT1                     (*(reg8 *)  VermogenRechts_PWMHW__RT1)
   #define VermogenRechts_RT1_PTR                 ( (reg8 *)  VermogenRechts_PWMHW__RT1)

#else
   #if (VermogenRechts_Resolution == 8u) /* 8bit - PWM */

       #if(VermogenRechts_PWMModeIsCenterAligned)
           #define VermogenRechts_PERIOD_LSB      (*(reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define VermogenRechts_PERIOD_LSB_PTR  ((reg8 *)   VermogenRechts_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define VermogenRechts_PERIOD_LSB      (*(reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define VermogenRechts_PERIOD_LSB_PTR  ((reg8 *)   VermogenRechts_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (VermogenRechts_PWMModeIsCenterAligned) */

       #define VermogenRechts_COMPARE1_LSB        (*(reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define VermogenRechts_COMPARE1_LSB_PTR    ((reg8 *)   VermogenRechts_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define VermogenRechts_COMPARE2_LSB        (*(reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define VermogenRechts_COMPARE2_LSB_PTR    ((reg8 *)   VermogenRechts_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define VermogenRechts_COUNTERCAP_LSB      (*(reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define VermogenRechts_COUNTERCAP_LSB_PTR  ((reg8 *)   VermogenRechts_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define VermogenRechts_COUNTER_LSB         (*(reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define VermogenRechts_COUNTER_LSB_PTR     ((reg8 *)   VermogenRechts_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define VermogenRechts_CAPTURE_LSB         (*(reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define VermogenRechts_CAPTURE_LSB_PTR     ((reg8 *)   VermogenRechts_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(VermogenRechts_PWMModeIsCenterAligned)
               #define VermogenRechts_PERIOD_LSB      (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define VermogenRechts_PERIOD_LSB_PTR  ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define VermogenRechts_PERIOD_LSB      (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define VermogenRechts_PERIOD_LSB_PTR  ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (VermogenRechts_PWMModeIsCenterAligned) */

            #define VermogenRechts_COMPARE1_LSB       (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define VermogenRechts_COMPARE1_LSB_PTR   ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define VermogenRechts_COMPARE2_LSB       (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define VermogenRechts_COMPARE2_LSB_PTR   ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define VermogenRechts_COUNTERCAP_LSB     (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define VermogenRechts_COUNTERCAP_LSB_PTR ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define VermogenRechts_COUNTER_LSB        (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define VermogenRechts_COUNTER_LSB_PTR    ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define VermogenRechts_CAPTURE_LSB        (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define VermogenRechts_CAPTURE_LSB_PTR    ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(VermogenRechts_PWMModeIsCenterAligned)
               #define VermogenRechts_PERIOD_LSB      (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define VermogenRechts_PERIOD_LSB_PTR  ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define VermogenRechts_PERIOD_LSB      (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define VermogenRechts_PERIOD_LSB_PTR  ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (VermogenRechts_PWMModeIsCenterAligned) */

            #define VermogenRechts_COMPARE1_LSB       (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define VermogenRechts_COMPARE1_LSB_PTR   ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define VermogenRechts_COMPARE2_LSB       (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define VermogenRechts_COMPARE2_LSB_PTR   ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define VermogenRechts_COUNTERCAP_LSB     (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define VermogenRechts_COUNTERCAP_LSB_PTR ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define VermogenRechts_COUNTER_LSB        (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define VermogenRechts_COUNTER_LSB_PTR    ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define VermogenRechts_CAPTURE_LSB        (*(reg16 *) VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define VermogenRechts_CAPTURE_LSB_PTR    ((reg16 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define VermogenRechts_AUX_CONTROLDP1          (*(reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define VermogenRechts_AUX_CONTROLDP1_PTR      ((reg8 *)   VermogenRechts_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (VermogenRechts_Resolution == 8) */

   #define VermogenRechts_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define VermogenRechts_AUX_CONTROLDP0          (*(reg8 *)  VermogenRechts_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define VermogenRechts_AUX_CONTROLDP0_PTR      ((reg8 *)   VermogenRechts_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (VermogenRechts_UsingFixedFunction) */

#if(VermogenRechts_KillModeMinTime )
    #define VermogenRechts_KILLMODEMINTIME        (*(reg8 *)  VermogenRechts_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define VermogenRechts_KILLMODEMINTIME_PTR    ((reg8 *)   VermogenRechts_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (VermogenRechts_KillModeMinTime ) */

#if(VermogenRechts_DeadBandMode == VermogenRechts__B_PWM__DBM_256_CLOCKS)
    #define VermogenRechts_DEADBAND_COUNT         (*(reg8 *)  VermogenRechts_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define VermogenRechts_DEADBAND_COUNT_PTR     ((reg8 *)   VermogenRechts_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define VermogenRechts_DEADBAND_LSB_PTR       ((reg8 *)   VermogenRechts_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define VermogenRechts_DEADBAND_LSB           (*(reg8 *)  VermogenRechts_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(VermogenRechts_DeadBandMode == VermogenRechts__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (VermogenRechts_UsingFixedFunction)
        #define VermogenRechts_DEADBAND_COUNT         (*(reg8 *)  VermogenRechts_PWMHW__CFG0)
        #define VermogenRechts_DEADBAND_COUNT_PTR     ((reg8 *)   VermogenRechts_PWMHW__CFG0)
        #define VermogenRechts_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << VermogenRechts_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define VermogenRechts_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define VermogenRechts_DEADBAND_COUNT         (*(reg8 *)  VermogenRechts_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define VermogenRechts_DEADBAND_COUNT_PTR     ((reg8 *)   VermogenRechts_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define VermogenRechts_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << VermogenRechts_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define VermogenRechts_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (VermogenRechts_UsingFixedFunction) */
#endif /* (VermogenRechts_DeadBandMode == VermogenRechts__B_PWM__DBM_256_CLOCKS) */



#if (VermogenRechts_UsingFixedFunction)
    #define VermogenRechts_STATUS                 (*(reg8 *) VermogenRechts_PWMHW__SR0)
    #define VermogenRechts_STATUS_PTR             ((reg8 *) VermogenRechts_PWMHW__SR0)
    #define VermogenRechts_STATUS_MASK            (*(reg8 *) VermogenRechts_PWMHW__SR0)
    #define VermogenRechts_STATUS_MASK_PTR        ((reg8 *) VermogenRechts_PWMHW__SR0)
    #define VermogenRechts_CONTROL                (*(reg8 *) VermogenRechts_PWMHW__CFG0)
    #define VermogenRechts_CONTROL_PTR            ((reg8 *) VermogenRechts_PWMHW__CFG0)
    #define VermogenRechts_CONTROL2               (*(reg8 *) VermogenRechts_PWMHW__CFG1)
    #define VermogenRechts_CONTROL3               (*(reg8 *) VermogenRechts_PWMHW__CFG2)
    #define VermogenRechts_GLOBAL_ENABLE          (*(reg8 *) VermogenRechts_PWMHW__PM_ACT_CFG)
    #define VermogenRechts_GLOBAL_ENABLE_PTR      ( (reg8 *) VermogenRechts_PWMHW__PM_ACT_CFG)
    #define VermogenRechts_GLOBAL_STBY_ENABLE     (*(reg8 *) VermogenRechts_PWMHW__PM_STBY_CFG)
    #define VermogenRechts_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) VermogenRechts_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define VermogenRechts_BLOCK_EN_MASK          (VermogenRechts_PWMHW__PM_ACT_MSK)
    #define VermogenRechts_BLOCK_STBY_EN_MASK     (VermogenRechts_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define VermogenRechts_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define VermogenRechts_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define VermogenRechts_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define VermogenRechts_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define VermogenRechts_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define VermogenRechts_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define VermogenRechts_CTRL_ENABLE            (uint8)((uint8)0x01u << VermogenRechts_CTRL_ENABLE_SHIFT)
    #define VermogenRechts_CTRL_RESET             (uint8)((uint8)0x01u << VermogenRechts_CTRL_RESET_SHIFT)
    #define VermogenRechts_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << VermogenRechts_CTRL_CMPMODE2_SHIFT)
    #define VermogenRechts_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << VermogenRechts_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define VermogenRechts_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define VermogenRechts_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << VermogenRechts_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define VermogenRechts_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define VermogenRechts_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define VermogenRechts_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define VermogenRechts_STATUS_TC_INT_EN_MASK_SHIFT            (VermogenRechts_STATUS_TC_SHIFT - 4u)
    #define VermogenRechts_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define VermogenRechts_STATUS_CMP1_INT_EN_MASK_SHIFT          (VermogenRechts_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define VermogenRechts_STATUS_TC              (uint8)((uint8)0x01u << VermogenRechts_STATUS_TC_SHIFT)
    #define VermogenRechts_STATUS_CMP1            (uint8)((uint8)0x01u << VermogenRechts_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define VermogenRechts_STATUS_TC_INT_EN_MASK              (uint8)((uint8)VermogenRechts_STATUS_TC >> 4u)
    #define VermogenRechts_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)VermogenRechts_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define VermogenRechts_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define VermogenRechts_RT1_MASK              (uint8)((uint8)0x03u << VermogenRechts_RT1_SHIFT)
    #define VermogenRechts_SYNC                  (uint8)((uint8)0x03u << VermogenRechts_RT1_SHIFT)
    #define VermogenRechts_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define VermogenRechts_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << VermogenRechts_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define VermogenRechts_SYNCDSI_EN            (uint8)((uint8)0x0Fu << VermogenRechts_SYNCDSI_SHIFT)


#else
    #define VermogenRechts_STATUS                (*(reg8 *)   VermogenRechts_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define VermogenRechts_STATUS_PTR            ((reg8 *)    VermogenRechts_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define VermogenRechts_STATUS_MASK           (*(reg8 *)   VermogenRechts_PWMUDB_genblk8_stsreg__MASK_REG)
    #define VermogenRechts_STATUS_MASK_PTR       ((reg8 *)    VermogenRechts_PWMUDB_genblk8_stsreg__MASK_REG)
    #define VermogenRechts_STATUS_AUX_CTRL       (*(reg8 *)   VermogenRechts_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define VermogenRechts_CONTROL               (*(reg8 *)   VermogenRechts_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define VermogenRechts_CONTROL_PTR           ((reg8 *)    VermogenRechts_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define VermogenRechts_CTRL_ENABLE_SHIFT      (0x07u)
    #define VermogenRechts_CTRL_RESET_SHIFT       (0x06u)
    #define VermogenRechts_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define VermogenRechts_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define VermogenRechts_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define VermogenRechts_CTRL_ENABLE            (uint8)((uint8)0x01u << VermogenRechts_CTRL_ENABLE_SHIFT)
    #define VermogenRechts_CTRL_RESET             (uint8)((uint8)0x01u << VermogenRechts_CTRL_RESET_SHIFT)
    #define VermogenRechts_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << VermogenRechts_CTRL_CMPMODE2_SHIFT)
    #define VermogenRechts_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << VermogenRechts_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define VermogenRechts_STATUS_KILL_SHIFT          (0x05u)
    #define VermogenRechts_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define VermogenRechts_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define VermogenRechts_STATUS_TC_SHIFT            (0x02u)
    #define VermogenRechts_STATUS_CMP2_SHIFT          (0x01u)
    #define VermogenRechts_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define VermogenRechts_STATUS_KILL_INT_EN_MASK_SHIFT          (VermogenRechts_STATUS_KILL_SHIFT)
    #define VermogenRechts_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (VermogenRechts_STATUS_FIFONEMPTY_SHIFT)
    #define VermogenRechts_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (VermogenRechts_STATUS_FIFOFULL_SHIFT)
    #define VermogenRechts_STATUS_TC_INT_EN_MASK_SHIFT            (VermogenRechts_STATUS_TC_SHIFT)
    #define VermogenRechts_STATUS_CMP2_INT_EN_MASK_SHIFT          (VermogenRechts_STATUS_CMP2_SHIFT)
    #define VermogenRechts_STATUS_CMP1_INT_EN_MASK_SHIFT          (VermogenRechts_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define VermogenRechts_STATUS_KILL            (uint8)((uint8)0x00u << VermogenRechts_STATUS_KILL_SHIFT )
    #define VermogenRechts_STATUS_FIFOFULL        (uint8)((uint8)0x01u << VermogenRechts_STATUS_FIFOFULL_SHIFT)
    #define VermogenRechts_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << VermogenRechts_STATUS_FIFONEMPTY_SHIFT)
    #define VermogenRechts_STATUS_TC              (uint8)((uint8)0x01u << VermogenRechts_STATUS_TC_SHIFT)
    #define VermogenRechts_STATUS_CMP2            (uint8)((uint8)0x01u << VermogenRechts_STATUS_CMP2_SHIFT)
    #define VermogenRechts_STATUS_CMP1            (uint8)((uint8)0x01u << VermogenRechts_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define VermogenRechts_STATUS_KILL_INT_EN_MASK            (VermogenRechts_STATUS_KILL)
    #define VermogenRechts_STATUS_FIFOFULL_INT_EN_MASK        (VermogenRechts_STATUS_FIFOFULL)
    #define VermogenRechts_STATUS_FIFONEMPTY_INT_EN_MASK      (VermogenRechts_STATUS_FIFONEMPTY)
    #define VermogenRechts_STATUS_TC_INT_EN_MASK              (VermogenRechts_STATUS_TC)
    #define VermogenRechts_STATUS_CMP2_INT_EN_MASK            (VermogenRechts_STATUS_CMP2)
    #define VermogenRechts_STATUS_CMP1_INT_EN_MASK            (VermogenRechts_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define VermogenRechts_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define VermogenRechts_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define VermogenRechts_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define VermogenRechts_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define VermogenRechts_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* VermogenRechts_UsingFixedFunction */

#endif  /* CY_PWM_VermogenRechts_H */


/* [] END OF FILE */
