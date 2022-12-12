/*******************************************************************************
* File Name: VermogenRechts_PM.c
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

#include "VermogenRechts.h"

static VermogenRechts_backupStruct VermogenRechts_backup;


/*******************************************************************************
* Function Name: VermogenRechts_SaveConfig
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
*  VermogenRechts_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void VermogenRechts_SaveConfig(void) 
{

    #if(!VermogenRechts_UsingFixedFunction)
        #if(!VermogenRechts_PWMModeIsCenterAligned)
            VermogenRechts_backup.PWMPeriod = VermogenRechts_ReadPeriod();
        #endif /* (!VermogenRechts_PWMModeIsCenterAligned) */
        VermogenRechts_backup.PWMUdb = VermogenRechts_ReadCounter();
        #if (VermogenRechts_UseStatus)
            VermogenRechts_backup.InterruptMaskValue = VermogenRechts_STATUS_MASK;
        #endif /* (VermogenRechts_UseStatus) */

        #if(VermogenRechts_DeadBandMode == VermogenRechts__B_PWM__DBM_256_CLOCKS || \
            VermogenRechts_DeadBandMode == VermogenRechts__B_PWM__DBM_2_4_CLOCKS)
            VermogenRechts_backup.PWMdeadBandValue = VermogenRechts_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(VermogenRechts_KillModeMinTime)
             VermogenRechts_backup.PWMKillCounterPeriod = VermogenRechts_ReadKillTime();
        #endif /* (VermogenRechts_KillModeMinTime) */

        #if(VermogenRechts_UseControl)
            VermogenRechts_backup.PWMControlRegister = VermogenRechts_ReadControlRegister();
        #endif /* (VermogenRechts_UseControl) */
    #endif  /* (!VermogenRechts_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: VermogenRechts_RestoreConfig
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
*  VermogenRechts_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void VermogenRechts_RestoreConfig(void) 
{
        #if(!VermogenRechts_UsingFixedFunction)
            #if(!VermogenRechts_PWMModeIsCenterAligned)
                VermogenRechts_WritePeriod(VermogenRechts_backup.PWMPeriod);
            #endif /* (!VermogenRechts_PWMModeIsCenterAligned) */

            VermogenRechts_WriteCounter(VermogenRechts_backup.PWMUdb);

            #if (VermogenRechts_UseStatus)
                VermogenRechts_STATUS_MASK = VermogenRechts_backup.InterruptMaskValue;
            #endif /* (VermogenRechts_UseStatus) */

            #if(VermogenRechts_DeadBandMode == VermogenRechts__B_PWM__DBM_256_CLOCKS || \
                VermogenRechts_DeadBandMode == VermogenRechts__B_PWM__DBM_2_4_CLOCKS)
                VermogenRechts_WriteDeadTime(VermogenRechts_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(VermogenRechts_KillModeMinTime)
                VermogenRechts_WriteKillTime(VermogenRechts_backup.PWMKillCounterPeriod);
            #endif /* (VermogenRechts_KillModeMinTime) */

            #if(VermogenRechts_UseControl)
                VermogenRechts_WriteControlRegister(VermogenRechts_backup.PWMControlRegister);
            #endif /* (VermogenRechts_UseControl) */
        #endif  /* (!VermogenRechts_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: VermogenRechts_Sleep
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
*  VermogenRechts_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void VermogenRechts_Sleep(void) 
{
    #if(VermogenRechts_UseControl)
        if(VermogenRechts_CTRL_ENABLE == (VermogenRechts_CONTROL & VermogenRechts_CTRL_ENABLE))
        {
            /*Component is enabled */
            VermogenRechts_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            VermogenRechts_backup.PWMEnableState = 0u;
        }
    #endif /* (VermogenRechts_UseControl) */

    /* Stop component */
    VermogenRechts_Stop();

    /* Save registers configuration */
    VermogenRechts_SaveConfig();
}


/*******************************************************************************
* Function Name: VermogenRechts_Wakeup
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
*  VermogenRechts_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VermogenRechts_Wakeup(void) 
{
     /* Restore registers values */
    VermogenRechts_RestoreConfig();

    if(VermogenRechts_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        VermogenRechts_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
