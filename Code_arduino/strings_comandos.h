#ifndef _strings_comandos_h_
#define _strings_comandos_h_

// Defines
#define COMANDO_SI 0		// El número ha de concidir amb el string comando[]
#define COMANDO_NO 1
#define BALANCA_CALIB 2
#define BALANCA_TARA 3
#define BALANCA_GET_PES 4

#define COMANDO_LED_ON 5
#define COMANDO_LED_OFF 6

#define ERROR -1

#define NUM_MAX_COMANDOS 7

// Strings comandos
String comando[NUM_MAX_COMANDOS] =
	{
		"si",		// i = 0
		"no",		// i = 1
		"calibrar", // i = 2
		"tara",		// i = 3
		"pes",		// i = 4
		"on",		// son de debug, i = 5
		"off"		//               i = 6

};

// Strings d'errors
String str_err_cmd_not_exists = "ERROR: Comando no existeix";

#endif
