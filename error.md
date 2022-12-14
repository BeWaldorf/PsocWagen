
I (22895) TX_TASK: status: �0  en 3 verstuurd
I (24895) TX_TASK: status: �0  en 3 verstuurd
I (26895) TX_TASK: status: �0  en 3 verstuurd
E (26895) task_wdt: Task watchdog got triggered. The following tasks/users did not reset the watchdog in time:
E (26895) task_wdt:  - IDLE (CPU 1)
E (26895) task_wdt: Tasks currently running:
E (26895) task_wdt: CPU 0: IDLE
E (26895) task_wdt: CPU 1: rcx_uart_task
E (26895) task_wdt: Print CPU 1 backtrace


Backtrace: 0x4008505E:0x3FFB20C0 0x40082BD5:0x3FFB20E0 0x400D71E5:0x3FFC8EC0 0x400D70CD:0x3FFC8EF0 0x4008CA9D:0x3FFC8F10
0x4008505e: esp_crosscore_isr at C:/Users/df16/esp/esp-idf/components/esp_system/crosscore_int.c:96

0x40082bd5: _xt_lowint1 at C:/Users/df16/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/xtensa_vectors.S:1117

0x400d71e5: rx_task_maker at G:/project/PsocWagen/main/ESPuart.c:55

0x400d70cd: rx_task at G:/project/PsocWagen/main/main.c:32

0x4008ca9d: vPortTaskWrapper at C:/Users/df16/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:151


I (28895) TX_TASK: status: �0  en 3 verstuurd
I (30895) TX_TASK: status: �0  en 3 verstuurd
E (31895) task_wdt: Task watchdog got triggered. The following tasks/users did not reset the watchdog in time:
E (31895) task_wdt:  - IDLE (CPU 1)
E (31895) task_wdt: Tasks currently running:
E (31895) task_wdt: CPU 0: IDLE
E (31895) task_wdt: CPU 1: rcx_uart_task
E (31895) task_wdt: Print CPU 1 backtrace


Backtrace: 0x4008505E:0x3FFB20C0 0x40082BD5:0x3FFB20E0 0x400D7215:0x3FFC8EC0 0x400D70CD:0x3FFC8EF0 0x4008CA9D:0x3FFC8F10
0x4008505e: esp_crosscore_isr at C:/Users/df16/esp/esp-idf/components/esp_system/crosscore_int.c:96

0x40082bd5: _xt_lowint1 at C:/Users/df16/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/xtensa_vectors.S:1117

0x400d7215: rx_task_maker at G:/project/PsocWagen/main/ESPuart.c:66

0x400d70cd: rx_task at G:/project/PsocWagen/main/main.c:32

0x4008ca9d: vPortTaskWrapper at C:/Users/df16/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:151


I (32895) TX_TASK: status: �0  en 3 verstuurd
I (34895) TX_TASK: status: �0  en 3 verstuurd
I (36895) TX_TASK: status: �0  en 3 verstuurd
E (36895) task_wdt: Task watchdog got triggered. The following tasks/users did not reset the watchdog in time:
E (36895) task_wdt:  - IDLE (CPU 1)
E (36895) task_wdt: Tasks currently running:
E (36895) task_wdt: CPU 0: IDLE
E (36895) task_wdt: CPU 1: rcx_uart_task
E (36895) task_wdt: Print CPU 1 backtrace


Backtrace: 0x4008505E:0x3FFB20C0 0x40082BD5:0x3FFB20E0 0x400D7215:0x3FFC8EC0 0x400D70CD:0x3FFC8EF0 0x4008CA9D:0x3FFC8F10
0x4008505e: esp_crosscore_isr at C:/Users/df16/esp/esp-idf/components/esp_system/crosscore_int.c:96

0x40082bd5: _xt_lowint1 at C:/Users/df16/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/xtensa_vectors.S:1117

0x400d7215: rx_task_maker at G:/project/PsocWagen/main/ESPuart.c:66

0x400d70cd: rx_task at G:/project/PsocWagen/main/main.c:32

0x4008ca9d: vPortTaskWrapper at C:/Users/df16/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:151


I (38895) TX_TASK: status: �0  en 3 verstuurd
I (40895) TX_TASK: status: �0  en 3 verstuurd