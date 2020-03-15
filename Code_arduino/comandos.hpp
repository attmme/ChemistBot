#ifndef _comandos_hpp_
#define _comandos_hpp_

// Defines
#define _BAUDRATE 9600
#define _TEMPS_TIMEOUT 50
#define MAX_SIZE_BUFFER 128

// Variables globals
extern char _buffer[MAX_SIZE_BUFFER];

// Funcions
void comando_init();
void pooling_comando();
int mirar_comando();
void comando_gestionar_errors();
bool comando_new_data();
void comando_flush();

// Comandos fets servir per a la balança / genèrics
void comando_si_no(bool si_no);
void comando_calibrar();
void comando_tara();
void comando_pes();

void print_si_no();
void print_error_comando();

#endif
