#include "robot.hpp"
#include "Arduino.h" // pinMode, digitalRead, etc
#include "drivers.h"
#include "HX711.h"          // HX711 circuit wiring
#include "EEPROM_Arduino.h" // aqui estan les direccions de memòria
#include "comandos.hpp"

HX711 bascula; // HX711 = DAC que es fa servir per a llegir la balança
EEPROM_ARDUINO eeprom;

// estàtiques
int Robot::step_calib = 0;
int Robot::si = 0;
int Robot::no = 0;
bool Robot::calibrant = false; // quan acaba la calibració, els comandos tornen a funcionar normal

// Public
void Robot::init()
{
    // Bascula
    bascula.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    this->reiniciar_variables();
    this->tara();

    // Motor rotor
    pinMode(MOTOR_ROTOR, OUTPUT);

    // Sensor rotor
    pinMode(PIN_SENSOR_ROTOR, INPUT);
    digitalWrite(PIN_SENSOR_ROTOR, HIGH); // pull-up = actiu ( estalviem una R )

    // Motor dispensador cartutxo

    // Sensors cartutxo
}

// Bascula
float Robot::llegir_pes()
{
    if (!bascula.is_ready()) // si no està llesta, retornem 0
    {
        return 0;
    }

    return (float)bascula.get_units(N_MESURES); // else -> retornem pes
}

void Robot::tara()
{
    eeprom.llegir(ADDR_CALIBRACIO_BASCULA, calibracio);
    bascula.set_scale(calibracio);
    bascula.tare();
}

void Robot::calibrar()
{
    switch (this->step_calib)
    {
    case 0:
        this->calibrant = 1; // estem calibrant, els comandos funcionaran com a respostes de les preguntes
        Serial.print("1/2. Treu el pes de la balança\nFet? si : no ==> ");
        break;

    case 1:
        print_si_no();

        if (this->si)
        {
            bascula.set_scale();
            bascula.tare();
        }
        else
        {
            this->calibrar();
        }

        Serial.print("2/2. Posa un pes de 50gr\nEn tens? si : no ==> ");
        break;

    case 2:
        print_si_no();

        if (this->si)
        {
            calibracio = (bascula.get_units(N_MESURES) / PES_CONEGUT); // per a calibrar fas numero rebut / pes conegut
            eeprom.escriure(ADDR_CALIBRACIO_BASCULA, calibracio);
            bascula.set_scale(calibracio);
        }

        Serial.println("Acabat!");
        this->reiniciar_variables();
        break;
    }
}

void Robot::reiniciar_variables()
{
    this->step_calib = 0;
    this->calibrant = 0;
    this->si = 0;
    this->no = 0;
}

// Motor rotor


// Sensor rotor



// Motor dispensador cartutxo



// Sensors cartutxo
