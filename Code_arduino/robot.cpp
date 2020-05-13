#include "robot.hpp"
#include "Arduino.h" // pinMode, digitalRead, etc
#include "drivers.h"
#include "HX711.h"          // HX711 circuit wiring
#include "EEPROM_Arduino.h" // aqui estan les direccions de memòria
#include "comandos.hpp"
#include "Servo.h"

HX711 bascula; // HX711 = ADC que es fa servir per a llegir la balança
Servo servo_lector;
Servo servo_pastilla;

EEPROM_ARDUINO eeprom;

// estàtiques
int Robot::step_calib = 0;
int Robot::si = 0;
int Robot::no = 0;
bool Robot::calibrant = false; // quan acaba la calibració, els comandos tornen a funcionar normal

// Public
void Robot::init()
{
    // Motor rotor
    pinMode(MOTOR_ROTOR, OUTPUT);
    girar(false);

    // Bascula
    bascula.begin(BASCULA_DATA, BASCULA_SCK);
    this->reiniciar_variables();
    this->tara();

    // Sensor rotor
    pinMode(PIN_SENSOR_ROTOR, INPUT);
    digitalWrite(PIN_SENSOR_ROTOR, INPUT_PULLUP); // pull-up = actiu ( estalviem una R )

    // Servos dispensador cartutxo / lector
    servo_pastilla.attach(PIN_LECTOR);
    servo_pastilla.write(MIN_SERVO_PASTILLA);

    servo_lector.attach(PIN_DISPENSADOR);
    servo_lector.write(MIN_SERVO_LECTOR);

    // Sensors
    pinMode(SENSOR_0, INPUT);
    pinMode(SENSOR_1, INPUT);
    pinMode(SENSOR_2, INPUT);
    digitalWrite(SENSOR_0, INPUT_PULLUP); // pull-up = actiu ( estalviem una R )
    digitalWrite(SENSOR_1, INPUT_PULLUP);
    digitalWrite(SENSOR_2, INPUT_PULLUP);

    // Bloqueig rotor
    pinMode(SENSOR_FINAL, INPUT);
    pinMode(SENSOR_INICI, INPUT);
    digitalWrite(SENSOR_FINAL, INPUT_PULLUP); // pull-up = actiu ( estalviem una R )
    digitalWrite(SENSOR_INICI, INPUT_PULLUP);

    pinMode(CONTROL_A, OUTPUT);
    pinMode(CONTROL_B, OUTPUT);
    digitalWrite(CONTROL_A, LOW);
    digitalWrite(CONTROL_B, LOW);
}

// Bascula
float Robot::llegir_pes()
{
    float pes = 0;

    if (bascula.is_ready()) // caldrà un bloqueo? fer un flush del buffer?
    {
        pes = bascula.get_units(N_MESURES);
    }
    else
    {
        //Serial.println("Debug: HX711 not found.");
    }
    return pes;
}

void Robot::tara()
{
    calibracio = eeprom.llegir(ADDR_CALIBRACIO_BASCULA);
    bascula.set_scale(calibracio);
    bascula.tare();
}

void Robot::calibrar()
{
inici:

    switch (this->step_calib)
    {
    case 0:
        this->calibrant = 1; // estem calibrant, els comandos funcionaran com a respostes de les preguntes
        Serial.print("Info: 1/2. Treu el pes de la balança\nFet? si : no ==> ");
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
            this->step_calib = 0; // tornem a començar ( no ha tret el pes per a fer la tara )
            goto inici;
        }

        Serial.print("Info: 2/2. Posa un pes de 50gr\nEn tens? si : no ==> ");
        break;

    case 2:
        print_si_no();

        if (this->si)
        {
            calibracio = (bascula.get_units(N_MESURES) / PES_CONEGUT); // per a calibrar fas numero rebut / pes conegut
            eeprom.escriure(ADDR_CALIBRACIO_BASCULA, calibracio);
            bascula.set_scale(calibracio);
        }

        Serial.println("Info: Acabat!");
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
void Robot::girar(bool si_no)
{
    if (si_no)
    {
        analogWrite(MOTOR_ROTOR, MOTOR_ROTOR_PWM_MAXIM);
    }
    else
    {
        analogWrite(MOTOR_ROTOR, 0);
    }
}

// Sensor rotor
bool Robot::davant_cartutxo()
{
    // 0 = on, 1 = off

    if (!digitalRead(PIN_SENSOR_ROTOR)) // sistema anti-rebote
    {
        delay(5);

        if (!digitalRead(PIN_SENSOR_ROTOR))
        {
            while (!digitalRead(PIN_SENSOR_ROTOR))
                ; // fins que no es deixi anar, no pirem

            delay(TEMPS_AJUSTE_POSICIO_CARRO); // temps per ajustar millor
            return true;
        }
    }

    return false;
}

// Motor dispensador cartutxo
void Robot::dosificar_pastilla()
{
    servo_pastilla.write(MAX_SERVO_PASTILLA);
    delay(600); // temps per a que arrivi el pistó dosificador al dispensador
    servo_pastilla.write(MIN_SERVO_PASTILLA);
    delay(500); // temps que donem per a que caigui la pastilla
}

// Sensors cartutxo
int Robot::llegir_cartutxo()
{
    servo_lector.write(MAX_SERVO_LECTOR); // encaixem lector

    delay(TEMPS_ESPERA_LECTURA); // cal un temps desde que li dius que mogui el lector fins que ho fa

    int n_cartutxo = ((digitalRead(SENSOR_2) << 2) | (digitalRead(SENSOR_1) << 1) | (digitalRead(SENSOR_0) << 0));

    servo_lector.write(MIN_SERVO_LECTOR); // retirem lector

    return n_cartutxo;
}

// Bloqueig / desbloqueig
void Robot::tambor(char blck_desbl)
{
    char sensor = 0;

    switch (blck_desbl)
    {
    case BLOQ:
        sensor = SENSOR_FINAL;
        break;

    case DESBLOQ:
        sensor = SENSOR_INICI;
        break;
    }

    bloq_desbloq(blck_desbl);
    while (digitalRead(sensor))
        ;
    bloq_desbloq(STOP);
}

void Robot::bloq_desbloq(char rotacio)
{
    // --------------------STOP ------BLOQ-----DESBLOQ
    char array_pos[6] = {LOW, LOW, HIGH, LOW, LOW, HIGH};

    digitalWrite(CONTROL_A, array_pos[rotacio]);
    digitalWrite(CONTROL_B, array_pos[(rotacio + 1)]);
}