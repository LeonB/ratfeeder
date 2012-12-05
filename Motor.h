#ifndef Motor_h
#define Motor_h

#include <inttypes.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include "Stream.h"

class Motor
{
protected:
  int _direction = 0;
  int _speed = 0;
  unsigned long _timer = 0;

  int ENA=5;//connected to Arduino's port 5 (output pwm)
  int IN1=3;//connected to Arduino's port 3
  int IN2=4;//connected to Arduino's port 4
  
  void init(Stream &comms);

public:
  Motor();
  Motor(Stream &comms);
  void rotate_stop();
  void rotate(int speed);
  void rotate_cw();
  void rotate_ccw();
  void rotate_time(unsigned long time);
  void do_background_work();

  // Polymorphism used to interact with serial class
  // Stream is an abstract base class which defines a base set
  // of functionality used by the serial classes.
  Stream *comms;
};

#endif
