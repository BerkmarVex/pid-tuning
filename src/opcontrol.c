#include "main.h"

void motorSet(unsigned char channel, int speed);

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

int pidDo(struct PID *this) {
	this->error = this->target - this->sensor;
	//this->integral = this->integral + (this->error);
	this->derivative = (this->error - this->previous_error);

  if(abs(this->error) > 5){
    //this->integral = this->error/(abs(this->derivative) + 10) + this->integral;
    this->integral += this->error*exp(-(pow(0.1,2) * pow(this->derivative,2)));
  }

  else if(-this->derivative > this->error||abs(this->error) <= 5){
    this->integral = 0;
  }

	this->pid = (this->Kp) * this->error + (this->Ki) * this->integral + (this->Kd) * this->derivative + this->bias;
	this->output = (int) this->pid;
	this->previous_error = this->error;
  return -this->output;
};

void tempUpdate(){
	dumpValues = joystickGetDigital( 1, 7, JOY_UP);
	timesTen = joystickGetDigital( 1, 7, JOY_RIGHT);
	changePID = joystickGetDigital(1, 7, JOY_DOWN);
	divTen = joystickGetDigital( 1, 7, JOY_LEFT);
	pidAdd = joystickGetDigital( 1, 8, JOY_UP);
	changePidType = joystickGetDigital(1, 8, JOY_RIGHT);
	//printf("%d\n" ,changePidType);
	pidSub = joystickGetDigital( 1, 8, JOY_DOWN);
  imeGet(0, &leftMotor.sensor);
  leftMotor.sensor = 1 * leftMotor.sensor;
}

int pidType(){

	if(changePidType != previousChangePidType){

		if(count < 3){
			count += 1;
		}

		else{
			count = 0;
		}
	}
	//printf("%d\n", count);
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

void logPid(struct PID *this){
  printf("Kp(%f), Ki(%f), Kd(%f)\n", this->Kp, this->Ki, this->Kd);
}

void control(struct PID *this){
	tempUpdate();
  /*************
  *set PID HERE* 
  *************/
	buttonCheck(this, pidType());
  logPid(this);
  motorSet(2, pidDo(this));
	previousChangePidType = changePidType;
}

void operatorControl() {
	while (1) {
    control(&leftMotor);
		delay(20);
	}
}
