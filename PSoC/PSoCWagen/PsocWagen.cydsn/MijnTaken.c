#include <project.h>
#include <UART.h>
#include <Wagen.h>



QueueHandle_t   motor_naar_comm, comm_naar_motor, lijn_naar_comm,
                comm_naar_lijn, comm_naar_ESP, ESP_naar_comm;
uint8 snelheden_prod[2], snelheden_cons[2], comm_ESP_prod_data[3], comm_ESP_cons_data[3];
int bericht = 0;

void opstart(){
    setupFreeRTOS();
    wagen_start();
    
    comm_naar_ESP = xQueueCreate(10, 3);
    //comm_naar_lijn = xQueueCreate(5,1);
    comm_naar_motor = xQueueCreate(5, 2);
    //motor_naar_comm = xQueueCreate(5, 2);
    //lijn_naar_comm = xQueueCreate(5, 1);
    ESP_naar_comm = xQueueCreate(10,2);
}
//taak functies
void motorSturing(){
    while(1){
        xQueueReceive(comm_naar_motor,&snelheden_cons,10);
        snelheidsregelaar(snelheden_cons[LINKS],snelheden_cons[RECHTS]);
        vTaskDelay(100);
    }
}
void sensorStatus(){
    while(1){
        commArrMaker();
        xQueueSend(comm_naar_ESP, &comm_ESP_prod_data, 10);
        vTaskDelay(100);
    }
}
void rtxFunc(){
    while(1){
        xQueueReceive(comm_naar_ESP, &comm_ESP_prod_data, 10);
        trx_verzend(comm_ESP_prod_data);
        vTaskDelay(50);
    }
}
void rcxFunc(){
    while(1){
        if(bericht>1){
            uint8 snelheden_prod = RingBuf_getchar(&rcx_buffer);
            xQueueSend(comm_naar_motor, &snelheden_prod, portMAX_DELAY);
            --bericht;
        }
        vTaskDelay(50);
    }
}

//taak declaraties
