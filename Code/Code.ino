// External libraries:
// HX711_Arduino_Library

#include <Ethernet.h>
#include <EthernetServer.h>
#include <EthernetClient.h>

#include "modos_debug.h"
#include "driver_balanca.hpp"
#include "protocol.hpp"
#include "comandos.hpp"


Bascula balanca;

void setup()
{
	protocol_init();
	comando_init();
	balanca.init();

}

void loop()
{
	pooling_comando(); // comandos.cpp
	
	//balanca.llegir_pes();
	//delay(250);
	//balanca_read_pes(); // temporal debug
}
