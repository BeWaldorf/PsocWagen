/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <MijnTaken.h>


int main(void)
{
    CyGlobalIntEnable;
    opstart();
    
    void rcxTaakSetup();
    void trxTaakSetup();
    void motorTaakSetup();
    void sensorTaakSetup();
    
    vTaskStartScheduler();
}

/* [] END OF FILE */
