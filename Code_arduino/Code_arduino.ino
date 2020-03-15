// External libraries:
// HX711_Arduino_Library

#include "comandos.hpp"
#include "robot.hpp"

Robot robotet;
int contador = 0; // temporal

void setup()
{
	comando_init();
	robotet.init();

	Serial.println("Debug");
	robotet.rotar(true);
}

void loop()
{
	if (robotet.davant_cartutxo())
	{
		robotet.rotar(false);

		if (robotet.llegir_cartutxo() == 5) // cartutxo que vol l'usuari
		{
			Serial.println("Cartutxo trovat, dosificar");
			robotet.dosificar_pastilla();
		}
		else
		{
			Serial.println("Hem passat per un cartutxo, no era el bo");
			robotet.rotar(true);
		}
	}

	pooling_comando(); // comandos.cpp
}
