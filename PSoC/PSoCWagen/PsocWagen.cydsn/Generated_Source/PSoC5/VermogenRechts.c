/*******************************************************************************
* File Name: VermogenRechts.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "VermogenRechts.h"

/* Error message for removed <resource> through optimization */
#ifdef VermogenRechts_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* VermogenRechts_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 VermogenRechts_initVar = 0u;


/*******************************************************************************
* Function Name: VermogenRechts_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  VermogenRechts_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void VermogenRechts_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(VermogenRechts_initVar == 0u)
    {
        VermogenRechts_Init();
        VermogenRechts_initVar = 1u;
    }
    VermogenRechts_Enable();

}


/*******************************************************************************
* Function Name: VermogenRechts_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  VermogenRechts_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void VermogenRechts_Init(void) 
{
    #if (VermogenRechts_UsingFixedFunction || VermogenRechts_UseControl)
        uint8 ctrl;
    #endif /* (VermogenRechts_UsingFixedFunction || VermogenRechts_UseControl) */

    #if(!VermogenRechts_UsingFixedFunction)
        #if(VermogenRechts_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 VermogenRechts_interruptState;
        #endif /* (VermogenRechts_UseStatus) */
    #endif /* (!VermogenRechts_UsingFixedFunction) */

    #if (VermogenRechts_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        VermogenRechts_CONTROL |= VermogenRechts_CFG0_MODE;
        #if (VermogenRechts_DeadBand2_4)
            VermogenRechts_CONTROL |= VermogenRechts_CFG0_DB;
        #endif /* (VermogenRechts_DeadBand2_4) */

        ctrl = VermogenRechts_CONTROL3 & ((uint8 )(~VermogenRechts_CTRL_CMPMODE1_MASK));
        VermogenRechts_CONTROL3 = ctrl | VermogenRechts_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        VermogenRechts_RT1 &= ((uint8)(~VermogenRechts_RT1_MASK));
        VermogenRechts_RT1 |= VermogenRechts_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        VermogenRechts_RT1 &= ((uint8)(~VermogenRechts_SYNCDSI_MASK));
        VermogenRechts_RT1 |= VermogenRechts_SYNCDSI_EN;

    #elif (VermogenRechts_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = VermogenRechts_CONTROL & ((uint8)(~VermogenRechts_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~VermogenRechts_CTRL_CMPMODE1_MASK));
        VermogenRechts_CONTROL = ctrl | VermogenRechts_DEFAULT_COMPARE2_MODE |
                                   VermogenRechts_DEFAULT_COMPARE1_MODE;
    #endif /* (VermogenRechts_UsingFixedFunction) */

    #if (!VermogenRechts_UsingFixedFunction)
        #if (VermogenRechts_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            VermogenRechts_AUX_CONTROLDP0 |= (VermogenRechts_AUX_CTRL_FIFO0_CLR);
        #else /* (VermogenRechts_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            VermogenRechts_AUX_CONTROLDP0 |= (VermogenRechts_AUX_CTRL_FIFO0_CLR);
            VermogenRechts_AUX_CONTROLDP1 |= (VermogenRechts_AUX_CTRL_FIFO0_CLR);
        #endif /* (VermogenRechts_Resolution == 8) */

        VermogenRechts_WriteCounter(VermogenRechts_INIT_PERIOD_VALUE);
    #endif /* (!VermogenRechts_UsingFixedFunction) */

    VermogenRechts_WritePeriod(VermogenRechts_INIT_PERIOD_VALUE);

        #if (VermogenRechts_UseOneCompareMode)
            VermogenRechts_WriteCompare(VermogenRechts_INIT_COMPARE_VALUE1);
        #else
            VermogenRechts_WriteCompare1(VermogenRechts_INIT_COMPARE_VALUE1);
            VermogenRechts_WriteCompare2(VermogenRechts_INIT_COMPARE_VALUE2);
        #endif /* (VermogenRechts_UseOneCompareMode) */

        #if (VermogenRechts_KillModeMinTime)
            VermogenRechts_WriteKillTime(VermogenRechts_MinimumKillTime);
        #endif /* (VermogenRechts_KillModeMinTime) */

        #if (VermogenRechts_DeadBandUsed)
            VermogenRechts_WriteDeadTime(VermogenRechts_INIT_DEAD_TIME);
        #endif /* (VermogenRechts_DeadBandUsed) */

    #if (VermogenRechts_UseStatus || VermogenRechts_UsingFixedFunction)
        VermogenRechts_SetInterruptMode(VermogenRechts_INIT_INTERRUPTS_MODE);
    #endif /* (VermogenRechts_UseStatus || VermogenRechts_UsingFixedFunction) */

    #if (VermogenRechts_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        VermogenRechts_GLOBAL_ENABLE |= VermogenRechts_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        VermogenRechts_CONTROL2 |= VermogenRechts_CTRL2_IRQ_SEL;
    #else
        #if(VermogenRechts_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            VermogenRechts_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            VermogenRechts_STATUS_AUX_CTRL |= VermogenRechts_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(VermogenRechts_interruptState);

            /* Clear the FIFO to enable the VermogenRechts_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            VermogenRechts_ClearFIFO();
        #endif /* (VermogenRechts_UseStatus) */
    #endif /* (VermogenRechts_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: VermogenRechts_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void VermogenRechts_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (VermogenRechts_UsingFixedFunction)
        VermogenRechts_GLOBAL_ENABLE |= VermogenRechts_BLOCK_EN_MASK;
        VermogenRechts_GLOBAL_STBY_ENABLE |= VermogenRechts_BLOCK_STBY_EN_MASK;
    #endif /* (VermogenRechts_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (VermogenRechts_UseControl || VermogenRechts_UsingFixedFunction)
        VermogenRechts_CONTROL |= VermogenRechts_CTRL_ENABLE;
    #endif /* (VermogenRechts_UseControl || VermogenRechts_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: VermogenRechts_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void VermogenRechts_Stop(void) 
{
    #if (VermogenRechts_UseControl || VermogenRechts_UsingFixedFunction)
        VermogenRechts_CONTROL &= ((uint8)(~VermogenRechts_CTRL_ENABLE));
    #endif /* (VermogenRechts_UseControl || VermogenRechts_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (VermogenRechts_UsingFixedFunction)
        VermogenRechts_GLOBAL_ENABLE &= ((uint8)(~VermogenRechts_BLOCK_EN_MASK));
        VermogenRechts_GLOBAL_STBY_ENABLE &= ((uint8)(~VermogenRechts_BLOCK_STBY_EN_MASK));
    #endif /* (VermogenRechts_UsingFixedFunction) */
}

