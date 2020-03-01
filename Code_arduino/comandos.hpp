#ifndef _comandos_hpp_
#define _comandos_hpp_

// Han de coincidir amb el:
// String comando[NUM_MAX_COMANDOS] de strings_comandos.h
#define COMANDO_SI 0
#define COMANDO_NO 1
#define BALANCA_CALIB 2
#define BALANCA_TARA 3
#define BALANCA_GET_PES 4

#define COMANDO_LED_ON  5
#define COMANDO_LED_OFF  6

#define ERROR -1

// Funcions
void comando_init();
void pooling_comando();
int mirar_comando();
void comando_gestionar_errors();
void comando_si();
void comando_no();

#endif
