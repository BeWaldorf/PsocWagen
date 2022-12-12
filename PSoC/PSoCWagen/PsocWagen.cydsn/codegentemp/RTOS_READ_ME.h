//Deze file dient om je gewoon op gang te zetten
/*
___________dus je hebt gekozen om met een OS te werken__________________
- je zou normaal dus de component moeten toegevoegd hebben
- je kan normaal een deel dingen instellen in het OS blokje zelf, 
  maar bekijk zeker ook de file FreeRTOSConfig.h
- Voeg verder ook de juiste include map in:
    ga naar de bibliotheek/myFreeRTOS/source/include (dit doe je bij de compiler)
- En voeg liefst een taak toe voor meer uitleg over taken ga naar de taak
 */  


//___Je hoofdprogramma ziet er ongeveer zo uit:
/*______________________________________________________________________________________    
#include "project.h"

int main(void)
{
    CyGlobalIntEnable; //Enable global interrupts. 
    
    setupFreeRTOS();
  
    MyTask_1_setup(0);          // Not using the task handle 

    vTaskStartScheduler();
}
______________________________________________________________________________________*/

//___Een taak ziet er ongeveer zo uit:
/*______________________________________________________________________________________

void Task1_function(void *arg)
{
    (void)arg;
    
        while(1) {
        vTaskDelay(600);
        }
}
        
en task setup doet dan weer dit
        void Task1_setup(void *arg)
{

    xTaskCreate(
        Task1_function,     // Task function 
        "BlinkLed",         // Task name (string) 
        100,                // Task stack, allocated from heap 
        arg,                // No param passed to task function 
        1,                  // Low priority 
        0 );                // Not using the task handle 
   

}
______________________________________________________________________________________*/

//___Een semaphore ziet er ongeveer zo uit:
/*_______________________________________________________________________

static SemaphoreHandle_t mutex;         // het aanmaken van de handle (verwijzing)

mutex = xSemaphoreCreateMutex();        // kies het juiste type (Mutex, recursive Mutex, semaphore binairy/counting

voorbeeldcode voor gebruik
    xSemaphoreTake(mutex,portMAX_DELAY);
    LED_Write(1);
    vTaskDelay(100);   
    xSemaphoreGive(mutex);   
        
* verander de configuratie
* creeer de semaphore voor de plek(ken) waar je ze wil gebruiken. kan door gebruik te maken van H file met globale definitie
* setup voor OS start (liefst)
* gebruik in functie's die het nodig hebben
        
_______________________________________________________________________*/

//___een queue ziet er ongeveer zo uit:
/*_______________________________________________________________________________________
typedef struct Message {
  char body[20];
  int count;
} Message;

Message rcv_msg;

static QueueHandle_t msg_queue;
msg_queue = xQueueCreate(msg_queue_len, sizeof(Message));


xQueueSend(msg_queue, (void *)&msg, 10);




if (xQueueReceive(msg_queue, (void *)&rcv_msg, 0) == pdTRUE) {
      Serial.print(rcv_msg.body);

_______________________________________________________________________________________*/


//____een event ziet er ongeveer zo uit:
/*__________________________________________________________________________

defineer het bestaan van de groep 
EventGroupHandle_t  xEventGroup;

welke bit in de eventgroup is wat? 
#define TASK1_BIT   (1UL << 0UL) // zero shift for bit0
#define TASK2_BIT   (1UL << 1UL) // 1 shift for flag  bit 1
#define TASK3_BIT   (1UL << 2UL) // 2 shift for flag bit 2

maak de groep aan en ken ze toe aan de definitie
xEventGroup  =  xEventGroupCreate();

in 1 taak kan je nu een event bit setten
xEventGroupSetBits(xEventGroup, TASK1_BIT);

in een andere taak ga je wachten tot er iets aan de hand is
const EventBits_t xBitsToWaitFor  = (TASK1_BIT | TASK2_BIT | TASK3_BIT);

EventBits_t xEventGroupValue;
 xEventGroupValue  = xEventGroupWaitBits(xEventGroup,
                                            xBitsToWaitFor,
                                            pdTRUE,
                                            pdTRUE,
                                            portMAX_DELAY
                                            );




__________________________________________________________________________*/





