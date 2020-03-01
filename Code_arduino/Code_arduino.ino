// External libraries:
// HX711_Arduino_Library

#include "modos_debug.h"
#include "driver_balanca.hpp"
#include "protocol.hpp"
#include "comandos.hpp"

Bascula balanca;

void setup()
{
	Serial.begin(9600);

	protocol_init();
	comando_init();
	balanca.init();
}

void loop()
{
	pooling_comando(); // comandos.cpp
}
