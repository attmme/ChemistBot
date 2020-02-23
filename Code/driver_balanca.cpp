#include "modos_debug.h" // debug
#include "Arduino.h" // pinMode, digitalRead, etc
#include "driver_balanca.hpp"
#include "HX711.h"  // HX711 circuit wiring
#include <EEPROM.h>
#include "EEPROM.h" // aqui estan les direccions de memòria


#if DEBUG_BALANCA == 1
	#include "protocol.hpp" // modo debug
#endif

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 bascula;

// Public
	void Bascula::init()
	{
		bascula.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
		this->reiniciar_variables();
		
		#if DEBUG_BALANCA == 1
			protocol_init(); // per la modo debug
		#endif

		this->tara();
	}

	float Bascula::llegir_pes()
	{
		float pes = 0;

		if(bascula.is_ready()) // caldrà un bloqueo? fer un flush del buffer?
		{
			pes = bascula.get_units(N_MESURES);
		}
		else
		{ 
			//Serial.println("HX711 not found.");
		} 
		return pes;
	}
	
	void Bascula::tara()
	{
		#if DEBUG_COMANDOS == 1
			Serial.println("DEBUG: driver_balanca.cpp -> tara() -> tara balança");
		#endif
		
		EEPROM.get(ADDR_CALIBRACIO_BASCULA, calibracio);
		bascula.set_scale(calibracio);
		bascula.tare();
	}

	void Bascula::calibrar()
	{
		switch(this->step_calib)
		{
			case 0:
				this->calibrant = 1; // estem calibrant, els comandos funcionaran com a respostes de les preguntes
				Serial.print("1/2. Treu el pes de la balança\nFet? si : no ==> ");
			break;
			
			case 1:
				this->si_no_error();
				
				if(this->si)
				{
					bascula.set_scale();
					bascula.tare();
				}
				else { this->calibrar(); }
				
				Serial.print("2/2. Posa un pes de 50gr\nEn tens? si : no ==> ");
			break;

			case 2:
				this->si_no_error();
				
				if(this->si)
				{
					calibracio = (bascula.get_units(N_MESURES) / PES_CONEGUT ); // per a calibrar fas numero rebut / pes conegut
					EEPROM.put(ADDR_CALIBRACIO_BASCULA, calibracio);
					bascula.set_scale(calibracio);
				}
				
				Serial.println("Acabat!");
				this->reiniciar_variables();
			break;		
		}
	}
	
	void Bascula::si_no_error() // mirem si la resposta és si, no o no existeix
	{
		if(this->si)
		{
			Serial.println("si\n");
		}
		else if(this->no)
		{
			Serial.println("no\n");
		}
	}
	
	void Bascula::error_comando()
	{
		Serial.println("No t'entenc, intenta-ho de nou");
		this->calibrar();
	}
	
	void Bascula::reiniciar_variables()
	{
		this->step_calib = 0;
		this->calibrant = 0;
		this->si = 0;
		this->no = 0;
	}






