#ifndef _strings_comandos_h_
#define _strings_comandos_h_

// Defines. El número ha de concidir amb el string comando[]
#define ERROR -1
#define COMANDO_SI 0
#define COMANDO_NO 1
#define BALANCA_CALIB 2
#define BALANCA_TARA 3
#define BALANCA_GET_PES 4
#define	ROBOT_DOSIFICAR 5
#define	ROBOT_LLEGIR 6
#define SO_OK 7
#define SO_ERROR 8

#define NUM_MAX_COMANDOS 9

// Tipus de missatges per la terminal:
//"Debug: "
//"Info: "
//"Error: "

// Strings comandos
String comando[NUM_MAX_COMANDOS] =
	{
		"si",		// i = 0
		"no",		// i = 1
		"calibrar", // i = 2
		"tara",		// i = 3
		"pes",		// i = 4
		"dosificar", // i = 5
		"llegir", // i = 6
		"so_ok", // i = 7
		"so_error", // i = 8
};

// Strings d'errors
String str_err_cmd_not_exists = "ERROR: Comando no existeix";

#endif
