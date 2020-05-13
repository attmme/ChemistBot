#include "Arduino.h"
#include "strings_comandos.h"
#include "robot.hpp"
#include "comandos.hpp"
#include <EEPROM.h>

// Variables globals
char _buffer[MAX_SIZE_BUFFER];
bool hi_ha_dada = false;

int cartutxo = 0;
int n_pastilles = 0;

Robot robotet;

// Public
void comando_init()
{
	Serial.begin(_BAUDRATE);
	Serial.setTimeout(_TEMPS_TIMEOUT);
	robotet.init();
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

		case ROBOT_DOSIFICAR:
			comando_dosificar_pastilla();
			break;

		default:
			comando_gestionar_errors();
			break;
		}

		comando_flush();
	}
}

// Private
int mirar_comando() // Per a afegir nous comandos, anar a strings_comandos.h
{
	for (int i = 0; i < NUM_MAX_COMANDOS; i++)
	{
		if (comando[i].equals(_buffer)) // comandos sense paràmetres
		{
			return i;
		}
		else if (strstr(_buffer, comando[ROBOT_DOSIFICAR].c_str()) != NULL) // if comando dosificar
		{
			cartutxo = get_numero(_buffer, NUM_CARTUTXO);
			n_pastilles = get_numero(_buffer, NUM_PASTILLES);

			return ROBOT_DOSIFICAR;
		}
	}

	return ERROR;
}

void comando_gestionar_errors()
{
	if (robotet.calibrant == 1) //  Si estem calibrant
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
	if (robotet.calibrant == 1) // Si estem calibrant
	{
		robotet.step_calib++;
		si_no ? robotet.si++ : robotet.no++;
		robotet.calibrar();
	}
	else // Si no estem fent cap procés
	{
		Serial.println(str_err_cmd_not_exists);
	}
}

void comando_calibrar()
{
	robotet.calibrar();
}

void comando_tara()
{
	Serial.println("Info: Tara");
	robotet.tara();
}

void comando_pes()
{
	Serial.print("Info: Pes: ");
	Serial.println(robotet.llegir_pes());
}

void print_si_no()
{
	if (robotet.si)
	{
		Serial.println("Info: si\n");
	}
	else if (robotet.no)
	{
		Serial.println("Info: no\n");
	}
}

void print_error_comando()
{
	Serial.println("Error: No t'entenc, intenta-ho de nou");
	robotet.calibrar();
}

void comando_dosificar_pastilla()
{
	int error = 1;

	robotet.girar(true);

	for (int i = 0; i < 7; i++)
	{
		while (!robotet.davant_cartutxo())
			;

		robotet.girar(false);
		robotet.tambor(BLOQ);

		if (robotet.llegir_cartutxo() == cartutxo)
		{
			Serial.println("Debug: Cartutxo trovat, dosificar");

			for (int j = 0; j < n_pastilles; j++)
			{
				robotet.dosificar_pastilla();
			}
			
			robotet.tambor(DESBLOQ);
			error = 0;
			break;
		}
		else
		{
			robotet.tambor(DESBLOQ);
			robotet.girar(true);
		}
	}

	robotet.girar(false);

	if (error == 1)
	{
		Serial.println("Error: s'han mirat tots els cartutxos, no existeix");
	}
}

// altres
int get_numero(String text, int n)
{
	int y = 0;
	String arr_numeros[2];
	int numeros[2];

	String tmp = text.substring(comando[ROBOT_DOSIFICAR].length() + 1, text.length());

	for (int x = 0; x < tmp.length(); x++)
	{
		if (tmp[x] == ' ')
		{
			y++;
			continue;
		}
		arr_numeros[y].concat(tmp[x]);
	}

	return arr_numeros[n].toInt();
}

//IRQ. Quan hi ha una IRQ de nou paquet per la UART, entra aqui i va llegint fins al salt de línia (\n)
void serialEvent()
{
	Serial.readBytesUntil('\n', _buffer, MAX_SIZE_BUFFER);
	hi_ha_dada = true;
}
