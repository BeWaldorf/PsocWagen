/*******************************************************************************
* File Name: GeenIsr0.h  
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

#if !defined(CY_PINS_GeenIsr0_H) /* Pins GeenIsr0_H */
#define CY_PINS_GeenIsr0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "GeenIsr0_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 GeenIsr0__PORT == 15 && ((GeenIsr0__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    GeenIsr0_Write(uint8 value);
void    GeenIsr0_SetDriveMode(uint8 mode);
uint8   GeenIsr0_ReadDataReg(void);
uint8   GeenIsr0_Read(void);
void    GeenIsr0_SetInterruptMode(uint16 position, uint16 mode);
uint8   GeenIsr0_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the GeenIsr0_SetDriveMode() function.
     *  @{
     */
        #define GeenIsr0_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define GeenIsr0_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define GeenIsr0_DM_RES_UP          PIN_DM_RES_UP
        #define GeenIsr0_DM_RES_DWN         PIN_DM_RES_DWN
        #define GeenIsr0_DM_OD_LO           PIN_DM_OD_LO
        #define GeenIsr0_DM_OD_HI           PIN_DM_OD_HI
        #define GeenIsr0_DM_STRONG          PIN_DM_STRONG
        #define GeenIsr0_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define GeenIsr0_MASK               GeenIsr0__MASK
#define GeenIsr0_SHIFT              GeenIsr0__SHIFT
#define GeenIsr0_WIDTH              1u

/* Interrupt constants */
#if defined(GeenIsr0__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in GeenIsr0_SetInterruptMode() function.
     *  @{
     */
        #define GeenIsr0_INTR_NONE      (uint16)(0x0000u)
        #define GeenIsr0_INTR_RISING    (uint16)(0x0001u)
        #define GeenIsr0_INTR_FALLING   (uint16)(0x0002u)
        #define GeenIsr0_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define GeenIsr0_INTR_MASK      (0x01u) 
#endif /* (GeenIsr0__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define GeenIsr0_PS                     (* (reg8 *) GeenIsr0__PS)
/* Data Register */
#define GeenIsr0_DR                     (* (reg8 *) GeenIsr0__DR)
/* Port Number */
#define GeenIsr0_PRT_NUM                (* (reg8 *) GeenIsr0__PRT) 
/* Connect to Analog Globals */                                                  
#define GeenIsr0_AG                     (* (reg8 *) GeenIsr0__AG)                       
/* Analog MUX bux enable */
#define GeenIsr0_AMUX                   (* (reg8 *) GeenIsr0__AMUX) 
/* Bidirectional Enable */                                                        
#define GeenIsr0_BIE                    (* (reg8 *) GeenIsr0__BIE)
/* Bit-mask for Aliased Register Access */
#define GeenIsr0_BIT_MASK               (* (reg8 *) GeenIsr0__BIT_MASK)
/* Bypass Enable */
#define GeenIsr0_BYP                    (* (reg8 *) GeenIsr0__BYP)
/* Port wide control signals */                                                   
#define GeenIsr0_CTL                    (* (reg8 *) GeenIsr0__CTL)
/* Drive Modes */
#define GeenIsr0_DM0                    (* (reg8 *) GeenIsr0__DM0) 
#define GeenIsr0_DM1                    (* (reg8 *) GeenIsr0__DM1)
#define GeenIsr0_DM2                    (* (reg8 *) GeenIsr0__DM2) 
/* Input Buffer Disable Override */
#define GeenIsr0_INP_DIS                (* (reg8 *) GeenIsr0__INP_DIS)
/* LCD Common or Segment Drive */
#define GeenIsr0_LCD_COM_SEG            (* (reg8 *) GeenIsr0__LCD_COM_SEG)
/* Enable Segment LCD */
#define GeenIsr0_LCD_EN                 (* (reg8 *) GeenIsr0__LCD_EN)
/* Slew Rate Control */
#define GeenIsr0_SLW                    (* (reg8 *) GeenIsr0__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define GeenIsr0_PRTDSI__CAPS_SEL       (* (reg8 *) GeenIsr0__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define GeenIsr0_PRTDSI__DBL_SYNC_IN    (* (reg8 *) GeenIsr0__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define GeenIsr0_PRTDSI__OE_SEL0        (* (reg8 *) GeenIsr0__PRTDSI__OE_SEL0) 
#define GeenIsr0_PRTDSI__OE_SEL1        (* (reg8 *) GeenIsr0__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define GeenIsr0_PRTDSI__OUT_SEL0       (* (reg8 *) GeenIsr0__PRTDSI__OUT_SEL0) 
#define GeenIsr0_PRTDSI__OUT_SEL1       (* (reg8 *) GeenIsr0__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define GeenIsr0_PRTDSI__SYNC_OUT       (* (reg8 *) GeenIsr0__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(GeenIsr0__SIO_CFG)
    #define GeenIsr0_SIO_HYST_EN        (* (reg8 *) GeenIsr0__SIO_HYST_EN)
    #define GeenIsr0_SIO_REG_HIFREQ     (* (reg8 *) GeenIsr0__SIO_REG_HIFREQ)
    #define GeenIsr0_SIO_CFG            (* (reg8 *) GeenIsr0__SIO_CFG)
    #define GeenIsr0_SIO_DIFF           (* (reg8 *) GeenIsr0__SIO_DIFF)
#endif /* (GeenIsr0__SIO_CFG) */

/* Interrupt Registers */
#if defined(GeenIsr0__INTSTAT)
    #define GeenIsr0_INTSTAT            (* (reg8 *) GeenIsr0__INTSTAT)
    #define GeenIsr0_SNAP               (* (reg8 *) GeenIsr0__SNAP)
    
	#define GeenIsr0_0_INTTYPE_REG 		(* (reg8 *) GeenIsr0__0__INTTYPE)
#endif /* (GeenIsr0__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_GeenIsr0_H */


/* [] END OF FILE */
