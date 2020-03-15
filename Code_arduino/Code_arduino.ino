// External libraries:
// HX711_Arduino_Library

#include "comandos.hpp"
#include "robot.hpp"

Robot robotet;

void setup()
{
	pinMode(53, INPUT);
	digitalWrite(53, HIGH);

	Serial.begin(9600);
	Serial.println("Debug");

	comando_init();
	robotet.init();
}

void loop()
{
	if (digitalRead(53)) // temporal
	{
		Serial.println(1);
	}
	else
	{
		Serial.println(0);
	}

	pooling_comando(); // comandos.cpp
}
