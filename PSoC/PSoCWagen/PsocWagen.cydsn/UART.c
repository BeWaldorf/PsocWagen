#define BUF_SIZE    100
#define EIND        139
#define OVER        104
#define BEGIN       137
#define POGING      2
#include <project.h>

typedef struct{
    char buffer[BUF_SIZE];
    int schrijf;
    int lees;
    int size;
}RingBuf;

void RingBuf_reset(RingBuf * RB);
void RingBuf_putchar(RingBuf * RB,char a);
char RingBuf_getchar(RingBuf * RB);
extern int bericht;
extern RingBuf BufUart;


int _write(int file, char *ptr, int len){
    int i;
    file = file;
    for (i = 0; i < len; i++)
    {
        UART_TRX_PutChar(*ptr++);
    }
    return len;
}

CY_ISR(vulBuffer){
    char temp = UART_RCX_GetChar();
    if(temp == EIND){
        RingBuf_putchar(&BufUart,0);
        bericht++;
    }
    else{
        RingBuf_putchar(&BufUart,temp);
    }
}

void RingBuf_reset(RingBuf * RB){
    RB->lees = 0;
    RB->schrijf = 0;
    RB->size = 0;
}
void RingBuf_putchar(RingBuf * RB, char c){
    if (RB->schrijf +1 != RB->lees){
        RB->buffer[RB->schrijf] = c;
        RB->schrijf++; if(RB->schrijf == BUF_SIZE) RB->schrijf = 0;
    }
}
char RingBuf_getchar(RingBuf * RB){
    if (RB->schrijf != RB->lees){
        RB->lees++; 
        if(RB->lees == BUF_SIZE) {
            RB->lees = 0;
            return RB->buffer[BUF_SIZE-1];
        }
        else{
            return RB->buffer[RB->lees - 1];
        }
    }
    return 0;
}
void commStart(){
    UART_RCX_Init();
    UART_TRX_Init();
}
void rcx_bevestig(){
    UART_RCX_PutChar(OVER);
    UART_RCX_PutChar(EIND);
}

void trx_verzend(uint8 commArr[3]){
    UART_TRX_PutChar(BEGIN);
    for(uint8 data_pos = 0; data_pos > 3 ; data_pos++){
        UART_TRX_PutChar(commArr[data_pos]);
    }
    UART_TRX_PutChar(EIND);
}
/* [] EIND OF FILE */
