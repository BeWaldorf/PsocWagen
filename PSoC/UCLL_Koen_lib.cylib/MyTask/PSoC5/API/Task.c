#include "project.h"


void `$INSTANCE_NAME`_setup(void *arg)
{

    xTaskCreate(
        `$INSTANCE_NAME`_function,       /* Task function */
        "`$TaskName`",    /* Task name (string) */
        `$TaskStack`,            /* Task stack, allocated from heap */
        arg,              /* No param passed to task function */
        `$TaskPrior`,              /* Low priority */
        0 );            /* Not using the task handle */
   

}

/*
int main(void)
{
    CyGlobalIntEnable; //Enable global interrupts. 
    
    setupFreeRTOS();
  
    `$INSTANCE_NAME`_setup();          // Not using the task handle 
    `$INSTANCE_NAME`_setup();          // Not using the task handle 
    `$INSTANCE_NAME`_setup();          // Not using the task handle 
    ...

    vTaskStartScheduler();
}
*/