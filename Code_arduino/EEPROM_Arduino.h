#ifndef _eeprom_arduino_h_
#define _eeprom_arduino_h_

#include <EEPROM.h>

// Macros
#define ADDR_CALIBRACIO_BASCULA 0x0000

class EEPROM_ARDUINO
{
public:
    inline float llegir(int add)
    {
        float valor = 0;
        EEPROM.get(add, valor);
        return valor;
    }

    inline void escriure(int add, float valor)
    {
        Serial.print(valor);
        EEPROM.put(add, valor);
    }
};

#endif
