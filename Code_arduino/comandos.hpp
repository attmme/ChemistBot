#ifndef _comandos_hpp_
#define _comandos_hpp_

// Defines
#define _BAUDRATE 9600
#define _TEMPS_TIMEOUT 50
#define MAX_SIZE_BUFFER 128
#define SI 1
#define NO 0

#define NUM_CARTUTXO 0
#define NUM_PASTILLES 1

// Variables globals
extern char _buffer[MAX_SIZE_BUFFER];
extern bool hi_ha_dada;

extern int cartutxo;
extern int n_pastilles;

// Funcions
void comando_init();
void pooling_comando();
int mirar_comando();
void comando_gestionar_errors();
bool comando_new_data();
void comando_flush();

// Comandos fets servir per a la balança / genèrics
void comando_si_no(int si_no);
void comando_calibrar();
void comando_tara();
void comando_pes();

void print_si_no();
void print_error_comando();

void comando_dosificar_pastilla();

int get_numero(String text, int n);

#endif
