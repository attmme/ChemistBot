#include "Arduino.h" // pinMode, digitalRead, etc
#include "drivers.h"
#include "Musica.hpp"


void Musica::init()
{
	pinMode(BUZZER, OUTPUT);
}

void Musica::reproduir(int music)
{
	int melodia[4][6] = {

		// melodia up
		{NOTA_E6, NOTA_G6, NOTA_E7, NOTA_C7, NOTA_D7, NOTA_G7}, // 0
		{150, 150, 150, 150, 150, 150}, // 1

		// melodia error
		{NOTA_G6, NOTA_G6, NOTA_G6, NOTA_G6, NOTA_G6, NOTA_G6}, // 2
		{200, 200, 200, 200, 200, 200}, // 3
	};

	for (int i = 0; i < 6; i++)
	{
		tone(46, music[melodia][i]);
		delay(music[melodia + 1][i]);
	}
	noTone(46);
}