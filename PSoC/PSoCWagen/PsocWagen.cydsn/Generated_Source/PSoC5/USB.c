/*******************************************************************************
* File Name: USB.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
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
#if (USB_INTERNAL_CLOCK_USED)
    #include "USB_IntClock.h"
#endif /* End USB_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 USB_initVar = 0u;

#if (USB_TX_INTERRUPT_ENABLED && USB_TX_ENABLED)
    volatile uint8 USB_txBuffer[USB_TX_BUFFER_SIZE];
    volatile uint8 USB_txBufferRead = 0u;
    uint8 USB_txBufferWrite = 0u;
#endif /* (USB_TX_INTERRUPT_ENABLED && USB_TX_ENABLED) */

#if (USB_RX_INTERRUPT_ENABLED && (USB_RX_ENABLED || USB_HD_ENABLED))
    uint8 USB_errorStatus = 0u;
    volatile uint8 USB_rxBuffer[USB_RX_BUFFER_SIZE];
    volatile uint8 USB_rxBufferRead  = 0u;
    volatile uint8 USB_rxBufferWrite = 0u;
    volatile uint8 USB_rxBufferLoopDetect = 0u;
    volatile uint8 USB_rxBufferOverflow   = 0u;
    #if (USB_RXHW_ADDRESS_ENABLED)
        volatile uint8 USB_rxAddressMode = USB_RX_ADDRESS_MODE;
        volatile uint8 USB_rxAddressDetected = 0u;
    #endif /* (USB_RXHW_ADDRESS_ENABLED) */
#endif /* (USB_RX_INTERRUPT_ENABLED && (USB_RX_ENABLED || USB_HD_ENABLED)) */


