#include "Arduino.h"
#include "strings_comandos.h"
#include "robot.hpp"
#include "comandos.hpp"
#include <EEPROM.h>

// Variables globals
char _buffer[MAX_SIZE_BUFFER];
bool hi_ha_dada = false;

Robot _cmd_balanca;

void comando_init()
{
	Serial.begin(_BAUDRATE);
	Serial.setTimeout(_TEMPS_TIMEOUT);

	pinMode(12, OUTPUT); // tmp debug
}

void pooling_comando() // switch on es mira quin comando és el que s'ha rebut
{
	if (comando_new_data())
	{
		switch (mirar_comando())
		{
		case COMANDO_SI:
			comando_si_no(SI);
			break;

		case COMANDO_NO:
			comando_si_no(NO);
			break;

		case BALANCA_CALIB:
			comando_calibrar();
			break;

		case BALANCA_TARA:
			comando_tara();
			break;

		case BALANCA_GET_PES:
			comando_pes();
			break;

		case COMANDO_TEMPORAL:
			_cmd_balanca.variable_temporal = 5;
		break;

		default:
			comando_gestionar_errors();
			break;
		}

		comando_flush();
	}
}

int mirar_comando() // Per a afegir nous comandos, anar a strings_comandos.h
{
	for (int i = 0; i < sizeof(_buffer); i++)
	{
		if (comando[i].equals(_buffer))
		{
			return i;
		}
	}

	return ERROR;
}

void comando_gestionar_errors()
{
	if (_cmd_balanca.calibrant == 1) //  Si estem calibrant
	{
		print_error_comando();
	}
	else // Si no estem fent cap procés
	{
		Serial.println(str_err_cmd_not_exists);
	}
}

bool comando_new_data()
{
	return hi_ha_dada;
}

void comando_flush()
{
	for (int i = 0; i < sizeof(_buffer); i++) // netejem la part del string feta servir
	{
		_buffer[i] = 0;
	}

	hi_ha_dada = false; // no hi ha nova dada
}

// Comandos fets servir per a la balança / genèrics

void comando_si_no(int si_no)
{
	if (_cmd_balanca.calibrant == 1) // Si estem calibrant
	{
		_cmd_balanca.step_calib++;
		si_no ? _cmd_balanca.si++ : _cmd_balanca.no++;
		_cmd_balanca.calibrar();
	}
	else // Si no estem fent cap procés
	{
		Serial.println(str_err_cmd_not_exists);
	}
}

void comando_calibrar()
{
	_cmd_balanca.calibrar();
}

void comando_tara()
{
	Serial.println("Tara");
	_cmd_balanca.tara();
}

void comando_pes()
{
	Serial.print("Pes: ");
	Serial.println(_cmd_balanca.llegir_pes());
}

void print_si_no()
{
	if (_cmd_balanca.si)
	{
		Serial.println("si\n");
	}
	else if (_cmd_balanca.no)
	{
		Serial.println("no\n");
	}
}

void print_error_comando()
{
	Serial.println("No t'entenc, intenta-ho de nou");
	_cmd_balanca.calibrar();
}

//IRQ. Quan hi ha una IRQ de nou paquet per la UART, entra aqui i va llegint fins al salt de línia (\n)
void serialEvent()
{
	Serial.readBytesUntil('\n', _buffer, MAX_SIZE_BUFFER);
	hi_ha_dada = true;
}