#if (VermogenRechts_UseOneCompareMode)
    #if (VermogenRechts_CompareMode1SW)


        /*******************************************************************************
        * Function Name: VermogenRechts_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void VermogenRechts_SetCompareMode(uint8 comparemode) 
        {
            #if(VermogenRechts_UsingFixedFunction)

                #if(0 != VermogenRechts_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << VermogenRechts_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != VermogenRechts_CTRL_CMPMODE1_SHIFT) */

                VermogenRechts_CONTROL3 &= ((uint8)(~VermogenRechts_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                VermogenRechts_CONTROL3 |= comparemodemasked;

            #elif (VermogenRechts_UseControl)

                #if(0 != VermogenRechts_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << VermogenRechts_CTRL_CMPMODE1_SHIFT)) &
                                                VermogenRechts_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & VermogenRechts_CTRL_CMPMODE1_MASK;
                #endif /* (0 != VermogenRechts_CTRL_CMPMODE1_SHIFT) */

                #if(0 != VermogenRechts_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << VermogenRechts_CTRL_CMPMODE2_SHIFT)) &
                                               VermogenRechts_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & VermogenRechts_CTRL_CMPMODE2_MASK;
                #endif /* (0 != VermogenRechts_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                VermogenRechts_CONTROL &= ((uint8)(~(VermogenRechts_CTRL_CMPMODE1_MASK |
                                            VermogenRechts_CTRL_CMPMODE2_MASK)));
                VermogenRechts_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (VermogenRechts_UsingFixedFunction) */
        }
    #endif /* VermogenRechts_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (VermogenRechts_CompareMode1SW)


        /*******************************************************************************
        * Function Name: VermogenRechts_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void VermogenRechts_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != VermogenRechts_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << VermogenRechts_CTRL_CMPMODE1_SHIFT)) &
                                           VermogenRechts_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & VermogenRechts_CTRL_CMPMODE1_MASK;
            #endif /* (0 != VermogenRechts_CTRL_CMPMODE1_SHIFT) */

            #if (VermogenRechts_UseControl)
                VermogenRechts_CONTROL &= ((uint8)(~VermogenRechts_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                VermogenRechts_CONTROL |= comparemodemasked;
            #endif /* (VermogenRechts_UseControl) */
        }
    #endif /* VermogenRechts_CompareMode1SW */

#if (VermogenRechts_CompareMode2SW)


    /*******************************************************************************
    * Function Name: VermogenRechts_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void VermogenRechts_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != VermogenRechts_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << VermogenRechts_CTRL_CMPMODE2_SHIFT)) &
                                                 VermogenRechts_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & VermogenRechts_CTRL_CMPMODE2_MASK;
        #endif /* (0 != VermogenRechts_CTRL_CMPMODE2_SHIFT) */

        #if (VermogenRechts_UseControl)
            VermogenRechts_CONTROL &= ((uint8)(~VermogenRechts_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            VermogenRechts_CONTROL |= comparemodemasked;
        #endif /* (VermogenRechts_UseControl) */
    }
    #endif /*VermogenRechts_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!VermogenRechts_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: VermogenRechts_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void VermogenRechts_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(VermogenRechts_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: VermogenRechts_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 VermogenRechts_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(VermogenRechts_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(VermogenRechts_CAPTURE_LSB_PTR));
    }

    #if (VermogenRechts_UseStatus)


        /*******************************************************************************
        * Function Name: VermogenRechts_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void VermogenRechts_ClearFIFO(void) 
        {
            while(0u != (VermogenRechts_ReadStatusRegister() & VermogenRechts_STATUS_FIFONEMPTY))
            {
                (void)VermogenRechts_ReadCapture();
            }
        }

    #endif /* VermogenRechts_UseStatus */

#endif /* !VermogenRechts_UsingFixedFunction */


/*******************************************************************************
* Function Name: VermogenRechts_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void VermogenRechts_WritePeriod(uint8 period) 
{
    #if(VermogenRechts_UsingFixedFunction)
        CY_SET_REG16(VermogenRechts_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(VermogenRechts_PERIOD_LSB_PTR, period);
    #endif /* (VermogenRechts_UsingFixedFunction) */
}

