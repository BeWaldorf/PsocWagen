/*******************************************************************************
* File Name: MotorRechts.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_MotorRechts_H) /* Pins MotorRechts_H */
#define CY_PINS_MotorRechts_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MotorRechts_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MotorRechts__PORT == 15 && ((MotorRechts__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MotorRechts_Write(uint8 value);
void    MotorRechts_SetDriveMode(uint8 mode);
uint8   MotorRechts_ReadDataReg(void);
uint8   MotorRechts_Read(void);
void    MotorRechts_SetInterruptMode(uint16 position, uint16 mode);
uint8   MotorRechts_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MotorRechts_SetDriveMode() function.
     *  @{
     */
        #define MotorRechts_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MotorRechts_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MotorRechts_DM_RES_UP          PIN_DM_RES_UP
        #define MotorRechts_DM_RES_DWN         PIN_DM_RES_DWN
        #define MotorRechts_DM_OD_LO           PIN_DM_OD_LO
        #define MotorRechts_DM_OD_HI           PIN_DM_OD_HI
        #define MotorRechts_DM_STRONG          PIN_DM_STRONG
        #define MotorRechts_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MotorRechts_MASK               MotorRechts__MASK
#define MotorRechts_SHIFT              MotorRechts__SHIFT
#define MotorRechts_WIDTH              1u

/* Interrupt constants */
#if defined(MotorRechts__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MotorRechts_SetInterruptMode() function.
     *  @{
     */
        #define MotorRechts_INTR_NONE      (uint16)(0x0000u)
        #define MotorRechts_INTR_RISING    (uint16)(0x0001u)
        #define MotorRechts_INTR_FALLING   (uint16)(0x0002u)
        #define MotorRechts_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MotorRechts_INTR_MASK      (0x01u) 
#endif /* (MotorRechts__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MotorRechts_PS                     (* (reg8 *) MotorRechts__PS)
/* Data Register */
#define MotorRechts_DR                     (* (reg8 *) MotorRechts__DR)
/* Port Number */
#define MotorRechts_PRT_NUM                (* (reg8 *) MotorRechts__PRT) 
/* Connect to Analog Globals */                                                  
#define MotorRechts_AG                     (* (reg8 *) MotorRechts__AG)                       
/* Analog MUX bux enable */
#define MotorRechts_AMUX                   (* (reg8 *) MotorRechts__AMUX) 
/* Bidirectional Enable */                                                        
#define MotorRechts_BIE                    (* (reg8 *) MotorRechts__BIE)
/* Bit-mask for Aliased Register Access */
#define MotorRechts_BIT_MASK               (* (reg8 *) MotorRechts__BIT_MASK)
/* Bypass Enable */
#define MotorRechts_BYP                    (* (reg8 *) MotorRechts__BYP)
/* Port wide control signals */                                                   
#define MotorRechts_CTL                    (* (reg8 *) MotorRechts__CTL)
/* Drive Modes */
#define MotorRechts_DM0                    (* (reg8 *) MotorRechts__DM0) 
#define MotorRechts_DM1                    (* (reg8 *) MotorRechts__DM1)
#define MotorRechts_DM2                    (* (reg8 *) MotorRechts__DM2) 
/* Input Buffer Disable Override */
#define MotorRechts_INP_DIS                (* (reg8 *) MotorRechts__INP_DIS)
/* LCD Common or Segment Drive */
#define MotorRechts_LCD_COM_SEG            (* (reg8 *) MotorRechts__LCD_COM_SEG)
/* Enable Segment LCD */
#define MotorRechts_LCD_EN                 (* (reg8 *) MotorRechts__LCD_EN)
/* Slew Rate Control */
#define MotorRechts_SLW                    (* (reg8 *) MotorRechts__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MotorRechts_PRTDSI__CAPS_SEL       (* (reg8 *) MotorRechts__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MotorRechts_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MotorRechts__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MotorRechts_PRTDSI__OE_SEL0        (* (reg8 *) MotorRechts__PRTDSI__OE_SEL0) 
#define MotorRechts_PRTDSI__OE_SEL1        (* (reg8 *) MotorRechts__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MotorRechts_PRTDSI__OUT_SEL0       (* (reg8 *) MotorRechts__PRTDSI__OUT_SEL0) 
#define MotorRechts_PRTDSI__OUT_SEL1       (* (reg8 *) MotorRechts__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MotorRechts_PRTDSI__SYNC_OUT       (* (reg8 *) MotorRechts__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MotorRechts__SIO_CFG)
    #define MotorRechts_SIO_HYST_EN        (* (reg8 *) MotorRechts__SIO_HYST_EN)
    #define MotorRechts_SIO_REG_HIFREQ     (* (reg8 *) MotorRechts__SIO_REG_HIFREQ)
    #define MotorRechts_SIO_CFG            (* (reg8 *) MotorRechts__SIO_CFG)
    #define MotorRechts_SIO_DIFF           (* (reg8 *) MotorRechts__SIO_DIFF)
#endif /* (MotorRechts__SIO_CFG) */

/* Interrupt Registers */
#if defined(MotorRechts__INTSTAT)
    #define MotorRechts_INTSTAT            (* (reg8 *) MotorRechts__INTSTAT)
    #define MotorRechts_SNAP               (* (reg8 *) MotorRechts__SNAP)
    
	#define MotorRechts_0_INTTYPE_REG 		(* (reg8 *) MotorRechts__0__INTTYPE)
#endif /* (MotorRechts__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MotorRechts_H */


/* [] END OF FILE */
