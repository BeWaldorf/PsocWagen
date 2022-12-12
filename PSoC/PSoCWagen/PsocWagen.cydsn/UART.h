#define BUF_SIZE 100
#define BEGIN       137
#define EIND        139

typedef struct{
    char buffer[BUF_SIZE];
    int schrijf;
    int lees;
    int size;
}RingBuf;

extern int bericht;
extern RingBuf BufUart;

void RingBuf_reset(RingBuf * RB);
void RingBuf_putchar(RingBuf * RB,char a);
char RingBuf_getchar(RingBuf * RB);
int _write(int file, char *ptr, int len);
void commStart();
/* [] END OF FILE */
