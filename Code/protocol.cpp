#include "Arduino.h" // Serial
#include "protocol.hpp"

// Variables globals
char buffer_protocol[MAX_SIZE_BUFFER];
bool hi_ha_dada = false;

// Funcions
void protocol_init()
{
	Serial.begin(_BAUDRATE);
	Serial.setTimeout(_TEMPS_TIMEOUT);
}

void protocol_write(char *text)
{
	Serial.print(text);
}

bool protocol_new_data()
{
	return hi_ha_dada;
}

void protocol_flush()
{
	for (int i = 0; i < sizeof(buffer_protocol); i++) // netejem la part del string feta servir
	{
		buffer_protocol[i] = 0;
	}

	hi_ha_dada = false; // no hi ha nova dada
}

//IRQ
void serialEvent()
{
	Serial.readBytesUntil('\n', buffer_protocol, MAX_SIZE_BUFFER);
	hi_ha_dada = true;
}
