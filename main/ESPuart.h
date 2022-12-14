extern char data_wagen[3];
extern char data_godot[2];

int sendData(const char *logNaam, const char *trx_data);
void tx_task_maker(void *arg);
void rx_task_maker(void *arg);
void uart_start(void);