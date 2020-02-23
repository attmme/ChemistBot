#ifndef _protocol_hpp_
#define _protocol_hpp_

// Defines
	#define _BAUDRATE 		9600
	#define _TEMPS_TIMEOUT	50
	#define MAX_SIZE_BUFFER	128

// Variables globals
	extern char buffer_protocol[MAX_SIZE_BUFFER];




void protocol_init();
void protocol_write(char* text);
bool protocol_new_data();
void protocol_flush();


#endif
