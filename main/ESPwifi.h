#include "esp_event.h"
#include "esp_http_server.h"
#include "esp_log.h"

extern char data_wagen[4];
extern char data_godot[3];

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
esp_err_t get_handler(httpd_req_t *req);
esp_err_t set_handler(httpd_req_t *req);
void wifi_start(void);
void server_start(void);