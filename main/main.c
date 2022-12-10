
#if 1
    #include <stdio.h>
    #include <string.h>

    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "freertos/event_groups.h"

     #include "esp_http_server.h"
     #include "esp_system.h"
     #include "esp_log.h"
     #include "esp_event.h"

    #include "driver/uart.h"
    #include "driver/gpio.h"

    #include <wifi.h>
    #endif
#if 1
    
    // #define SSID                "SuperNitro"
    // #define WIFI_PASS           "SuperWagen5"
    //benaming RCX/TRX volgens PSOC-logica, rx tx lokaal.
    #define TRX_UART            UART_NUM_1
    #define RCX_UART            UART_NUM_2
    #define PIN_RCX_rx          GPIO_NUM_9
    #define PIN_RCX_tx          GPIO_NUM_10
    #define TRX_UART            UART_NUM_1
    #define PIN_TRX_rx          GPIO_NUM_16
    #define PIN_TRX_tx          GPIO_NUM_17
    #define COMM_START          137
    #define COMM_EINDE          139
    // #define WIFI_SUCCES_BIT     BIT0
    // #define WIFI_BUIS_BIT       BIT1
    #define ESP                 "piloot"
    // #define MAX_WIFI_POGING     25
    // #define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
    #define TRX_BUF_SIZE        80
    // #define MIN(a,b) ((a)<(b) ?(a):(b))
#endif

// vars
// static EventGroupHandle_t static_wifi_groep;
// static int s_retry_num = 0;
static const int RCX_BUF_SIZE = 400;
// static httpd_handle_t server = NULL;
char data_wagen[4]={100,100,7,8};
char data_godot[3]={100,100,5};

// static void wifi_event_handler(void *arg, esp_event_base_t event_base,
//                           int32_t event_id, void *event_data)       {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
//         esp_wifi_connect();
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
//         if (s_retry_num < MAX_WIFI_POGING){
//             esp_wifi_connect();
//             s_retry_num++;
//             ESP_LOGI(ESP, " probeert verbinding te maken met circuit");
//         }
//         else{
//             xEventGroupSetBits(static_wifi_groep, WIFI_BUIS_BIT);
//         }
//         ESP_LOGI(ESP, " kon geen circuit vinden.");
//     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
//         ESP_LOGI(ESP, "woop woop");
//         ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
//         ESP_LOGI(ESP, " zijn IP:" IPSTR, IP2STR(&event->ip_info.ip));
//         s_retry_num = 0;
//         xEventGroupSetBits(static_wifi_groep, WIFI_SUCCES_BIT);
//     }
//     else{
//         ESP_LOGI(ESP,"WAP WAP");
//         xEventGroupSetBits(static_wifi_groep, WIFI_SUCCES_BIT);
//     }
// }

int sendData(const char *logNaam, const char *trx_data){
    const int len = strlen(trx_data);
    const int txBytes = uart_write_bytes(TRX_UART, trx_data, len);
    ESP_LOGI(logNaam, "status: %d en %d verstuurd",trx_data, txBytes);
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
    char buf_adres = 0
    while (1)
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
        if(rcx_data[buf_adres] == COMM_START){
            uint8_t bericht_index = buf_adres;
            for(uint_fast8_t i = 1; i <= 4; i++){
                data_wagen[i-1] = rcx_data[bericht_index + i];
            }
            buf_adres = COMM_EINDE;
        }
    } while (buf_adres!= COMM_EINDE);
    
    free(rcx_data);
}

// esp_err_t get_handler(httpd_req_t *req){
//     char antw[4];
//     for(uint_fast8_t i = 0;i<4; i++){
//         antw[i]=data_wagen[i];
//     }
//     httpd_resp_send(req, antw, HTTPD_RESP_USE_STRLEN);
//     httpd_resp_set_type(req,(char)200);
//     //return "ge weet toch da ge mij kunt betrouwen zeker"
//     return ESP_OK;
// }
// esp_err_t set_handler(httpd_req_t *req){
//     char godot_bericht[13];
//     size_t recv_size = MIN(req->content_len,sizeof(godot_bericht));
//     int controle = httpd_req_recv(req, godot_bericht, recv_size);
    
//     if(controle <= 0){
//         if (controle == HTTPD_SOCK_ERR_TIMEOUT){
//             httpd_resp_send_408(req);
//         }
//         return ESP_FAIL;
//     }
//     httpd_resp_send(req, godot_bericht, HTTPD_RESP_USE_STRLEN);
//     for(uint_fast8_t i=0; i<3; i++){
//         data_godot[i]=godot_bericht[i];
//     }
//     godot_bericht[13] = '\0';
//     ESP_LOGI(ESP,"waarde: %s",godot_bericht);
//     return ESP_OK;
// }

// void wifi_start(void){
//     static_wifi_groep = xEventGroupCreate();
//     nvs_flash_init();
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     esp_netif_create_default_wifi_sta();
//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));
//     esp_event_handler_instance_t instance_any_id;
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
//                                                         ESP_EVENT_ANY_ID,
//                                                         &wifi_event_handler,
//                                                         NULL,
//                                                         &instance_any_id));
//     wifi_config_t mijn_conf = {
//         .sta = {
//             .ssid = SSID,
//             .password = WIFI_PASS,
            
//             .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
//             .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
//         },
//     };
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &mijn_conf));
//     ESP_ERROR_CHECK(esp_wifi_start());
//     ESP_LOGI(ESP, " heeft wifi succesvol gestart.");
//     EventBits_t bits = xEventGroupWaitBits(static_wifi_groep,
//                                            WIFI_SUCCES_BIT | WIFI_BUIS_BIT,
//                                            pdFALSE,
//                                            pdFALSE,
//                                            portMAX_DELAY);
//     if (bits & WIFI_SUCCES_BIT){
//         ESP_LOGI(ESP, "connected to ap SSID:%s",
//                  SSID);
//     }
//     else if (bits & WIFI_BUIS_BIT){
//         ESP_LOGI(ESP, "Failed to connect to SSID:%s",
//                  SSID);
//     }
//     else{
//         ESP_LOGE(ESP, "encountered UNEXPECTED EVENT");
//     }
// }
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
// void server_start(void){
//     httpd_config_t servconf = HTTPD_DEFAULT_CONFIG();
//     if(httpd_start(&server,&servconf) == ESP_OK){
//         //maak de 2 URI's
//         httpd_uri_t wagen_get = {
//             .uri        ="/esp",
//             .method     =HTTP_GET,
//             .handler    =get_handler,
//             .user_ctx   =NULL
//         };
//         httpd_uri_t wagen_set = {
//             .uri        ="/pse",
//             .method     =HTTP_POST,
//             .handler    =set_handler,
//             .user_ctx   =NULL
//         };
//         httpd_register_uri_handler(server, &wagen_get);
//         httpd_register_uri_handler(server, &wagen_set);
        
//     }
// }

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