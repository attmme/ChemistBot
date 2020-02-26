#ifndef driver_balanca_hpp_
#define driver_balanca_hpp_

// Macros
#define N_MESURES 15 // + mesures == + estable la mesura
#define PES_CONEGUT 50

// Class
class Bascula
{
public:
	int step_calib = 0;
	bool si = 0;
	bool no = 0;
	bool calibrant = false; // quan acaba la calibració, els comandos tornen a funcionar normal

	void init();
	void tara();
	void calibrar();
	float llegir_pes();

	void error_comando();

private:
	float calibracio;
	void si_no_error();
	void reiniciar_variables();
};

#endif
