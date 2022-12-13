#define BUF_SIZE 100
#define BEGIN       137
#define EIND        139

typedef unsigned char   uint8;
typedef struct{
    char buffer[BUF_SIZE];
    int geschreven;
    int gelezen;
    int size;
}RingBuf;
typedef struct{
    uint8 links;
    uint8 rechts;
}bericht_handler;

extern int bericht;
extern RingBuf rcx_buffer;

void RingBuf_reset(RingBuf * RB);
void RingBuf_putchar(RingBuf * RB,char a);
char RingBuf_getchar(RingBuf * RB);
int _write(int file, char *ptr, int len);
void commStart();
void trx_verzend(uint8 commArr[3]);
/* [] END OF FILE */
