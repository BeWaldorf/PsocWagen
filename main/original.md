<h1>ESP server post handler</h1>
esp_err_t set_handler(httpd_req_t *req)
{
    char godot_bericht[64];
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

<h1>UART ontvanger task error</h1>
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
            rcx_data[rxBytes] = COMM_EINDE;
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