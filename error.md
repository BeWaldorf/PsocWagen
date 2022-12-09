E (24978) uart: uart_read_bytes(1281): uart driver error
E (24988) uart: uart_read_bytes(1281): uart driver error
E (24988) uart: uart_read_bytes(1281): uart driver error
E (24998) uart: uart_read_bytes(1281): uart driver error
E (24998) uart: uart_read_bytes(1281): uart driver error
E (25008) uart: uart_read_bytes(1281): uart driver error
E (25018) uart: uart_read_bytes(1281): uart driver error
E (25018) uart: uart_read_bytes(1281): uart driver error
E (25028) uart: uart_read_bytes(1281): uart driver error
E (25028) uart: uart_read_bytes(1281): uart driver error
E (25038) uart: uart_read_bytes(1281): uart driver error
E (25048) uart: uart_read_bytes(1281): uart driver error
E (25048) uart: uart_read_bytes(1281): uart driver error
E (25058) uart: uart_read_bytes(1281): uart E (25058) task_wdt: Task watchdog got triggered. The following tasks/users did not 
reset the watchdog in time:
E (25058) task_wdt:  - IDLE (CPU 0)
E (25058) task_wdt: Tasks currently running:
E (25058) task_wdt: CPU 0: rcx_uart_task
E (25058) task_wdt: CPU 1: IDLE
E (25058) task_wdt: Print CPU 0 (current core) backtrace


Backtrace: 0x400DAF17:0x3FFB1A90 0x400DB0A4:0x3FFB1AB0 0x40082BD5:0x3FFB1AD0 0x400D68C7:0x3FFC81C0 0x400D7097:0x3FFC81F0 0x400D5F83:0x3FFC8210 0x400D5572:0x3FFC8230 0x4000BD83:0x3FFC8250 0x4000117D:0x3FFC8270 0x400592FE:0x3FFC8290 0x4005937A:0x3FFC82B0 0x401412E7:0x3FFC82D0 0x4014CC9B:0x3FFC8300 0x4014837E:0x3FFC8320 0x401484F9:0x3FFC8630 0x40153359:0x3FFC8660 0x4008EE8D:0x3FFC8690 0x400D9441:0x3FFC86E0 0x400D7204:0x3FFC8730 0x4008CA9D:0x3FFC8760
0x400daf17: task_wdt_timeout_handling at C:/Users/df16/esp/esp-idf/components/esp_system/task_wdt.c:486 (discriminator 3)

0x400db0a4: task_wdt_isr at C:/Users/df16/esp/esp-idf/components/esp_system/task_wdt.c:586

0x40082bd5: _xt_lowint1 at C:/Users/df16/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/xtensa_vectors.S:1117

0x400d68c7: uart_ll_get_txfifo_len at C:/Users/df16/esp/esp-idf/components/hal/esp32/include/hal/uart_ll.h:299 (discriminator 1)
 (inlined by) uart_tx_char at C:/Users/df16/esp/esp-idf/components/vfs/vfs_uart.c:156 (discriminator 1)

0x400d7097: uart_write at C:/Users/df16/esp/esp-idf/components/vfs/vfs_uart.c:209

0x400d5f83: console_write at C:/Users/df16/esp/esp-idf/components/vfs/vfs_console.c:73

0x400d5572: esp_vfs_write at C:/Users/df16/esp/esp-idf/components/vfs/vfs.c:436 (discriminator 4)

0x401412e7: __sfvwrite_r at /builds/idf/crosstool-NG/.build/HOST-x86_64-w64-mingw32/xtensa-esp32-elf/src/newlib/newlib/libc/stdio/fvwrite.c:251

0x4014cc9b: __sprint_r at /builds/idf/crosstool-NG/.build/HOST-x86_64-w64-mingw32/xtensa-esp32-elf/src/newlib/newlib/libc/stdio/vfprintf.c:429
 (inlined by) __sprint_r at /builds/idf/crosstool-NG/.build/HOST-x86_64-w64-mingw32/xtensa-esp32-elf/src/newlib/newlib/libc/stdio/vfprintf.c:399

0x4014837e: _vfprintf_r at ??:?

0x401484f9: vprintf at /builds/idf/crosstool-NG/.build/HOST-x86_64-w64-mingw32/xtensa-esp32-elf/src/newlib/newlib/libc/stdio/vprintf.c:34 (discriminator 5)

0x40153359: esp_log_writev at C:/Users/df16/esp/esp-idf/components/log/log.c:200

0x4008ee8d: esp_log_write at C:/Users/df16/esp/esp-idf/components/log/log.c:210

0x400d9441: uart_read_bytes at C:/Users/df16/esp/esp-idf/components/driver/uart.c:1281 (discriminator 5)

0x400d7204: rx_task at G:/project/wagenESPRTOS/main/main.c:97

0x4008ca9d: vPortTaskWrapper at C:/Users/df16/esp/esp-idf/components/freertos/FreeRTOS-Kernel/portable/xtensa/port.c:151


driver error
E (25138) uart: uart_read_bytes(1281): uart driver error
E (25138) uart: uart_read_bytes(1281): uart driver error
E (25148) uart: uart_read_bytes(1281): uart driver error