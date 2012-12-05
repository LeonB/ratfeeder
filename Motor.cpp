#include "Motor.h"

Motor::Motor()
{
}

Motor::Motor(Stream &ccomms)
{
  init(ccomms);
}
  
void Motor::init(Stream &ccomms)
{
  Serial.println("Setting up motor object");

  this->comms = &ccomms;

  pinMode(ENA, OUTPUT);//output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  //this->rotate_stop();
  this->rotate_cw();
}

void Motor::rotate_stop()
{
  //    for (int i=this->_speed; i >= 0; i--) {
  //      this->rotate(i);
  //      delay(5);
  //    }
  digitalWrite(ENA, LOW); //stop driving
  this->_speed = 0;
  this->_timer = 0;
}

void Motor::rotate(int speed)
{
  analogWrite(ENA, speed);//start driving motorA
  this->_speed = speed;
}

void Motor::rotate_cw()
{
  if (this->_direction != 1) { //cw
    int speed = this->_speed;
    this->rotate_stop();
    digitalWrite(IN2, LOW);
    digitalWrite(IN1, HIGH);
    this->_direction = 1;
    if (speed > 0) {
      this->rotate(speed);
    }
    // Serial.println("Turning CW");
  }
}

void Motor::rotate_ccw()
{
  if (this->_direction != 2) { //ccw
    int speed = this->_speed;
    this->rotate_stop();
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    this->_direction = 2;
    if (speed > 0) {
      this->rotate(speed);
    }
    // Serial.println("Turning CCW");
  }
}

void Motor::rotate_time(unsigned long time)
{
  this->_timer = millis() + time;
  this->rotate(255);
}

void Motor::do_background_work()
{
  // Check if timer is in use
  if (this->_timer > 0) {
    
    // If current time is past timer: stop
    if (millis() >= this->_timer) {
      this->rotate_stop();
    }
  }
}
