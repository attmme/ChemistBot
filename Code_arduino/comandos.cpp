#include "Arduino.h"
#include "strings_comandos.h"
#include "robot.hpp"
#include "comandos.hpp"
#include <EEPROM.h>
#include "Musica.hpp"

// Variables globals
char _buffer[MAX_SIZE_BUFFER];
bool hi_ha_dada = false;

int n_cartutxos;
int cartutxos[6];
int pastilles[6];

String buffer_params_cartutxos;
Robot robotet;
Musica _musica;

// Public
void comando_init()
{
	Serial.begin(_BAUDRATE);
	Serial.setTimeout(_TEMPS_TIMEOUT);
	robotet.init();
	robotet.tambor(DESBLOQ);
	_musica.init();
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
			get_valors_cartutxos(buffer_params_cartutxos);
			comando_dosificar_pastilla();
			break;

		case ROBOT_LLEGIR:
			Serial.print("Nº cartutxo: ");
			Serial.println(comando_llegir_cartutxo(), DEC);
			break;

		case SO_OK:
			_musica.reproduir(MUSIC_UP);
			break;

		case SO_ERROR:
			_musica.reproduir(MUSIC_ERROR);
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
			buffer_params_cartutxos = _buffer;
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

void comando_dosificar_pastilla() // array nº cartutxos + nº pastilles
{
	robotet.tambor(BLOQ);
	int error = 1;

	for (int i = 0; i < n_cartutxos; i++) // per cada cartutxo, donaràs una volta completa al tambor
	{
		error = 1;
		Serial.print("Buscant cartutxo: ");
		Serial.println(cartutxos[i]);

		for (int j = 0; j < 7; j++)
		{
			if (cartutxos[i] == robotet.llegir_cartutxo())
			{
				Serial.println("Debug: Cartutxo trovat, dosificar");
				robotet.tara();
				error = _dosifica(pastilles[i]);

				if (error == 0)
				{
					j = 7; // pirem d'una forma molt elegant per a menjar-nos el girar_un d'abaix

					if( i == n_cartutxos-1 ) // si és l'últim cas, no volem el girar 1, pirem
					{
						break;
					}
				}
				else // error
				{
					i = 255; // sortim dels 2 bucles d'un plumazo
					break;
				}
			}
			robotet.girar_un(); // gira 1 posició
		}
	}

	robotet.girar(false);
	robotet.tambor(DESBLOQ);
	_gestio_errors(error);
}

int _dosifica(int n_cops)
{
	int error = 1;

	for (int i = 0; i < n_cops; i++) // dosificar
	{
		for (int j = 0; j < 2; j++) // intentar x cops
		{
			robotet.dosificar_pastilla();
			float pes_nou = robotet.llegir_pes();
			Serial.print("Debug: pes pasti");
			Serial.println(pes_nou);

			if (pes_nou < 0.15f) // no ha caigut pastilla, intentarem 1 cop
			{
				Serial.println("Error: no ha caigut pastilla!");
				error = 1;
			}
			else // tot bé, ha caigut
			{
				Serial.println("Debug: tot be, anem a pel pròxim cartutxo");
				error = 0;
				break;
			}
		}
	}

	return error;
}

void _gestio_errors(int error)
{
	if (error == 1)
	{
		Serial.println("Error: s'han mirat tots els cartutxos, no existeix");
		_musica.reproduir(MUSIC_ERROR);
	}
	else
	{
		Serial.println("Debug: music up");
		_musica.reproduir(MUSIC_UP);
	}
}

int comando_llegir_cartutxo()
{
	int n = 0;
	robotet.girar(false);
	n = robotet.llegir_cartutxo();
	return n;
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

void get_valors_cartutxos(String t_buffer)
{
	n_cartutxos = ((t_buffer.length() - 9) / 4);

	int j = 0;
	for (int i = 10; i < t_buffer.length(); i += 4)
	{
		cartutxos[j] = _buffer[i] - 48, DEC;
		pastilles[j] = _buffer[i + 2] - 48, DEC;
		j++;
	}

	cartutxos[j] = 7;
}

//IRQ. Quan hi ha una IRQ de nou paquet per la UART, entra aqui i va llegint fins al salt de línia (\n)
void serialEvent()
{
	Serial.readBytesUntil('\n', _buffer, MAX_SIZE_BUFFER);
	hi_ha_dada = true;
}
