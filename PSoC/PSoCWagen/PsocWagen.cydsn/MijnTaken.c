#include <project.h>
#include <UART.h>
#include <Wagen.h>

RingBuf rcx_buffer;
QueueHandle_t   comm_naar_motor, comm_naar_ESP, ESP_naar_comm;
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
void motorSturingFunc(){
    while(1){
        xQueueReceive(comm_naar_motor,&snelheden_cons,10);
        snelheidsregelaar(snelheden_cons[LINKS],snelheden_cons[RECHTS]);
        vTaskDelay(100);
    }
}
void sensorFunc(){
    while(1){
        commArrMaker();
        xQueueSend(comm_naar_ESP, &comm_ESP_prod_data, 10);
        vTaskDelay(100);
    }
}
void trxFunc(){
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
void rcxTaakSetup(void *arg){
    xTaskCreate(rcxFunc,"UART ontvanger",500,arg,1,0);
}
void trxTaakSetup(void *arg){
    xTaskCreate(trxFunc,"UART verzender",500,arg,1,0);
}
void motorTaakSetup(void *arg){
    xTaskCreate(motorSturingFunc,"regelt vermogen",200,arg,1,0);
}
void sensorTaakSetup(void *arg){
    xTaskCreate(sensorFunc,"stelt status op",200,arg,1,0);
}