#if (VermogenRechts_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: VermogenRechts_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void VermogenRechts_WriteCompare(uint8 compare) \
                                       
    {
        #if(VermogenRechts_UsingFixedFunction)
            CY_SET_REG16(VermogenRechts_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(VermogenRechts_COMPARE1_LSB_PTR, compare);
        #endif /* (VermogenRechts_UsingFixedFunction) */

        #if (VermogenRechts_PWMMode == VermogenRechts__B_PWM__DITHER)
            #if(VermogenRechts_UsingFixedFunction)
                CY_SET_REG16(VermogenRechts_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(VermogenRechts_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (VermogenRechts_UsingFixedFunction) */
        #endif /* (VermogenRechts_PWMMode == VermogenRechts__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: VermogenRechts_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void VermogenRechts_WriteCompare1(uint8 compare) \
                                        
    {
        #if(VermogenRechts_UsingFixedFunction)
            CY_SET_REG16(VermogenRechts_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(VermogenRechts_COMPARE1_LSB_PTR, compare);
        #endif /* (VermogenRechts_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: VermogenRechts_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void VermogenRechts_WriteCompare2(uint8 compare) \
                                        
    {
        #if(VermogenRechts_UsingFixedFunction)
            CY_SET_REG16(VermogenRechts_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(VermogenRechts_COMPARE2_LSB_PTR, compare);
        #endif /* (VermogenRechts_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (VermogenRechts_DeadBandUsed)


    /*******************************************************************************
    * Function Name: VermogenRechts_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void VermogenRechts_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!VermogenRechts_DeadBand2_4)
            CY_SET_REG8(VermogenRechts_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            VermogenRechts_DEADBAND_COUNT &= ((uint8)(~VermogenRechts_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(VermogenRechts_DEADBAND_COUNT_SHIFT)
                VermogenRechts_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << VermogenRechts_DEADBAND_COUNT_SHIFT)) &
                                                    VermogenRechts_DEADBAND_COUNT_MASK;
            #else
                VermogenRechts_DEADBAND_COUNT |= deadtime & VermogenRechts_DEADBAND_COUNT_MASK;
            #endif /* (VermogenRechts_DEADBAND_COUNT_SHIFT) */

        #endif /* (!VermogenRechts_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: VermogenRechts_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 VermogenRechts_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!VermogenRechts_DeadBand2_4)
            return (CY_GET_REG8(VermogenRechts_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(VermogenRechts_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(VermogenRechts_DEADBAND_COUNT & VermogenRechts_DEADBAND_COUNT_MASK)) >>
                                                                           VermogenRechts_DEADBAND_COUNT_SHIFT));
            #else
                return (VermogenRechts_DEADBAND_COUNT & VermogenRechts_DEADBAND_COUNT_MASK);
            #endif /* (VermogenRechts_DEADBAND_COUNT_SHIFT) */
        #endif /* (!VermogenRechts_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (VermogenRechts_UseStatus || VermogenRechts_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: VermogenRechts_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void VermogenRechts_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(VermogenRechts_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: VermogenRechts_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 VermogenRechts_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(VermogenRechts_STATUS_PTR));
    }

#endif /* (VermogenRechts_UseStatus || VermogenRechts_UsingFixedFunction) */


#if (VermogenRechts_UseControl)


    /*******************************************************************************
    * Function Name: VermogenRechts_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 VermogenRechts_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(VermogenRechts_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: VermogenRechts_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void VermogenRechts_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(VermogenRechts_CONTROL_PTR, control);
    }

#endif /* (VermogenRechts_UseControl) */


#if (!VermogenRechts_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: VermogenRechts_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint8 VermogenRechts_ReadCapture(void) 
    {
        return (CY_GET_REG8(VermogenRechts_CAPTURE_LSB_PTR));
    }

#endif /* (!VermogenRechts_UsingFixedFunction) */


#if (VermogenRechts_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: VermogenRechts_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint8 VermogenRechts_ReadCompare(void) 
    {
        #if(VermogenRechts_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(VermogenRechts_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(VermogenRechts_COMPARE1_LSB_PTR));
        #endif /* (VermogenRechts_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: VermogenRechts_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 VermogenRechts_ReadCompare1(void) 
    {
        return (CY_GET_REG8(VermogenRechts_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: VermogenRechts_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 VermogenRechts_ReadCompare2(void) 
    {
        return (CY_GET_REG8(VermogenRechts_COMPARE2_LSB_PTR));
    }

#endif /* (VermogenRechts_UseOneCompareMode) */


/*******************************************************************************
* Function Name: VermogenRechts_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint8 VermogenRechts_ReadPeriod(void) 
{
    #if(VermogenRechts_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(VermogenRechts_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(VermogenRechts_PERIOD_LSB_PTR));
    #endif /* (VermogenRechts_UsingFixedFunction) */
}

#if ( VermogenRechts_KillModeMinTime)


    /*******************************************************************************
    * Function Name: VermogenRechts_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void VermogenRechts_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(VermogenRechts_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: VermogenRechts_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 VermogenRechts_ReadKillTime(void) 
    {
        return (CY_GET_REG8(VermogenRechts_KILLMODEMINTIME_PTR));
    }

#endif /* ( VermogenRechts_KillModeMinTime) */

/* [] END OF FILE */
