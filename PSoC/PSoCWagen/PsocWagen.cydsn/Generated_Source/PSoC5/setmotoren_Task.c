#include "project.h"


void setmotoren_setup(void *arg)
{

    xTaskCreate(
        setmotoren_function,       /* Task function */
        "zet motor snelheden",    /* Task name (string) */
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
  
    setmotoren_setup();          // Not using the task handle 
    setmotoren_setup();          // Not using the task handle 
    setmotoren_setup();          // Not using the task handle 
    ...

    vTaskStartScheduler();
}
*/