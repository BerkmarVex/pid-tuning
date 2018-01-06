#include "main.h"

bool dumpValues;
bool timesTen;
bool changePID;
bool divTen;
bool pidAdd;
bool changePidType;
bool previousChangePidType = true;
bool pidSub;
int count = 0;
int PidCount = 0;

struct PID {
      int sensor;
      int target;
      int error;
      int previous_error;
      double Kp;
      double Ki;
      double Kd;
      int bias;
      int iTime;
      int integral;
      int derivative;
      double pid;
      int output;
  };

struct PID leftMotor = {
  .Kp = 0.3,
  .Ki = 0.02,
  .Kd = 0.9,
  .error = 0,
  .previous_error = 0,
  .integral = 0,
  .derivative = 0,
  .target = 300,
  .sensor = 0,
  .bias = 0,
  .integral = 0
};

struct PID rightMotor = {
  .Kp = 0.3,
  .Ki = 0.02,
  .Kd = 0.9,
  .error = 0,
  .previous_error = 0,
  .integral = 0,
  .derivative = 0,
  .target = 3000,
  .sensor = 0,
  .bias = 0,
  .integral = 0
};

struct PID glLift = {
  .Kp = 0.6,
  .Ki = 0.0,
  .Kd = 0.8,
  .error = 0,
  .previous_error = 0,
  .integral = 0,
  .derivative = 0,
  .target = 3000,
  .sensor = 0,
  .bias = 0,
  .integral = 0
};
void tempUpdate(){
	dumpValues = joystickGetDigital( 1, 7, JOY_UP);
	timesTen = joystickGetDigital( 1, 7, JOY_RIGHT);
	changePID = joystickGetDigital(1, 7, JOY_DOWN);
	divTen = joystickGetDigital( 1, 7, JOY_LEFT);
	pidAdd = joystickGetDigital( 1, 8, JOY_UP);
	changePidType = joystickGetDigital(1, 8, JOY_RIGHT);
	previousChangePidType = changePidType;
	pidSub = joystickGetDigital( 1, 8, JOY_DOWN);
}

int pidType(){

	if(changePidType && changePidType != previousChangePidType){

		if(count < 3){
			count += 1;
		}

		else{
			count = 0;
		}
	}

	return count;
}

void valUp(struct PID *this, int type){
	if(timesTen || divTen){
		if(timesTen){
			if(type == 0){
				this->Kp += 0.1;
			}

			else if(type == 1){
				this->Kd += 0.1;
			}

			else if(type == 2){
				this->Ki += 0.1;
			}
		}

		else if(divTen){
			if(type == 0){
				this->Kp += 0.001;
			}

			else if(type == 1){
				this->Kd += 0.001;
			}

			else if(type == 2){
				this->Ki += 0.001;
			}
		}

		else {

			if(type == 0){
				this->Kp += 0.01;
			}

			else if(type == 1){
				this->Kd += 0.01;
			}

			else if(type == 2){
				this->Ki += 0.01;
			}
		}
	}

	else{
		if(type == 0){
			this->Kp += 0.01;
		}

		if(type == 1){
			this->Kd += 0.01;
		}

		if(type == 2){
			this->Ki += 0.01;
		}
	}
}

void valDown(struct PID *this, int type){
	if(timesTen || divTen){

		if(timesTen || divTen){
		if(timesTen){
			if(type == 0){
				this->Kp -= 0.1;
			}

			else if(type == 1){
				this->Kd -= 0.1;
			}

			else if(type == 2){
				this->Ki -= 0.1;
			}
		}

		else if(divTen){
			if(type == 0){
				this->Kp -= 0.001;
			}

			else if(type == 1){
				this->Kd -= 0.001;
			}

			else if(type == 2){
				this->Ki -= 0.001;
			}
		}
	}

		else {

			if(type == 0){
				this->Kp -= 0.01;
			}

			else if(type == 1){
				this->Kd -= 0.01;
			}

			else if(type == 2){
				this->Ki -= 0.01;
			}
		}
	}
}

void buttonCheck(struct PID *this, int type){
	if(pidAdd){
		valUp(this, type);
	}

	else if(pidSub){
		valDown(this, type);
	}

	else{
		return;
	}
}

void control(){
	tempUpdate();
  /*************
  *set PID HERE*
  *************/
	buttonCheck(&leftMotor, pidType());
}

void operatorControl() {
	while (1) {
		delay(20);
	}
}
