#ifndef _robot_hpp_
#define _robot_hpp_

// Macros
#define N_MESURES 15   // + mesures == + estable la mesura
#define PES_CONEGUT 50 // pes per a calibrar

class Robot
{
public:
	void init();

	// Balança
	static int step_calib; // calibrar té uns passos, amb això ho controlem
	static int si;		   // cops que l'usuari diu que si en la calibració
	static int no;		   // cops que l'usuari diu que no en la calibració
	static bool calibrant; // quan acaba la calibració, els comandos tornen a funcionar normal

	void tara();
	void calibrar();
	float llegir_pes();

	// Motor rotor

	// Sensor rotor

	// Motor dispensador cartutxo

	// Sensors cartutxo

private:
	// Balança
	float calibracio;
	void reiniciar_variables();

	// Motor rotor

	// Sensor rotor

	// Motor dispensador cartutxo

	// Sensors cartutxo
};

#endif
