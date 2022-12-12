/*******************************************************************************
* File Name: Klok.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Klok_H)
#define CY_CLOCK_Klok_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void Klok_Start(void) ;
void Klok_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Klok_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Klok_StandbyPower(uint8 state) ;
void Klok_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Klok_GetDividerRegister(void) ;
void Klok_SetModeRegister(uint8 modeBitMask) ;
void Klok_ClearModeRegister(uint8 modeBitMask) ;
uint8 Klok_GetModeRegister(void) ;
void Klok_SetSourceRegister(uint8 clkSource) ;
uint8 Klok_GetSourceRegister(void) ;
#if defined(Klok__CFG3)
void Klok_SetPhaseRegister(uint8 clkPhase) ;
uint8 Klok_GetPhaseRegister(void) ;
#endif /* defined(Klok__CFG3) */

#define Klok_Enable()                       Klok_Start()
#define Klok_Disable()                      Klok_Stop()
#define Klok_SetDivider(clkDivider)         Klok_SetDividerRegister(clkDivider, 1u)
#define Klok_SetDividerValue(clkDivider)    Klok_SetDividerRegister((clkDivider) - 1u, 1u)
#define Klok_SetMode(clkMode)               Klok_SetModeRegister(clkMode)
#define Klok_SetSource(clkSource)           Klok_SetSourceRegister(clkSource)
#if defined(Klok__CFG3)
#define Klok_SetPhase(clkPhase)             Klok_SetPhaseRegister(clkPhase)
#define Klok_SetPhaseValue(clkPhase)        Klok_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Klok__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Klok_CLKEN              (* (reg8 *) Klok__PM_ACT_CFG)
#define Klok_CLKEN_PTR          ((reg8 *) Klok__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Klok_CLKSTBY            (* (reg8 *) Klok__PM_STBY_CFG)
#define Klok_CLKSTBY_PTR        ((reg8 *) Klok__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Klok_DIV_LSB            (* (reg8 *) Klok__CFG0)
#define Klok_DIV_LSB_PTR        ((reg8 *) Klok__CFG0)
#define Klok_DIV_PTR            ((reg16 *) Klok__CFG0)

/* Clock MSB divider configuration register. */
#define Klok_DIV_MSB            (* (reg8 *) Klok__CFG1)
#define Klok_DIV_MSB_PTR        ((reg8 *) Klok__CFG1)

/* Mode and source configuration register */
#define Klok_MOD_SRC            (* (reg8 *) Klok__CFG2)
#define Klok_MOD_SRC_PTR        ((reg8 *) Klok__CFG2)

#if defined(Klok__CFG3)
/* Analog clock phase configuration register */
#define Klok_PHASE              (* (reg8 *) Klok__CFG3)
#define Klok_PHASE_PTR          ((reg8 *) Klok__CFG3)
#endif /* defined(Klok__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Klok_CLKEN_MASK         Klok__PM_ACT_MSK
#define Klok_CLKSTBY_MASK       Klok__PM_STBY_MSK

/* CFG2 field masks */
#define Klok_SRC_SEL_MSK        Klok__CFG2_SRC_SEL_MASK
#define Klok_MODE_MASK          (~(Klok_SRC_SEL_MSK))

#if defined(Klok__CFG3)
/* CFG3 phase mask */
#define Klok_PHASE_MASK         Klok__CFG3_PHASE_DLY_MASK
#endif /* defined(Klok__CFG3) */

#endif /* CY_CLOCK_Klok_H */


/* [] END OF FILE */
