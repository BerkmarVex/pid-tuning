#ifndef _PID_H

#define _PID_H

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
