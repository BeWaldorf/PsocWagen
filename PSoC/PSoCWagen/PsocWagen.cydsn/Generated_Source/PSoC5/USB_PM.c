/*******************************************************************************
* File Name: USB_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USB.h"


/***************************************
* Local data allocation
***************************************/

static USB_BACKUP_STRUCT  USB_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: USB_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the USB_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  USB_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_SaveConfig(void)
{
    #if(USB_CONTROL_REG_REMOVED == 0u)
        USB_backup.cr = USB_CONTROL_REG;
    #endif /* End USB_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: USB_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  USB_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling USB_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void USB_RestoreConfig(void)
{
    #if(USB_CONTROL_REG_REMOVED == 0u)
        USB_CONTROL_REG = USB_backup.cr;
    #endif /* End USB_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: USB_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The USB_Sleep() API saves the current component state. Then it
*  calls the USB_Stop() function and calls 
*  USB_SaveConfig() to save the hardware configuration.
*  Call the USB_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  USB_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Sleep(void)
{
    #if(USB_RX_ENABLED || USB_HD_ENABLED)
        if((USB_RXSTATUS_ACTL_REG  & USB_INT_ENABLE) != 0u)
        {
            USB_backup.enableState = 1u;
        }
        else
        {
            USB_backup.enableState = 0u;
        }
    #else
        if((USB_TXSTATUS_ACTL_REG  & USB_INT_ENABLE) !=0u)
        {
            USB_backup.enableState = 1u;
        }
        else
        {
            USB_backup.enableState = 0u;
        }
    #endif /* End USB_RX_ENABLED || USB_HD_ENABLED*/

    USB_Stop();
    USB_SaveConfig();
}


/*******************************************************************************
* Function Name: USB_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  USB_Sleep() was called. The USB_Wakeup() function
*  calls the USB_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  USB_Sleep() function was called, the USB_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  USB_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Wakeup(void)
{
    USB_RestoreConfig();
    #if( (USB_RX_ENABLED) || (USB_HD_ENABLED) )
        USB_ClearRxBuffer();
    #endif /* End (USB_RX_ENABLED) || (USB_HD_ENABLED) */
    #if(USB_TX_ENABLED || USB_HD_ENABLED)
        USB_ClearTxBuffer();
    #endif /* End USB_TX_ENABLED || USB_HD_ENABLED */

    if(USB_backup.enableState != 0u)
    {
        USB_Enable();
    }
}


/* [] END OF FILE */
