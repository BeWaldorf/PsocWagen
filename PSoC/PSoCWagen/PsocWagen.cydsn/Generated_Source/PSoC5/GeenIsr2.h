/*******************************************************************************
* File Name: GeenIsr2.h  
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

#if !defined(CY_PINS_GeenIsr2_H) /* Pins GeenIsr2_H */
#define CY_PINS_GeenIsr2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "GeenIsr2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 GeenIsr2__PORT == 15 && ((GeenIsr2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    GeenIsr2_Write(uint8 value);
void    GeenIsr2_SetDriveMode(uint8 mode);
uint8   GeenIsr2_ReadDataReg(void);
uint8   GeenIsr2_Read(void);
void    GeenIsr2_SetInterruptMode(uint16 position, uint16 mode);
uint8   GeenIsr2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the GeenIsr2_SetDriveMode() function.
     *  @{
     */
        #define GeenIsr2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define GeenIsr2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define GeenIsr2_DM_RES_UP          PIN_DM_RES_UP
        #define GeenIsr2_DM_RES_DWN         PIN_DM_RES_DWN
        #define GeenIsr2_DM_OD_LO           PIN_DM_OD_LO
        #define GeenIsr2_DM_OD_HI           PIN_DM_OD_HI
        #define GeenIsr2_DM_STRONG          PIN_DM_STRONG
        #define GeenIsr2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define GeenIsr2_MASK               GeenIsr2__MASK
#define GeenIsr2_SHIFT              GeenIsr2__SHIFT
#define GeenIsr2_WIDTH              1u

/* Interrupt constants */
#if defined(GeenIsr2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in GeenIsr2_SetInterruptMode() function.
     *  @{
     */
        #define GeenIsr2_INTR_NONE      (uint16)(0x0000u)
        #define GeenIsr2_INTR_RISING    (uint16)(0x0001u)
        #define GeenIsr2_INTR_FALLING   (uint16)(0x0002u)
        #define GeenIsr2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define GeenIsr2_INTR_MASK      (0x01u) 
#endif /* (GeenIsr2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define GeenIsr2_PS                     (* (reg8 *) GeenIsr2__PS)
/* Data Register */
#define GeenIsr2_DR                     (* (reg8 *) GeenIsr2__DR)
/* Port Number */
#define GeenIsr2_PRT_NUM                (* (reg8 *) GeenIsr2__PRT) 
/* Connect to Analog Globals */                                                  
#define GeenIsr2_AG                     (* (reg8 *) GeenIsr2__AG)                       
/* Analog MUX bux enable */
#define GeenIsr2_AMUX                   (* (reg8 *) GeenIsr2__AMUX) 
/* Bidirectional Enable */                                                        
#define GeenIsr2_BIE                    (* (reg8 *) GeenIsr2__BIE)
/* Bit-mask for Aliased Register Access */
#define GeenIsr2_BIT_MASK               (* (reg8 *) GeenIsr2__BIT_MASK)
/* Bypass Enable */
#define GeenIsr2_BYP                    (* (reg8 *) GeenIsr2__BYP)
/* Port wide control signals */                                                   
#define GeenIsr2_CTL                    (* (reg8 *) GeenIsr2__CTL)
/* Drive Modes */
#define GeenIsr2_DM0                    (* (reg8 *) GeenIsr2__DM0) 
#define GeenIsr2_DM1                    (* (reg8 *) GeenIsr2__DM1)
#define GeenIsr2_DM2                    (* (reg8 *) GeenIsr2__DM2) 
/* Input Buffer Disable Override */
#define GeenIsr2_INP_DIS                (* (reg8 *) GeenIsr2__INP_DIS)
/* LCD Common or Segment Drive */
#define GeenIsr2_LCD_COM_SEG            (* (reg8 *) GeenIsr2__LCD_COM_SEG)
/* Enable Segment LCD */
#define GeenIsr2_LCD_EN                 (* (reg8 *) GeenIsr2__LCD_EN)
/* Slew Rate Control */
#define GeenIsr2_SLW                    (* (reg8 *) GeenIsr2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define GeenIsr2_PRTDSI__CAPS_SEL       (* (reg8 *) GeenIsr2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define GeenIsr2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) GeenIsr2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define GeenIsr2_PRTDSI__OE_SEL0        (* (reg8 *) GeenIsr2__PRTDSI__OE_SEL0) 
#define GeenIsr2_PRTDSI__OE_SEL1        (* (reg8 *) GeenIsr2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define GeenIsr2_PRTDSI__OUT_SEL0       (* (reg8 *) GeenIsr2__PRTDSI__OUT_SEL0) 
#define GeenIsr2_PRTDSI__OUT_SEL1       (* (reg8 *) GeenIsr2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define GeenIsr2_PRTDSI__SYNC_OUT       (* (reg8 *) GeenIsr2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(GeenIsr2__SIO_CFG)
    #define GeenIsr2_SIO_HYST_EN        (* (reg8 *) GeenIsr2__SIO_HYST_EN)
    #define GeenIsr2_SIO_REG_HIFREQ     (* (reg8 *) GeenIsr2__SIO_REG_HIFREQ)
    #define GeenIsr2_SIO_CFG            (* (reg8 *) GeenIsr2__SIO_CFG)
    #define GeenIsr2_SIO_DIFF           (* (reg8 *) GeenIsr2__SIO_DIFF)
#endif /* (GeenIsr2__SIO_CFG) */

/* Interrupt Registers */
#if defined(GeenIsr2__INTSTAT)
    #define GeenIsr2_INTSTAT            (* (reg8 *) GeenIsr2__INTSTAT)
    #define GeenIsr2_SNAP               (* (reg8 *) GeenIsr2__SNAP)
    
	#define GeenIsr2_0_INTTYPE_REG 		(* (reg8 *) GeenIsr2__0__INTTYPE)
#endif /* (GeenIsr2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_GeenIsr2_H */


/* [] END OF FILE */
