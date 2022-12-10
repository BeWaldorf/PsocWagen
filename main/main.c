
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
    #define TRX_UART            UART_NUM_1
    #define RCX_UART            UART_NUM_2
    #define PIN_RCX_rx          GPIO_NUM_9
    #define PIN_RCX_tx          GPIO_NUM_10
    #define TRX_UART            UART_NUM_1
    #define PIN_TRX_rx          GPIO_NUM_16
    #define PIN_TRX_tx          GPIO_NUM_17
    #define COMM_START          137
    #define COMM_EINDE          139
    #define ESP                 "piloot"
    #define TRX_BUF_SIZE        80
#endif

// vars
static const int RCX_BUF_SIZE = 400;
char data_wagen[4]={100,100,7,8};
char data_godot[3]={100,100,5};

int sendData(const char *logNaam, const char *trx_data){
    const int len = strlen(trx_data);
    const int txBytes = uart_write_bytes(TRX_UART, trx_data, len);
    ESP_LOGI(logNaam,"status: %s en %d verstuurd",trx_data, txBytes);
    return txBytes;
}

static void tx_task(void * arg){
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    while (1)
    {
        sendData(TX_TASK_TAG, data_godot);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
static void rx_task(void *arg){
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    char *rcx_data = (char *)malloc(RCX_BUF_SIZE + 1);
    char buf_adres = 0;
    while(1)
    {
        const int rxBytes = uart_read_bytes(RCX_UART, rcx_data, RCX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
        if (rxBytes > 0)
        {
            rcx_data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, rcx_data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, rcx_data, rxBytes, ESP_LOG_INFO);
        }
    }
    do
    {
        if(rcx_data[(uint8_t)buf_adres] == COMM_START){
            uint8_t bericht_index = buf_adres;
            for(uint_fast8_t i = 1; i <= 4; i++){
                data_wagen[i-1] = rcx_data[bericht_index + i];
            }
            buf_adres = COMM_EINDE;
        }
    } while (buf_adres!= COMM_EINDE);
    
    free(rcx_data);
}

void uart_start(void){
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_driver_install(TRX_UART, RCX_BUF_SIZE * 2, 0, 0, NULL, 0);
    ESP_ERROR_CHECK(uart_param_config(TRX_UART, &uart_config));
    uart_set_pin(TRX_UART, PIN_TRX_tx, PIN_TRX_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(RCX_UART,RCX_BUF_SIZE*2, 0, 0, NULL, 0);
    ESP_ERROR_CHECK(uart_param_config(RCX_UART, &uart_config));
    uart_set_pin(RCX_UART, PIN_RCX_tx, PIN_RCX_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
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