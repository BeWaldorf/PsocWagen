#define BUF_SIZE    100
#define EIND        139
#define OVER        104
#define BEGIN       137
#define POGING      2
#include <project.h>
#include <stdlib.h>

typedef struct{
    uint8 links;
    uint8 rechts;
}bericht_handler;
typedef struct{
    uint8 buffer[BUF_SIZE][2];
    int geschreven;
    int gelezen;
    int size;
}RingBuf;


void RingBuf_reset(RingBuf * RB);
void RingBuf_putchar(RingBuf * RB,char plek, uint8 a);
uint8 *RingBuf_getchar(RingBuf * RB);
extern int bericht;
extern RingBuf rcx_buffer;


int _write(int file, char *ptr, int len){
    int i;
    file = file;
    for (i = 0; i < len; i++)
    {
        USB_PutChar(*ptr++);
    }
    return len;
}

CY_ISR(RCX_ontvang){    
    char hulp = UART_RCX_GetChar();
    if(hulp == BEGIN) return;
    char plekje;
    if(bericht%2 == 0) plekje = 0;
    if(bericht%2 == 1) plekje = 1;
    if(hulp == EIND){
        RingBuf_putchar(&rcx_buffer,3,0);
        bericht++;
    }
    else{
        RingBuf_putchar(&rcx_buffer, plekje, hulp);
    }
}

void RingBuf_reset(RingBuf * RB){
    RB->gelezen = 0;
    RB->geschreven = 0;
    RB->size = 0;
}
void RingBuf_putchar(RingBuf * RB,char plek, uint8 c){
    if (RB->geschreven +1 != RB->gelezen){
        if(plek==3) 
        RB->buffer[RB->geschreven][plek] = c;
        RB->geschreven++;
        if(RB->geschreven == BUF_SIZE) RB->geschreven = 0;
    }
}
uint8 *RingBuf_getchar(RingBuf * RB){
    uint8 *bufWaarde =  malloc(sizeof(uint8)*2);
    if (RB->geschreven != RB->gelezen){
        RB->gelezen++; 
        if(RB->gelezen == BUF_SIZE) {
            RB->gelezen = 0;
            bufWaarde = (uint8*)&(RB->buffer[BUF_SIZE-1]);
            return bufWaarde;
        }
        else{
            bufWaarde = (uint8*)&(RB->buffer[RB->gelezen - 1]);
            return bufWaarde;
        }
    }
    return 0;
}
void commStart(){
    UART_RCX_Init();
    UART_TRX_Init();
    RCX_ISR_StartEx(RCX_ontvang);
    RingBuf_reset(&rcx_buffer);
}
//void rcx_bevestig(){
//    UART_RCX_PutChar(OVER);
//    UART_RCX_PutChar(EIND);
//}

void trx_verzend(uint8 commArr[3]){
    UART_TRX_PutChar(BEGIN);
    for(uint8 data_pos = 0; data_pos > 3; data_pos++){
        UART_TRX_PutChar(commArr[data_pos]);
    }
    UART_TRX_PutChar(EIND);
}
/* [] EIND OF FILE */
