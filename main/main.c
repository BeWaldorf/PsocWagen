
#if 1
    #include <stdio.h>
    #include <string.h>
    #include <ESPwifi.h>
    #include <ESPuart.h>

    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"

    #include "esp_system.h"
    #include "esp_log.h"
    #include "esp_event.h"
    #include "driver/uart.h"
    #include "driver/gpio.h"
    
    #endif
#if 1
    #define ESP                 "piloot"
#endif

// vars
char data_wagen[3]={100,100,7};
char data_godot[2]={99,50};


static void tx_task(void * arg){
    tx_task_maker(arg);
}

static void rx_task(void * arg){
    rx_task_maker(arg);
}

void init(void){
    wifi_start();
    uart_start();
    server_start();
}

void app_main(void){
    init();
    xTaskCreate(rx_task, "rcx_uart_task", 1024 * 2, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(tx_task, "trx_uart_task", 1024 * 2, NULL, configMAX_PRIORITIES, NULL);
}