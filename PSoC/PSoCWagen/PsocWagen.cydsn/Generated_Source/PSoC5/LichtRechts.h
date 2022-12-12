/*******************************************************************************
* File Name: LichtRechts.h  
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

#if !defined(CY_PINS_LichtRechts_H) /* Pins LichtRechts_H */
#define CY_PINS_LichtRechts_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LichtRechts_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 LichtRechts__PORT == 15 && ((LichtRechts__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    LichtRechts_Write(uint8 value);
void    LichtRechts_SetDriveMode(uint8 mode);
uint8   LichtRechts_ReadDataReg(void);
uint8   LichtRechts_Read(void);
void    LichtRechts_SetInterruptMode(uint16 position, uint16 mode);
uint8   LichtRechts_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the LichtRechts_SetDriveMode() function.
     *  @{
     */
        #define LichtRechts_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define LichtRechts_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define LichtRechts_DM_RES_UP          PIN_DM_RES_UP
        #define LichtRechts_DM_RES_DWN         PIN_DM_RES_DWN
        #define LichtRechts_DM_OD_LO           PIN_DM_OD_LO
        #define LichtRechts_DM_OD_HI           PIN_DM_OD_HI
        #define LichtRechts_DM_STRONG          PIN_DM_STRONG
        #define LichtRechts_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define LichtRechts_MASK               LichtRechts__MASK
#define LichtRechts_SHIFT              LichtRechts__SHIFT
#define LichtRechts_WIDTH              1u

/* Interrupt constants */
#if defined(LichtRechts__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LichtRechts_SetInterruptMode() function.
     *  @{
     */
        #define LichtRechts_INTR_NONE      (uint16)(0x0000u)
        #define LichtRechts_INTR_RISING    (uint16)(0x0001u)
        #define LichtRechts_INTR_FALLING   (uint16)(0x0002u)
        #define LichtRechts_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define LichtRechts_INTR_MASK      (0x01u) 
#endif /* (LichtRechts__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LichtRechts_PS                     (* (reg8 *) LichtRechts__PS)
/* Data Register */
#define LichtRechts_DR                     (* (reg8 *) LichtRechts__DR)
/* Port Number */
#define LichtRechts_PRT_NUM                (* (reg8 *) LichtRechts__PRT) 
/* Connect to Analog Globals */                                                  
#define LichtRechts_AG                     (* (reg8 *) LichtRechts__AG)                       
/* Analog MUX bux enable */
#define LichtRechts_AMUX                   (* (reg8 *) LichtRechts__AMUX) 
/* Bidirectional Enable */                                                        
#define LichtRechts_BIE                    (* (reg8 *) LichtRechts__BIE)
/* Bit-mask for Aliased Register Access */
#define LichtRechts_BIT_MASK               (* (reg8 *) LichtRechts__BIT_MASK)
/* Bypass Enable */
#define LichtRechts_BYP                    (* (reg8 *) LichtRechts__BYP)
/* Port wide control signals */                                                   
#define LichtRechts_CTL                    (* (reg8 *) LichtRechts__CTL)
/* Drive Modes */
#define LichtRechts_DM0                    (* (reg8 *) LichtRechts__DM0) 
#define LichtRechts_DM1                    (* (reg8 *) LichtRechts__DM1)
#define LichtRechts_DM2                    (* (reg8 *) LichtRechts__DM2) 
/* Input Buffer Disable Override */
#define LichtRechts_INP_DIS                (* (reg8 *) LichtRechts__INP_DIS)
/* LCD Common or Segment Drive */
#define LichtRechts_LCD_COM_SEG            (* (reg8 *) LichtRechts__LCD_COM_SEG)
/* Enable Segment LCD */
#define LichtRechts_LCD_EN                 (* (reg8 *) LichtRechts__LCD_EN)
/* Slew Rate Control */
#define LichtRechts_SLW                    (* (reg8 *) LichtRechts__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LichtRechts_PRTDSI__CAPS_SEL       (* (reg8 *) LichtRechts__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define LichtRechts_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LichtRechts__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define LichtRechts_PRTDSI__OE_SEL0        (* (reg8 *) LichtRechts__PRTDSI__OE_SEL0) 
#define LichtRechts_PRTDSI__OE_SEL1        (* (reg8 *) LichtRechts__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define LichtRechts_PRTDSI__OUT_SEL0       (* (reg8 *) LichtRechts__PRTDSI__OUT_SEL0) 
#define LichtRechts_PRTDSI__OUT_SEL1       (* (reg8 *) LichtRechts__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define LichtRechts_PRTDSI__SYNC_OUT       (* (reg8 *) LichtRechts__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(LichtRechts__SIO_CFG)
    #define LichtRechts_SIO_HYST_EN        (* (reg8 *) LichtRechts__SIO_HYST_EN)
    #define LichtRechts_SIO_REG_HIFREQ     (* (reg8 *) LichtRechts__SIO_REG_HIFREQ)
    #define LichtRechts_SIO_CFG            (* (reg8 *) LichtRechts__SIO_CFG)
    #define LichtRechts_SIO_DIFF           (* (reg8 *) LichtRechts__SIO_DIFF)
#endif /* (LichtRechts__SIO_CFG) */

/* Interrupt Registers */
#if defined(LichtRechts__INTSTAT)
    #define LichtRechts_INTSTAT            (* (reg8 *) LichtRechts__INTSTAT)
    #define LichtRechts_SNAP               (* (reg8 *) LichtRechts__SNAP)
    
	#define LichtRechts_0_INTTYPE_REG 		(* (reg8 *) LichtRechts__0__INTTYPE)
#endif /* (LichtRechts__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_LichtRechts_H */


/* [] END OF FILE */
