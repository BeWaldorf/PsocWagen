#include <project.h>
#include <UART.h>
#include <Wagen.h>

QueueHandle_t   motor_naar_comm, comm_naar_motor, lijn_naar_comm,
                comm_naar_lijn, comm_naar_ESP, ESP_naar_comm;
uint8 snelheden[2],lijn,comm_ESP_data[5] ;
                
void opstart(){
    setupFreeRTOS();
    wagen_start();
    
    comm_naar_ESP = xQueueCreate(10, 5);
    //comm_naar_lijn = xQueueCreate(5,1);
    comm_naar_motor = xQueueCreate(5, 2);
    //motor_naar_comm = xQueueCreate(5, 2);
    //lijn_naar_comm = xQueueCreate(5, 1);
    ESP_naar_comm = xQueueCreate(10,5);
}

void motorSturing(){
    while(1){
        xQueueReceive(comm_naar_motor,&snelheden,10);
        snelheidsregelaar(snelheden[LINKS],snelheden[RECHTS]);
        vTaskDelay(100);
    }
}
void sensorStatus(){
    while(1){
        commArrMaker();
        xQueueSend(comm_naar_ESP,&comm_ESP_data,10);
        vTaskDelay(100);
    }
}

