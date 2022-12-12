#include "project.h"


void getSensoren_setup(void *arg)
{

    xTaskCreate(
        getSensoren_function,       /* Task function */
        "verkrijg sensor status",    /* Task name (string) */
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
  
    getSensoren_setup();          // Not using the task handle 
    getSensoren_setup();          // Not using the task handle 
    getSensoren_setup();          // Not using the task handle 
    ...

    vTaskStartScheduler();
}
*/