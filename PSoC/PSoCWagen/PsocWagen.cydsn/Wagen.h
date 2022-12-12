/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
typedef unsigned char   uint8;
#define                 LINKS   0
#define                 RECHTS  1
//char kantArr[2];
uint8 lijnArr[3];
uint8 vermogensArr[2];
uint8 commArr[3];
uint8 kantArr[2];


//typedef struct kanten{
//    void(*kant_start[1])();
//    void(*kant_stop[1])();
//    int(*kant_read_comp)();
//    void(*kant_write_comp)(int);
//    
//}kant;

//void wagen_init();
//void startMotor(uint8 kant);
void herstartMotor(uint8 kant);
void setVermogen(uint8 kant,uint8 vermogen);
uint8 getVermogen(uint8 kant);
uint8 lijnCheck();
void commArrMaker();
void snelheidsregelaar(uint8 linker,uint8 rechter);
void wagen_start();

/* [] END OF FILE */
