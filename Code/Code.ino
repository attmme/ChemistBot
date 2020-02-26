// External libraries:
// HX711_Arduino_Library

#include "modos_debug.h"
#include "driver_balanca.hpp"
#include "protocol.hpp"
#include "comandos.hpp"
#include "ETH.h"

unsigned char ip[4] = {192, 168, 1, 7};
char mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

Bascula balanca;
ETH web;

void setup()
{
	protocol_init();
	comando_init();
	balanca.init();

	web.init(ip, mac);

}

void loop()
{
	pooling_comando(); // comandos.cpp

	//balanca.llegir_pes();
	//delay(250);
	//balanca_read_pes(); // temporal debug
}
