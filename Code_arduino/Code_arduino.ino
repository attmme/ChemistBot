// External libraries:
// HX711_Arduino_Library

#include "comandos.hpp"
#include "robot.hpp"

void setup()
{
	comando_init();
	Serial.println("Debug: Inici!");
}

void loop()
{
	pooling_comando(); // comandos.cpp
}