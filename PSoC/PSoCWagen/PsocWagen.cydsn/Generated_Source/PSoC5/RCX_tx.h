/*******************************************************************************
* File Name: RCX_tx.h  
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

#if !defined(CY_PINS_RCX_tx_H) /* Pins RCX_tx_H */
#define CY_PINS_RCX_tx_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RCX_tx_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 RCX_tx__PORT == 15 && ((RCX_tx__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    RCX_tx_Write(uint8 value);
void    RCX_tx_SetDriveMode(uint8 mode);
uint8   RCX_tx_ReadDataReg(void);
uint8   RCX_tx_Read(void);
void    RCX_tx_SetInterruptMode(uint16 position, uint16 mode);
uint8   RCX_tx_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the RCX_tx_SetDriveMode() function.
     *  @{
     */
        #define RCX_tx_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define RCX_tx_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define RCX_tx_DM_RES_UP          PIN_DM_RES_UP
        #define RCX_tx_DM_RES_DWN         PIN_DM_RES_DWN
        #define RCX_tx_DM_OD_LO           PIN_DM_OD_LO
        #define RCX_tx_DM_OD_HI           PIN_DM_OD_HI
        #define RCX_tx_DM_STRONG          PIN_DM_STRONG
        #define RCX_tx_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define RCX_tx_MASK               RCX_tx__MASK
#define RCX_tx_SHIFT              RCX_tx__SHIFT
#define RCX_tx_WIDTH              1u

/* Interrupt constants */
#if defined(RCX_tx__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RCX_tx_SetInterruptMode() function.
     *  @{
     */
        #define RCX_tx_INTR_NONE      (uint16)(0x0000u)
        #define RCX_tx_INTR_RISING    (uint16)(0x0001u)
        #define RCX_tx_INTR_FALLING   (uint16)(0x0002u)
        #define RCX_tx_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define RCX_tx_INTR_MASK      (0x01u) 
#endif /* (RCX_tx__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RCX_tx_PS                     (* (reg8 *) RCX_tx__PS)
/* Data Register */
#define RCX_tx_DR                     (* (reg8 *) RCX_tx__DR)
/* Port Number */
#define RCX_tx_PRT_NUM                (* (reg8 *) RCX_tx__PRT) 
/* Connect to Analog Globals */                                                  
#define RCX_tx_AG                     (* (reg8 *) RCX_tx__AG)                       
/* Analog MUX bux enable */
#define RCX_tx_AMUX                   (* (reg8 *) RCX_tx__AMUX) 
/* Bidirectional Enable */                                                        
#define RCX_tx_BIE                    (* (reg8 *) RCX_tx__BIE)
/* Bit-mask for Aliased Register Access */
#define RCX_tx_BIT_MASK               (* (reg8 *) RCX_tx__BIT_MASK)
/* Bypass Enable */
#define RCX_tx_BYP                    (* (reg8 *) RCX_tx__BYP)
/* Port wide control signals */                                                   
#define RCX_tx_CTL                    (* (reg8 *) RCX_tx__CTL)
/* Drive Modes */
#define RCX_tx_DM0                    (* (reg8 *) RCX_tx__DM0) 
#define RCX_tx_DM1                    (* (reg8 *) RCX_tx__DM1)
#define RCX_tx_DM2                    (* (reg8 *) RCX_tx__DM2) 
/* Input Buffer Disable Override */
#define RCX_tx_INP_DIS                (* (reg8 *) RCX_tx__INP_DIS)
/* LCD Common or Segment Drive */
#define RCX_tx_LCD_COM_SEG            (* (reg8 *) RCX_tx__LCD_COM_SEG)
/* Enable Segment LCD */
#define RCX_tx_LCD_EN                 (* (reg8 *) RCX_tx__LCD_EN)
/* Slew Rate Control */
#define RCX_tx_SLW                    (* (reg8 *) RCX_tx__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RCX_tx_PRTDSI__CAPS_SEL       (* (reg8 *) RCX_tx__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RCX_tx_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RCX_tx__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RCX_tx_PRTDSI__OE_SEL0        (* (reg8 *) RCX_tx__PRTDSI__OE_SEL0) 
#define RCX_tx_PRTDSI__OE_SEL1        (* (reg8 *) RCX_tx__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RCX_tx_PRTDSI__OUT_SEL0       (* (reg8 *) RCX_tx__PRTDSI__OUT_SEL0) 
#define RCX_tx_PRTDSI__OUT_SEL1       (* (reg8 *) RCX_tx__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RCX_tx_PRTDSI__SYNC_OUT       (* (reg8 *) RCX_tx__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(RCX_tx__SIO_CFG)
    #define RCX_tx_SIO_HYST_EN        (* (reg8 *) RCX_tx__SIO_HYST_EN)
    #define RCX_tx_SIO_REG_HIFREQ     (* (reg8 *) RCX_tx__SIO_REG_HIFREQ)
    #define RCX_tx_SIO_CFG            (* (reg8 *) RCX_tx__SIO_CFG)
    #define RCX_tx_SIO_DIFF           (* (reg8 *) RCX_tx__SIO_DIFF)
#endif /* (RCX_tx__SIO_CFG) */

/* Interrupt Registers */
#if defined(RCX_tx__INTSTAT)
    #define RCX_tx_INTSTAT            (* (reg8 *) RCX_tx__INTSTAT)
    #define RCX_tx_SNAP               (* (reg8 *) RCX_tx__SNAP)
    
	#define RCX_tx_0_INTTYPE_REG 		(* (reg8 *) RCX_tx__0__INTTYPE)
#endif /* (RCX_tx__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_RCX_tx_H */


/* [] END OF FILE */
