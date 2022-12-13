//switch plakker
//switch (kant){
//    case (LINKS):
//        break;
//    case (RECHTS):
//        break; 
#include <project.h>
#include <UART.h>

#define LINKS       0
#define RECHTS      1



typedef void(*kant_start)();
typedef void(*kant_stop)();
typedef uint8(*kant_read_comp)();
typedef void(*kant_write_comp)(uint8);
//uint8= char vr UART!

struct Kanten{
    kant_start starter;
    kant_stop stopper;
    kant_read_comp comp_reader;
    kant_write_comp comp_writer;
}Kant;

struct Kanten links;
struct Kanten rechts;
uint8 kantArr[2] = {LINKS,RECHTS};
uint8 lijnArr[3] = {2,3,5};
uint8 vermogensArr[2]={0,0};
uint8 commArr[3];

void wagen_init(){
    links.starter = &VermogenLinks_Start;
    links.stopper = &VermogenLinks_Stop;
    links.comp_reader = &VermogenLinks_ReadCompare;
    links.comp_writer = &VermogenLinks_WriteCompare;

    rechts.starter = &VermogenRechts_Start;
    rechts.stopper = &VermogenRechts_Stop;
    rechts.comp_reader = &VermogenRechts_ReadCompare;
    rechts.comp_writer = &VermogenRechts_WriteCompare;   
}

void startMotor(uint8 kant){
    switch (kant){
        case (LINKS):
            (*links.starter)();
            break;
        case (RECHTS):
            (*rechts.starter)();
            break;
    }
}

void herstartMotor(uint8 kant){
    switch (kant){
        case (LINKS):
            (*links.stopper)();
            (*links.starter)();
            break;
        case (RECHTS):
            (*rechts.stopper)();
            (*rechts.starter)();
            break;
    }
}

void setVermogen(uint8 kant,uint8 vermogen){
    if(vermogen >100) vermogen = 100; 
    switch (kant){
        case (LINKS):
        (*links.comp_writer)(vermogen);
            break;
        case (RECHTS):
            (*rechts.comp_writer)(vermogen);
            break;
    }
}

uint8 getVermogen(uint8 kant){
    uint8 vermogen=255;
    switch (kant){
        case (LINKS):
            vermogen = (*links.comp_reader)();
            break;
        case (RECHTS):
            vermogen = (*rechts.comp_reader)();
            break;
    }
    return vermogen;
}

uint8 lijnCheck(){
    uint8 lijn   = GeenIsr0_Read() * lijnArr[0]
                + GeenIsr1_Read() * lijnArr[1]
                + GeenIsr2_Read() * lijnArr[2];
    return lijn;
}   
void commArrMaker(){
    commArr[0] = lijnCheck();
    commArr[1] = getVermogen(LINKS);
    commArr[2] = getVermogen(RECHTS);
}

void snelheidsregelaar(uint8 linker,uint8 rechter){
    setVermogen(LINKS,linker);
    setVermogen(RECHTS,rechter);
}
void wagen_start(){
    wagen_init();
    startMotor(kantArr[LINKS]);
    setVermogen(kantArr[LINKS],vermogensArr[LINKS]);
    startMotor(kantArr[RECHTS]);
    setVermogen(kantArr[RECHTS],vermogensArr[RECHTS]);
    commStart();
}

/* [] END OF FILE */
