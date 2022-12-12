/*******************************************************************************
* File Name: VermogenLinks_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "VermogenLinks.h"

static VermogenLinks_backupStruct VermogenLinks_backup;


/*******************************************************************************
* Function Name: VermogenLinks_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  VermogenLinks_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void VermogenLinks_SaveConfig(void) 
{

    #if(!VermogenLinks_UsingFixedFunction)
        #if(!VermogenLinks_PWMModeIsCenterAligned)
            VermogenLinks_backup.PWMPeriod = VermogenLinks_ReadPeriod();
        #endif /* (!VermogenLinks_PWMModeIsCenterAligned) */
        VermogenLinks_backup.PWMUdb = VermogenLinks_ReadCounter();
        #if (VermogenLinks_UseStatus)
            VermogenLinks_backup.InterruptMaskValue = VermogenLinks_STATUS_MASK;
        #endif /* (VermogenLinks_UseStatus) */

        #if(VermogenLinks_DeadBandMode == VermogenLinks__B_PWM__DBM_256_CLOCKS || \
            VermogenLinks_DeadBandMode == VermogenLinks__B_PWM__DBM_2_4_CLOCKS)
            VermogenLinks_backup.PWMdeadBandValue = VermogenLinks_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(VermogenLinks_KillModeMinTime)
             VermogenLinks_backup.PWMKillCounterPeriod = VermogenLinks_ReadKillTime();
        #endif /* (VermogenLinks_KillModeMinTime) */

        #if(VermogenLinks_UseControl)
            VermogenLinks_backup.PWMControlRegister = VermogenLinks_ReadControlRegister();
        #endif /* (VermogenLinks_UseControl) */
    #endif  /* (!VermogenLinks_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: VermogenLinks_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  VermogenLinks_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void VermogenLinks_RestoreConfig(void) 
{
        #if(!VermogenLinks_UsingFixedFunction)
            #if(!VermogenLinks_PWMModeIsCenterAligned)
                VermogenLinks_WritePeriod(VermogenLinks_backup.PWMPeriod);
            #endif /* (!VermogenLinks_PWMModeIsCenterAligned) */

            VermogenLinks_WriteCounter(VermogenLinks_backup.PWMUdb);

            #if (VermogenLinks_UseStatus)
                VermogenLinks_STATUS_MASK = VermogenLinks_backup.InterruptMaskValue;
            #endif /* (VermogenLinks_UseStatus) */

            #if(VermogenLinks_DeadBandMode == VermogenLinks__B_PWM__DBM_256_CLOCKS || \
                VermogenLinks_DeadBandMode == VermogenLinks__B_PWM__DBM_2_4_CLOCKS)
                VermogenLinks_WriteDeadTime(VermogenLinks_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(VermogenLinks_KillModeMinTime)
                VermogenLinks_WriteKillTime(VermogenLinks_backup.PWMKillCounterPeriod);
            #endif /* (VermogenLinks_KillModeMinTime) */

            #if(VermogenLinks_UseControl)
                VermogenLinks_WriteControlRegister(VermogenLinks_backup.PWMControlRegister);
            #endif /* (VermogenLinks_UseControl) */
        #endif  /* (!VermogenLinks_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: VermogenLinks_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  VermogenLinks_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void VermogenLinks_Sleep(void) 
{
    #if(VermogenLinks_UseControl)
        if(VermogenLinks_CTRL_ENABLE == (VermogenLinks_CONTROL & VermogenLinks_CTRL_ENABLE))
        {
            /*Component is enabled */
            VermogenLinks_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            VermogenLinks_backup.PWMEnableState = 0u;
        }
    #endif /* (VermogenLinks_UseControl) */

    /* Stop component */
    VermogenLinks_Stop();

    /* Save registers configuration */
    VermogenLinks_SaveConfig();
}


/*******************************************************************************
* Function Name: VermogenLinks_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  VermogenLinks_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VermogenLinks_Wakeup(void) 
{
     /* Restore registers values */
    VermogenLinks_RestoreConfig();

    if(VermogenLinks_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        VermogenLinks_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
