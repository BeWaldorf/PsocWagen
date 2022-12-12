#include "project.h"


void TRX_Task_setup(void *arg)
{

    xTaskCreate(
        TRX_Task_function,       /* Task function */
        "TRX UART task",    /* Task name (string) */
        100,            /* Task stack, allocated from heap */
        arg,              /* No param passed to task function */
        1,              /* Low priority */
        0 );            /* Not using the task handle */
   

}

/*
int main(void)
{
    CyGlobalIntEnable; //Enable global interrupts. 
    
    setupFreeRTOS();
  
    TRX_Task_setup();          // Not using the task handle 
    TRX_Task_setup();          // Not using the task handle 
    TRX_Task_setup();          // Not using the task handle 
    ...

    vTaskStartScheduler();
}
*/