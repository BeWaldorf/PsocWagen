#include <stdio.h>
#include <string.h>

#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#define SSID "SuperNitro"
#define WIFI_PASS "SuperWagen5"
#define WIFI_SUCCES_BIT BIT0
#define WIFI_BUIS_BIT BIT1
#define ESP "piloot"
#define MAX_WIFI_POGING 25
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static httpd_handle_t server = NULL;
extern char data_wagen[3];
extern char data_godot[2];
static EventGroupHandle_t static_wifi_groep;
static int s_retry_num = 0;

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)       {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
        if (s_retry_num < MAX_WIFI_POGING){
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(ESP, " probeert verbinding te maken met circuit");
        }
        else{
            xEventGroupSetBits(static_wifi_groep, WIFI_BUIS_BIT);
        }
        ESP_LOGI(ESP, " kon geen circuit vinden.");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        ESP_LOGI(ESP, "woop woop");
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(ESP, " zijn IP:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(static_wifi_groep, WIFI_SUCCES_BIT);
    }
    else{
        ESP_LOGI(ESP,"WAP WAP");
        xEventGroupSetBits(static_wifi_groep, WIFI_SUCCES_BIT);
    }
}

esp_err_t get_handler(httpd_req_t *req)
{
    char antw[3];
    for (uint_fast8_t i = 0; i < 3; i++)
    {
        antw[i] = data_wagen[i];
    }
    httpd_resp_send(req, antw, HTTPD_RESP_USE_STRLEN);
    httpd_resp_set_type(req, (char)200);
    // return "ge weet toch da ge mij kunt betrouwen zeker"
    return ESP_OK;
}
esp_err_t set_handler(httpd_req_t *req)
{
    char godot_bericht[13];
    size_t recv_size = MIN(req->content_len, sizeof(godot_bericht));
    int controle = httpd_req_recv(req, godot_bericht, recv_size);

    if (controle <= 0)
    {
        if (controle == HTTPD_SOCK_ERR_TIMEOUT)
        {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }
    httpd_resp_send(req, godot_bericht, HTTPD_RESP_USE_STRLEN);
    for (uint_fast8_t i = 0; i < 2; i++)
    {
        data_godot[i] = godot_bericht[i];
    }
    godot_bericht[13] = '\0';
    ESP_LOGI(ESP, "waarde: %s", godot_bericht);
    return ESP_OK;
}

void wifi_start(void)
{
    static_wifi_groep = xEventGroupCreate();
    nvs_flash_init();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    wifi_config_t mijn_conf = {
        .sta = {
            .ssid = SSID,
            .password = WIFI_PASS,

            .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &mijn_conf));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(ESP, " heeft wifi succesvol gestart.");
    EventBits_t bits = xEventGroupWaitBits(static_wifi_groep,
                                           WIFI_SUCCES_BIT | WIFI_BUIS_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);
    if (bits & WIFI_SUCCES_BIT)
    {
        ESP_LOGI(ESP, "connected to ap SSID:%s",
                 SSID);
    }
    else if (bits & WIFI_BUIS_BIT)
    {
        ESP_LOGI(ESP, "Failed to connect to SSID:%s",
                 SSID);
    }
    else
    {
        ESP_LOGE(ESP, "encountered UNEXPECTED EVENT");
    }
}
void server_start(void)
{
    httpd_config_t servconf = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server, &servconf) == ESP_OK)
    {
        // maak de 2 URI's
        httpd_uri_t wagen_get = {
            .uri = "/esp",
            .method = HTTP_GET,
            .handler = get_handler,
            .user_ctx = NULL};
        httpd_uri_t wagen_set = {
            .uri = "/pse",
            .method = HTTP_POST,
            .handler = set_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &wagen_get);
        httpd_register_uri_handler(server, &wagen_set);
    }
}
