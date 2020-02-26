#ifndef _strings_comandos_h_
#define _strings_comandos_h_

// Defines
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
