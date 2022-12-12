/*******************************************************************************
* File Name: MotorLinks.h  
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

#if !defined(CY_PINS_MotorLinks_H) /* Pins MotorLinks_H */
#define CY_PINS_MotorLinks_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MotorLinks_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MotorLinks__PORT == 15 && ((MotorLinks__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MotorLinks_Write(uint8 value);
void    MotorLinks_SetDriveMode(uint8 mode);
uint8   MotorLinks_ReadDataReg(void);
uint8   MotorLinks_Read(void);
void    MotorLinks_SetInterruptMode(uint16 position, uint16 mode);
uint8   MotorLinks_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MotorLinks_SetDriveMode() function.
     *  @{
     */
        #define MotorLinks_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MotorLinks_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MotorLinks_DM_RES_UP          PIN_DM_RES_UP
        #define MotorLinks_DM_RES_DWN         PIN_DM_RES_DWN
        #define MotorLinks_DM_OD_LO           PIN_DM_OD_LO
        #define MotorLinks_DM_OD_HI           PIN_DM_OD_HI
        #define MotorLinks_DM_STRONG          PIN_DM_STRONG
        #define MotorLinks_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MotorLinks_MASK               MotorLinks__MASK
#define MotorLinks_SHIFT              MotorLinks__SHIFT
#define MotorLinks_WIDTH              1u

/* Interrupt constants */
#if defined(MotorLinks__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MotorLinks_SetInterruptMode() function.
     *  @{
     */
        #define MotorLinks_INTR_NONE      (uint16)(0x0000u)
        #define MotorLinks_INTR_RISING    (uint16)(0x0001u)
        #define MotorLinks_INTR_FALLING   (uint16)(0x0002u)
        #define MotorLinks_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MotorLinks_INTR_MASK      (0x01u) 
#endif /* (MotorLinks__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MotorLinks_PS                     (* (reg8 *) MotorLinks__PS)
/* Data Register */
#define MotorLinks_DR                     (* (reg8 *) MotorLinks__DR)
/* Port Number */
#define MotorLinks_PRT_NUM                (* (reg8 *) MotorLinks__PRT) 
/* Connect to Analog Globals */                                                  
#define MotorLinks_AG                     (* (reg8 *) MotorLinks__AG)                       
/* Analog MUX bux enable */
#define MotorLinks_AMUX                   (* (reg8 *) MotorLinks__AMUX) 
/* Bidirectional Enable */                                                        
#define MotorLinks_BIE                    (* (reg8 *) MotorLinks__BIE)
/* Bit-mask for Aliased Register Access */
#define MotorLinks_BIT_MASK               (* (reg8 *) MotorLinks__BIT_MASK)
/* Bypass Enable */
#define MotorLinks_BYP                    (* (reg8 *) MotorLinks__BYP)
/* Port wide control signals */                                                   
#define MotorLinks_CTL                    (* (reg8 *) MotorLinks__CTL)
/* Drive Modes */
#define MotorLinks_DM0                    (* (reg8 *) MotorLinks__DM0) 
#define MotorLinks_DM1                    (* (reg8 *) MotorLinks__DM1)
#define MotorLinks_DM2                    (* (reg8 *) MotorLinks__DM2) 
/* Input Buffer Disable Override */
#define MotorLinks_INP_DIS                (* (reg8 *) MotorLinks__INP_DIS)
/* LCD Common or Segment Drive */
#define MotorLinks_LCD_COM_SEG            (* (reg8 *) MotorLinks__LCD_COM_SEG)
/* Enable Segment LCD */
#define MotorLinks_LCD_EN                 (* (reg8 *) MotorLinks__LCD_EN)
/* Slew Rate Control */
#define MotorLinks_SLW                    (* (reg8 *) MotorLinks__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MotorLinks_PRTDSI__CAPS_SEL       (* (reg8 *) MotorLinks__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MotorLinks_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MotorLinks__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MotorLinks_PRTDSI__OE_SEL0        (* (reg8 *) MotorLinks__PRTDSI__OE_SEL0) 
#define MotorLinks_PRTDSI__OE_SEL1        (* (reg8 *) MotorLinks__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MotorLinks_PRTDSI__OUT_SEL0       (* (reg8 *) MotorLinks__PRTDSI__OUT_SEL0) 
#define MotorLinks_PRTDSI__OUT_SEL1       (* (reg8 *) MotorLinks__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MotorLinks_PRTDSI__SYNC_OUT       (* (reg8 *) MotorLinks__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MotorLinks__SIO_CFG)
    #define MotorLinks_SIO_HYST_EN        (* (reg8 *) MotorLinks__SIO_HYST_EN)
    #define MotorLinks_SIO_REG_HIFREQ     (* (reg8 *) MotorLinks__SIO_REG_HIFREQ)
    #define MotorLinks_SIO_CFG            (* (reg8 *) MotorLinks__SIO_CFG)
    #define MotorLinks_SIO_DIFF           (* (reg8 *) MotorLinks__SIO_DIFF)
#endif /* (MotorLinks__SIO_CFG) */

/* Interrupt Registers */
#if defined(MotorLinks__INTSTAT)
    #define MotorLinks_INTSTAT            (* (reg8 *) MotorLinks__INTSTAT)
    #define MotorLinks_SNAP               (* (reg8 *) MotorLinks__SNAP)
    
	#define MotorLinks_0_INTTYPE_REG 		(* (reg8 *) MotorLinks__0__INTTYPE)
#endif /* (MotorLinks__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MotorLinks_H */


/* [] END OF FILE */