/*******************************************************************************
* Function Name: USB_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  USB_Start() sets the initVar variable, calls the
*  USB_Init() function, and then calls the
*  USB_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The USB_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time USB_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the USB_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USB_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(USB_initVar == 0u)
    {
        USB_Init();
        USB_initVar = 1u;
    }

    USB_Enable();
}


/*******************************************************************************
* Function Name: USB_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call USB_Init() because
*  the USB_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void USB_Init(void) 
{
    #if(USB_RX_ENABLED || USB_HD_ENABLED)

        #if (USB_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(USB_RX_VECT_NUM, &USB_RXISR);
            CyIntSetPriority(USB_RX_VECT_NUM, USB_RX_PRIOR_NUM);
            USB_errorStatus = 0u;
        #endif /* (USB_RX_INTERRUPT_ENABLED) */

        #if (USB_RXHW_ADDRESS_ENABLED)
            USB_SetRxAddressMode(USB_RX_ADDRESS_MODE);
            USB_SetRxAddress1(USB_RX_HW_ADDRESS1);
            USB_SetRxAddress2(USB_RX_HW_ADDRESS2);
        #endif /* End USB_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        USB_RXBITCTR_PERIOD_REG = USB_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        USB_RXSTATUS_MASK_REG  = USB_INIT_RX_INTERRUPTS_MASK;
    #endif /* End USB_RX_ENABLED || USB_HD_ENABLED*/

    #if(USB_TX_ENABLED)
        #if (USB_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(USB_TX_VECT_NUM, &USB_TXISR);
            CyIntSetPriority(USB_TX_VECT_NUM, USB_TX_PRIOR_NUM);
        #endif /* (USB_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (USB_TXCLKGEN_DP)
            USB_TXBITCLKGEN_CTR_REG = USB_BIT_CENTER;
            USB_TXBITCLKTX_COMPLETE_REG = ((USB_NUMBER_OF_DATA_BITS +
                        USB_NUMBER_OF_START_BIT) * USB_OVER_SAMPLE_COUNT) - 1u;
        #else
            USB_TXBITCTR_PERIOD_REG = ((USB_NUMBER_OF_DATA_BITS +
                        USB_NUMBER_OF_START_BIT) * USB_OVER_SAMPLE_8) - 1u;
        #endif /* End USB_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (USB_TX_INTERRUPT_ENABLED)
            USB_TXSTATUS_MASK_REG = USB_TX_STS_FIFO_EMPTY;
        #else
            USB_TXSTATUS_MASK_REG = USB_INIT_TX_INTERRUPTS_MASK;
        #endif /*End USB_TX_INTERRUPT_ENABLED*/

    #endif /* End USB_TX_ENABLED */

    #if(USB_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        USB_WriteControlRegister( \
            (USB_ReadControlRegister() & (uint8)~USB_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(USB_PARITY_TYPE << USB_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End USB_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: USB_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call USB_Enable() because the USB_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  USB_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void USB_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (USB_RX_ENABLED || USB_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        USB_RXBITCTR_CONTROL_REG |= USB_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        USB_RXSTATUS_ACTL_REG  |= USB_INT_ENABLE;

        #if (USB_RX_INTERRUPT_ENABLED)
            USB_EnableRxInt();

            #if (USB_RXHW_ADDRESS_ENABLED)
                USB_rxAddressDetected = 0u;
            #endif /* (USB_RXHW_ADDRESS_ENABLED) */
        #endif /* (USB_RX_INTERRUPT_ENABLED) */
    #endif /* (USB_RX_ENABLED || USB_HD_ENABLED) */

    #if(USB_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!USB_TXCLKGEN_DP)
            USB_TXBITCTR_CONTROL_REG |= USB_CNTR_ENABLE;
        #endif /* End USB_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        USB_TXSTATUS_ACTL_REG |= USB_INT_ENABLE;
        #if (USB_TX_INTERRUPT_ENABLED)
            USB_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            USB_EnableTxInt();
        #endif /* (USB_TX_INTERRUPT_ENABLED) */
     #endif /* (USB_TX_INTERRUPT_ENABLED) */

    #if (USB_INTERNAL_CLOCK_USED)
        USB_IntClock_Start();  /* Enable the clock */
    #endif /* (USB_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: USB_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void USB_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (USB_RX_ENABLED || USB_HD_ENABLED)
        USB_RXBITCTR_CONTROL_REG &= (uint8) ~USB_CNTR_ENABLE;
    #endif /* (USB_RX_ENABLED || USB_HD_ENABLED) */

    #if (USB_TX_ENABLED)
        #if(!USB_TXCLKGEN_DP)
            USB_TXBITCTR_CONTROL_REG &= (uint8) ~USB_CNTR_ENABLE;
        #endif /* (!USB_TXCLKGEN_DP) */
    #endif /* (USB_TX_ENABLED) */

    #if (USB_INTERNAL_CLOCK_USED)
        USB_IntClock_Stop();   /* Disable the clock */
    #endif /* (USB_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (USB_RX_ENABLED || USB_HD_ENABLED)
        USB_RXSTATUS_ACTL_REG  &= (uint8) ~USB_INT_ENABLE;

        #if (USB_RX_INTERRUPT_ENABLED)
            USB_DisableRxInt();
        #endif /* (USB_RX_INTERRUPT_ENABLED) */
    #endif /* (USB_RX_ENABLED || USB_HD_ENABLED) */

    #if (USB_TX_ENABLED)
        USB_TXSTATUS_ACTL_REG &= (uint8) ~USB_INT_ENABLE;

        #if (USB_TX_INTERRUPT_ENABLED)
            USB_DisableTxInt();
        #endif /* (USB_TX_INTERRUPT_ENABLED) */
    #endif /* (USB_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: USB_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 USB_ReadControlRegister(void) 
{
    #if (USB_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(USB_CONTROL_REG);
    #endif /* (USB_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: USB_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  USB_WriteControlRegister(uint8 control) 
{
    #if (USB_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       USB_CONTROL_REG = control;
    #endif /* (USB_CONTROL_REG_REMOVED) */
}


#if(USB_RX_ENABLED || USB_HD_ENABLED)
    /*******************************************************************************
    * Function Name: USB_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      USB_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      USB_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      USB_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      USB_RX_STS_BREAK            Interrupt on break.
    *      USB_RX_STS_OVERRUN          Interrupt on overrun error.
    *      USB_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      USB_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void USB_SetRxInterruptMode(uint8 intSrc) 
    {
        USB_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: USB_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  USB_rxBuffer - RAM buffer pointer for save received data.
    *  USB_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  USB_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  USB_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 USB_ReadRxData(void) 
    {
        uint8 rxData;

    #if (USB_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        USB_DisableRxInt();

        locRxBufferRead  = USB_rxBufferRead;
        locRxBufferWrite = USB_rxBufferWrite;

        if( (USB_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = USB_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= USB_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            USB_rxBufferRead = locRxBufferRead;

            if(USB_rxBufferLoopDetect != 0u)
            {
                USB_rxBufferLoopDetect = 0u;
                #if ((USB_RX_INTERRUPT_ENABLED) && (USB_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( USB_HD_ENABLED )
                        if((USB_CONTROL_REG & USB_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            USB_RXSTATUS_MASK_REG  |= USB_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        USB_RXSTATUS_MASK_REG  |= USB_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end USB_HD_ENABLED */
                #endif /* ((USB_RX_INTERRUPT_ENABLED) && (USB_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = USB_RXDATA_REG;
        }

        USB_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = USB_RXDATA_REG;

    #endif /* (USB_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: USB_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  USB_RX_STS_FIFO_NOTEMPTY.
    *  USB_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  USB_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   USB_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   USB_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 USB_ReadRxStatus(void) 
    {
        uint8 status;

        status = USB_RXSTATUS_REG & USB_RX_HW_MASK;

    #if (USB_RX_INTERRUPT_ENABLED)
        if(USB_rxBufferOverflow != 0u)
        {
            status |= USB_RX_STS_SOFT_BUFF_OVER;
            USB_rxBufferOverflow = 0u;
        }
    #endif /* (USB_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: USB_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. USB_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  USB_rxBuffer - RAM buffer pointer for save received data.
    *  USB_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  USB_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  USB_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 USB_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (USB_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        USB_DisableRxInt();

        locRxBufferRead  = USB_rxBufferRead;
        locRxBufferWrite = USB_rxBufferWrite;

        if( (USB_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = USB_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= USB_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            USB_rxBufferRead = locRxBufferRead;

            if(USB_rxBufferLoopDetect != 0u)
            {
                USB_rxBufferLoopDetect = 0u;
                #if( (USB_RX_INTERRUPT_ENABLED) && (USB_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( USB_HD_ENABLED )
                        if((USB_CONTROL_REG & USB_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            USB_RXSTATUS_MASK_REG |= USB_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        USB_RXSTATUS_MASK_REG |= USB_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end USB_HD_ENABLED */
                #endif /* USB_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = USB_RXSTATUS_REG;
            if((rxStatus & USB_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = USB_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (USB_RX_STS_BREAK | USB_RX_STS_PAR_ERROR |
                                USB_RX_STS_STOP_ERROR | USB_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        USB_EnableRxInt();

    #else

        rxStatus =USB_RXSTATUS_REG;
        if((rxStatus & USB_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = USB_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (USB_RX_STS_BREAK | USB_RX_STS_PAR_ERROR |
                            USB_RX_STS_STOP_ERROR | USB_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (USB_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: USB_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 USB_GetByte(void) 
    {
        
    #if (USB_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        USB_DisableRxInt();
        locErrorStatus = (uint16)USB_errorStatus;
        USB_errorStatus = 0u;
        USB_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | USB_ReadRxData() );
    #else
        return ( ((uint16)USB_ReadRxStatus() << 8u) | USB_ReadRxData() );
    #endif /* USB_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: USB_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  USB_rxBufferWrite - used to calculate left bytes.
    *  USB_rxBufferRead - used to calculate left bytes.
    *  USB_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 USB_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (USB_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        USB_DisableRxInt();

        if(USB_rxBufferRead == USB_rxBufferWrite)
        {
            if(USB_rxBufferLoopDetect != 0u)
            {
                size = USB_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(USB_rxBufferRead < USB_rxBufferWrite)
        {
            size = (USB_rxBufferWrite - USB_rxBufferRead);
        }
        else
        {
            size = (USB_RX_BUFFER_SIZE - USB_rxBufferRead) + USB_rxBufferWrite;
        }

        USB_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((USB_RXSTATUS_REG & USB_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (USB_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: USB_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  USB_rxBufferWrite - cleared to zero.
    *  USB_rxBufferRead - cleared to zero.
    *  USB_rxBufferLoopDetect - cleared to zero.
    *  USB_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void USB_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        USB_RXDATA_AUX_CTL_REG |= (uint8)  USB_RX_FIFO_CLR;
        USB_RXDATA_AUX_CTL_REG &= (uint8) ~USB_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (USB_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        USB_DisableRxInt();

        USB_rxBufferRead = 0u;
        USB_rxBufferWrite = 0u;
        USB_rxBufferLoopDetect = 0u;
        USB_rxBufferOverflow = 0u;

        USB_EnableRxInt();

    #endif /* (USB_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: USB_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  USB__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  USB__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  USB__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  USB__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  USB__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  USB_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  USB_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void USB_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(USB_RXHW_ADDRESS_ENABLED)
            #if(USB_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* USB_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = USB_CONTROL_REG & (uint8)~USB_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << USB_CTRL_RXADDR_MODE0_SHIFT);
                USB_CONTROL_REG = tmpCtrl;

                #if(USB_RX_INTERRUPT_ENABLED && \
                   (USB_RXBUFFERSIZE > USB_FIFO_LENGTH) )
                    USB_rxAddressMode = addressMode;
                    USB_rxAddressDetected = 0u;
                #endif /* End USB_RXBUFFERSIZE > USB_FIFO_LENGTH*/
            #endif /* End USB_CONTROL_REG_REMOVED */
        #else /* USB_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End USB_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: USB_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void USB_SetRxAddress1(uint8 address) 
    {
        USB_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: USB_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void USB_SetRxAddress2(uint8 address) 
    {
        USB_RXADDRESS2_REG = address;
    }

#endif  /* USB_RX_ENABLED || USB_HD_ENABLED*/


#if( (USB_TX_ENABLED) || (USB_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: USB_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   USB_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   USB_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   USB_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   USB_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void USB_SetTxInterruptMode(uint8 intSrc) 
    {
        USB_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: USB_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  USB_txBuffer - RAM buffer pointer for save data for transmission
    *  USB_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  USB_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  USB_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USB_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(USB_initVar != 0u)
        {
        #if (USB_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            USB_DisableTxInt();

            if( (USB_txBufferRead == USB_txBufferWrite) &&
                ((USB_TXSTATUS_REG & USB_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                USB_TXDATA_REG = txDataByte;
            }
            else
            {
                if(USB_txBufferWrite >= USB_TX_BUFFER_SIZE)
                {
                    USB_txBufferWrite = 0u;
                }

                USB_txBuffer[USB_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                USB_txBufferWrite++;
            }

            USB_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            USB_TXDATA_REG = txDataByte;

        #endif /*(USB_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: USB_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 USB_ReadTxStatus(void) 
    {
        return(USB_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: USB_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  USB_txBuffer - RAM buffer pointer for save data for transmission
    *  USB_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  USB_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  USB_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void USB_PutChar(uint8 txDataByte) 
    {
    #if (USB_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((USB_TX_BUFFER_SIZE > USB_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            USB_DisableTxInt();
        #endif /* (USB_TX_BUFFER_SIZE > USB_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = USB_txBufferWrite;
            locTxBufferRead  = USB_txBufferRead;

        #if ((USB_TX_BUFFER_SIZE > USB_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            USB_EnableTxInt();
        #endif /* (USB_TX_BUFFER_SIZE > USB_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(USB_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((USB_TXSTATUS_REG & USB_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            USB_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= USB_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            USB_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((USB_TX_BUFFER_SIZE > USB_MAX_BYTE_VALUE) && (CY_PSOC3))
            USB_DisableTxInt();
        #endif /* (USB_TX_BUFFER_SIZE > USB_MAX_BYTE_VALUE) && (CY_PSOC3) */

            USB_txBufferWrite = locTxBufferWrite;

        #if ((USB_TX_BUFFER_SIZE > USB_MAX_BYTE_VALUE) && (CY_PSOC3))
            USB_EnableTxInt();
        #endif /* (USB_TX_BUFFER_SIZE > USB_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (USB_TXSTATUS_REG & USB_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                USB_SetPendingTxInt();
            }
        }

    #else

        while((USB_TXSTATUS_REG & USB_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        USB_TXDATA_REG = txDataByte;

    #endif /* USB_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: USB_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  USB_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void USB_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(USB_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                USB_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: USB_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  USB_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void USB_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(USB_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                USB_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: USB_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  USB_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USB_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(USB_initVar != 0u)
        {
            USB_PutChar(txDataByte);
            USB_PutChar(0x0Du);
            USB_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: USB_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  USB_txBufferWrite - used to calculate left space.
    *  USB_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 USB_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (USB_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        USB_DisableTxInt();

        if(USB_txBufferRead == USB_txBufferWrite)
        {
            size = 0u;
        }
        else if(USB_txBufferRead < USB_txBufferWrite)
        {
            size = (USB_txBufferWrite - USB_txBufferRead);
        }
        else
        {
            size = (USB_TX_BUFFER_SIZE - USB_txBufferRead) +
                    USB_txBufferWrite;
        }

        USB_EnableTxInt();

    #else

        size = USB_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & USB_TX_STS_FIFO_FULL) != 0u)
        {
            size = USB_FIFO_LENGTH;
        }
        else if((size & USB_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (USB_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: USB_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  USB_txBufferWrite - cleared to zero.
    *  USB_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void USB_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        USB_TXDATA_AUX_CTL_REG |= (uint8)  USB_TX_FIFO_CLR;
        USB_TXDATA_AUX_CTL_REG &= (uint8) ~USB_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (USB_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        USB_DisableTxInt();

        USB_txBufferRead = 0u;
        USB_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        USB_EnableTxInt();

    #endif /* (USB_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: USB_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   USB_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   USB_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   USB_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   USB_SEND_WAIT_REINIT - Performs both options: 
    *      USB_SEND_BREAK and USB_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  USB_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with USB_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The USB_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void USB_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(USB_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(USB_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == USB_SEND_BREAK) ||
                (retMode == USB_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                USB_WriteControlRegister(USB_ReadControlRegister() |
                                                      USB_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                USB_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = USB_TXSTATUS_REG;
                }
                while((tmpStat & USB_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == USB_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == USB_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = USB_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & USB_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == USB_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == USB_REINIT) ||
                (retMode == USB_SEND_WAIT_REINIT) )
            {
                USB_WriteControlRegister(USB_ReadControlRegister() &
                                              (uint8)~USB_CTRL_HD_SEND_BREAK);
            }

        #else /* USB_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == USB_SEND_BREAK) ||
                (retMode == USB_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (USB_PARITY_TYPE != USB__B_UART__NONE_REVB) || \
                                    (USB_PARITY_TYPE_SW != 0u) )
                    USB_WriteControlRegister(USB_ReadControlRegister() |
                                                          USB_CTRL_HD_SEND_BREAK);
                #endif /* End USB_PARITY_TYPE != USB__B_UART__NONE_REVB  */

                #if(USB_TXCLKGEN_DP)
                    txPeriod = USB_TXBITCLKTX_COMPLETE_REG;
                    USB_TXBITCLKTX_COMPLETE_REG = USB_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = USB_TXBITCTR_PERIOD_REG;
                    USB_TXBITCTR_PERIOD_REG = USB_TXBITCTR_BREAKBITS8X;
                #endif /* End USB_TXCLKGEN_DP */

                /* Send zeros */
                USB_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = USB_TXSTATUS_REG;
                }
                while((tmpStat & USB_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == USB_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == USB_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = USB_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & USB_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == USB_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == USB_REINIT) ||
                (retMode == USB_SEND_WAIT_REINIT) )
            {

            #if(USB_TXCLKGEN_DP)
                USB_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                USB_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End USB_TXCLKGEN_DP */

            #if( (USB_PARITY_TYPE != USB__B_UART__NONE_REVB) || \
                 (USB_PARITY_TYPE_SW != 0u) )
                USB_WriteControlRegister(USB_ReadControlRegister() &
                                                      (uint8) ~USB_CTRL_HD_SEND_BREAK);
            #endif /* End USB_PARITY_TYPE != NONE */
            }
        #endif    /* End USB_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: USB_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       USB_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       USB_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears USB_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void USB_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( USB_CONTROL_REG_REMOVED == 0u )
            USB_WriteControlRegister(USB_ReadControlRegister() |
                                                  USB_CTRL_MARK);
        #endif /* End USB_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( USB_CONTROL_REG_REMOVED == 0u )
            USB_WriteControlRegister(USB_ReadControlRegister() &
                                                  (uint8) ~USB_CTRL_MARK);
        #endif /* End USB_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndUSB_TX_ENABLED */

#if(USB_HD_ENABLED)


    /*******************************************************************************
    * Function Name: USB_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void USB_LoadRxConfig(void) 
    {
        USB_WriteControlRegister(USB_ReadControlRegister() &
                                                (uint8)~USB_CTRL_HD_SEND);
        USB_RXBITCTR_PERIOD_REG = USB_HD_RXBITCTR_INIT;

    #if (USB_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        USB_SetRxInterruptMode(USB_INIT_RX_INTERRUPTS_MASK);
    #endif /* (USB_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: USB_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void USB_LoadTxConfig(void) 
    {
    #if (USB_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        USB_SetRxInterruptMode(0u);
    #endif /* (USB_RX_INTERRUPT_ENABLED) */

        USB_WriteControlRegister(USB_ReadControlRegister() | USB_CTRL_HD_SEND);
        USB_RXBITCTR_PERIOD_REG = USB_HD_TXBITCTR_INIT;
    }

#endif  /* USB_HD_ENABLED */


/* [] END OF FILE */
