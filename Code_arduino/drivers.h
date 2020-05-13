#ifndef _drivers_h_
#define _drivers_h_

// Bascula  P27
#define BASCULA_DATA 2 // DATA
#define BASCULA_SCK 3  // CLK

// tmp: arduino mega2560, pins pwm => 2~13 i 44~46

// Motor rotor P33
#define MOTOR_ROTOR 11

// Sensor rotor P29
#define PIN_SENSOR_ROTOR 5

// Servos dispensador cartutxo / lector P30, P32
#define PIN_DISPENSADOR 6
#define PIN_LECTOR 8

// Sensors cartutxo  ( P7, P8)
#define SENSOR_0 48 // P7
#define SENSOR_1 49 // P7
#define SENSOR_2 47 // P8

// Bloqueig/desbloqueig
#define SENSOR_INICI 53 // P1
#define SENSOR_FINAL 52
#define CONTROL_A 51 // P2
#define CONTROL_B 50

#endif
