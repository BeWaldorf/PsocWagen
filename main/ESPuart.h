extern char data_wagen[4];
extern char data_godot[3];

int sendData(const char *logNaam, const char *trx_data);
void tx_task_maker(void *arg);
void rx_task_maker(void *arg);
void uart_start(void);