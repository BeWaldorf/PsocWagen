#include "project.h"


void RCX_Task_setup(void *arg)
{

    xTaskCreate(
        RCX_Task_function,       /* Task function */
        "UART  verzender task",    /* Task name (string) */
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
  
    RCX_Task_setup();          // Not using the task handle 
    RCX_Task_setup();          // Not using the task handle 
    RCX_Task_setup();          // Not using the task handle 
    ...

    vTaskStartScheduler();
}
*/