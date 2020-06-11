#ifndef _robot_hpp_
#define _robot_hpp_

// Macros
#define N_MESURES 15   // + mesures == + estable la mesura
#define PES_CONEGUT 50 // pes per a calibrar

#define TEMPS_AJUSTE_POSICIO_CARRO 75 // temps per a ajustar millor la posició

#define MOTOR_ROTOR_PWM_MAXIM 50

#define MIN_SERVO_LECTOR 0
#define MAX_SERVO_LECTOR 55 //45

#define MIN_SERVO_PASTILLA 0 // dispensador pastilla
#define MAX_SERVO_PASTILLA 128

#define TEMPS_ESPERA_LECTURA 400

#define MIN_VALOR_SENSOR 1000 // és el que decideix quan el valor analògic val 0 o 1

// Bloqueig motor
#define STOP 0
#define BLOQ 2
#define DESBLOQ 4

class Robot
{
public:
	// Balança
	static int step_calib; // calibrar té uns passos, amb això ho controlem
	static int si;		   // cops que l'usuari diu que si en la calibració
	static int no;		   // cops que l'usuari diu que no en la calibració
	static bool calibrant; // quan acaba la calibració, els comandos tornen a funcionar normal

	void init();

	void tara();
	void calibrar();
	float llegir_pes();

	// Motor rotor
	void girar(bool si_no);
	void girar_un();

	// Sensor rotor
	bool davant_cartutxo();

	// Motor dispensador cartutxo
	void dosificar_pastilla();

	// Sensors cartutxo
	int llegir_cartutxo();

	// Bloqueig rotor
	void tambor(char blck_desbl);

private:
	// Balança
	float calibracio;
	void reiniciar_variables();

	// Motor rotor

	// Sensor rotor

	// Motor dispensador cartutxo

	// Sensors cartutxo

	// Bloqueig rotor
	void bloquejar_rotor();
	void desbloquejar_rotor();
	void bloq_desbloq(char rotacio);
};

#endif
