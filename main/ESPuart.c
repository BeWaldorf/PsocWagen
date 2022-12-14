#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#define TRX_UART UART_NUM_1
#define RCX_UART UART_NUM_2
#define PIN_RCX_rx GPIO_NUM_9
#define PIN_RCX_tx GPIO_NUM_10
#define TRX_UART UART_NUM_1
#define PIN_TRX_rx GPIO_NUM_16
#define PIN_TRX_tx GPIO_NUM_17
#define COMM_START 137
#define COMM_EINDE 139
#define ESP "piloot"
#define TRX_BUF_SIZE 80

static const int RCX_BUF_SIZE = 400;
extern char data_wagen[3];
extern char data_godot[2];

int sendData(const char *logNaam, const char *trx_data){
    char commArr[5] = {COMM_START, trx_data[0], trx_data[1], trx_data[2], COMM_EINDE};
     const int len = strlen(commArr);
    const int txBytes = uart_write_bytes(TRX_UART, commArr, len);
    ESP_LOGI(logNaam, "status: %s en %d verstuurd", commArr, txBytes);
    return txBytes;
}

void tx_task_maker(void *arg){
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    while (1)
    {
        sendData(TX_TASK_TAG, data_godot);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

    void rx_task_maker(void *arg){
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    char *rcx_data = (char *)malloc(RCX_BUF_SIZE + 1);
    char buf_adres = 0;
    while (1)
    {
        const int rxBytes = uart_read_bytes(RCX_UART, rcx_data, RCX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
        if (rxBytes > 0)
        {
            rcx_data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, rcx_data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, rcx_data, rxBytes, ESP_LOG_INFO);
        }
        do
        {
            if (rcx_data[(uint8_t)buf_adres] == COMM_START)
            {
                uint8_t bericht_index = buf_adres;
                for (uint_fast8_t i = 1; i <= 3; i++)
                {
                    data_wagen[i - 1] = rcx_data[bericht_index + i];
                }
                buf_adres = COMM_EINDE;
            }
        } while (buf_adres != COMM_EINDE);
    }

    free(rcx_data);
}

void uart_start(void){
    const uart_config_t uart_config = {
        .baud_rate = 4800,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_2,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_driver_install(TRX_UART, RCX_BUF_SIZE * 2, 0, 0, NULL, 0);
    ESP_ERROR_CHECK(uart_param_config(TRX_UART, &uart_config));
    uart_set_pin(TRX_UART, PIN_TRX_tx, PIN_TRX_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(RCX_UART, RCX_BUF_SIZE * 2, 0, 0, NULL, 0);
    ESP_ERROR_CHECK(uart_param_config(RCX_UART, &uart_config));
    uart_set_pin(RCX_UART, PIN_RCX_tx, PIN_RCX_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}