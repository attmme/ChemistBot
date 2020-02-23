#include "modos_debug.h" // debug
#include "Arduino.h"
#include "strings_comandos.h"
#include "protocol.hpp"
#include "comandos.hpp"
#include "driver_balanca.hpp"
#include <EEPROM.h>

Bascula _cmd_balanca;

void comando_init()
{
	protocol_init(); // per la modo debug
}

void pooling_comando()
{
	if( protocol_new_data() )
	{
		switch( mirar_comando() )
		{
			case COMANDO_SI:
				comando_si();
			break;
			
			case COMANDO_NO:
				comando_no();
			break;			
			
			case BALANCA_CALIB:
				_cmd_balanca.calibrar();
			break;
			
			case BALANCA_TARA:
				_cmd_balanca.tara();
			break;
			
			case BALANCA_GET_PES:
				Serial.print("Pes: ");
				Serial.println(_cmd_balanca.llegir_pes());
			break;
			
			default:
				comando_gestionar_errors();
			break;
		}
		
		protocol_flush();
	}
}

int mirar_comando()
{
	for(int i = 0; i < sizeof(buffer_protocol); i++)
	{
		if( comando[i].equals(buffer_protocol) )
		{
			return i;
		}
	}

	return ERROR;
}


// Gestió de comandos del switch d'adalt
void comando_si()
{
	if(_cmd_balanca.calibrant == 1) // Si estem calibrant
	{
		_cmd_balanca.step_calib++;
		_cmd_balanca.si++;
		_cmd_balanca.calibrar();
	}
	else // Si no estem fent cap procés
	{
		Serial.println(str_err_cmd_not_exists);
	}
}

void comando_no()
{
	if(_cmd_balanca.calibrant == 1) //  Si estem calibrant
	{
		_cmd_balanca.step_calib++;
		_cmd_balanca.no++;
		_cmd_balanca.calibrar();
	}
	else // Si no estem fent cap procés
	{
		Serial.println(str_err_cmd_not_exists);
	}
}

void comando_gestionar_errors()
{
	if(_cmd_balanca.calibrant == 1) //  Si estem calibrant
	{
		_cmd_balanca.error_comando();
	}
	else // Si no estem fent cap procés
	{
		Serial.println(str_err_cmd_not_exists);
	}
}

