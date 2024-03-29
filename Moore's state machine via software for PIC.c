#include "pic18f4520.h"
#include "config.h"
#include "pwm.h"
#include "keypad.h"
#include "lcd.h"
#include "ssd.h"
#include "serial.h"
#include "timer.h"
#include "adc.h"

void LeTeclado(void) {
    char nome[7] = "FELIPE";
    char matricula[7] = "123456";
    unsigned int tecla = 16;
    int i = 0;
    if (kpRead() != tecla) {
        tecla = kpRead();
        if (BitTst(tecla, 0)) {
            lcdCommand(0x80);
            while (nome[i] != '\0') {
                lcdData(nome[i]);
                i++;
            }
            i = 0;


        }
        if (BitTst(tecla, 1)) {
            lcdCommand(0xC0);
            while (matricula[i] != '\0') {
                lcdData(matricula[i]);
                i++;
            }
            i = 0;
        }

        if (BitTst(tecla, 6)) {
            pwmSet1(75);
        }
        if (BitTst(tecla, 7)) {
            pwmSet1(0);
        }
        if (BitTst(tecla, 2)) {
            lcdCommand(0x01);
        }



    }


}

void Serial(void) {

    char tecla;
    int temp;
    char nome[7] = "FELIPE";
    char matricula[7] = "123456";
    int i = 0;
    tecla = serialRead();

    if (tecla == 'A') {
        
        while (nome[i] != '\0') {
            serialSend(nome[i]);
            i++;
        }
        i = 0;
        serialSend('\r');
    }

    if (tecla == 'B') {
        
        while (matricula[i] != '\0') {
            serialSend(matricula[i]);
            i++;
        }
        i = 0;
        serialSend('\r');
    }



    if (tecla == 'C') {
        pwmSet1(75);
    }
    if (tecla == 'D') {
        pwmSet1(0);
    }
    if (tecla == '0') {
        lcdCommand(0x01);
    }

    if(tecla == 'T'){
    temp = adcRead() / 2;
    serialSend('T');
    serialSend(':');
    serialSend(((temp / 10) % 10) + 48);
    serialSend((temp % 10) + 48);
    serialSend(13);
    serialSend('\r');
    }

}

void MostraDisplay(void) {
    int temp;
    
    temp = adcRead() / 2;
    
    ssdDigit((((temp / 10) % 10)), 1);
    ssdDigit(((temp % 10)), 0);
    ssdUpdate();
    
    
    
    
}

void main(void) {
    char slot;
    kpInit();
    adcInit();
    pwmInit();
    ssdInit();
    lcdInit();
    serialInit();
    timerInit();
    int tempo;
    //inicializações
    for (;;) {
        timerReset(5000);
        ssdUpdate();
        switch (slot) {
            case 0:
                LeTeclado();
                slot = 1;
                break;
            case 1:
                Serial();
                slot = 2;
                break;
            case 2:
                kpDebounce();
                slot = 3;
                break;
            case 3:
                MostraDisplay();
                //for (tempo = 0;tempo < 10;tempo++);
                slot = 0;
                break;
            default:
                slot = 0;
                break;

        }
        timerWait();
    }
}