#ifndef _eeprom_arduino_h_
#define _eeprom_arduino_h_

#include <EEPROM.h>

// Macros
#define ADDR_CALIBRACIO_BASCULA 0x0000

class EEPROM_ARDUINO
{
    public:
    
    inline void llegir(int add, float valor)
    {
        EEPROM.get(add, valor);
    }

    inline void escriure(int add, float valor)
    {
        EEPROM.put(add, valor);
    }   
};

#endif
