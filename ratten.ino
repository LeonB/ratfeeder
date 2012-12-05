/*
 * sudo apt-get install arduino
 * http://www.geekonfire.com/wiki/index.php?title=Dual_H-Bridge_Motor_Driver
 * http://arduino.cc/playground/Code/CmdMessenger
 * https://github.com/dreamcat4/CmdMessenger
 */
 
// CmdMessenger library available from https://github.com/dreamcat4/cmdmessenger
#include <CmdMessenger.h>

// Streaming4 library available from http://arduiniana.org/libraries/streaming/
#include <Streaming.h>

#include "Motor.h"

// ------------------ G L O B A L  V A R I A B L E S ------------------

// Mustnt conflict / collide with our message payload data. Fine if we use base64 library ^^ above
char field_separator = ',';
char command_separator = ';';

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 000, // Lets Arduino report serial port comm error back to the PC (only works for some comm errors)
  kACK           = 001, // Arduino acknowledges cmd was received
  kARDUINO_READY = 002, // After opening the comm port, send this cmd 02 from PC to check arduino is ready
  kERR           = 003, // Arduino reports badly formatted cmd, or cmd not recognised
  kTIMEOUT       = 004, // Arduino reports timeout

  // Now we can define many more 'send' commands, coming from the arduino -> the PC, eg
  // kICE_CREAM_READY,
  // kICE_CREAM_PRICE,
  // For the above commands, we just call cmdMessenger.sendCmd() anywhere we want in our Arduino program.

  kSEND_CMDS_END, // Mustnt delete this line
};

// Commands we send from the PC to be received on the Arduino
enum
{
  kfTESTER = 001,
  kfROTATE = 002,
  kfROTATE_STOP = 003,
  kfROTATE_CW = 004,
  kfROTATE_CCW = 005,
  kfROTATE_TIME = 006,
};

Motor motor;
String content = "";
char character;

// ------------------ D E F A U L T  C A L L B A C K S -----------------------

void arduino_ready()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}

void arduino_timeout()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kTIMEOUT,"Timeout occured");
  cmdMessenger.reset();
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

void tester()
{
  cmdMessenger.sendCmd(kACK, "Tester!");
}

void motor_rotate()
{
  cmdMessenger.sendCmd(kACK, "motor.rotate(255)");
  motor.rotate(255);
}

void motor_rotate_stop()
{
  cmdMessenger.sendCmd(kACK, "motor.rotate_stop()");
  motor.rotate_stop();
}

void motor_rotate_cw()
{
  cmdMessenger.sendCmd(kACK, "motor.rotate_cw()");
  motor.rotate_cw();
}

void motor_rotate_ccw()
{
  cmdMessenger.sendCmd(kACK, "motor.rotate_ccw()");
  motor.rotate_ccw();
}

void motor_rotate_time()
{
  String cmds[10];
  int i = -1;
  
  while ( cmdMessenger.available() )
  {
    i++;
    char buf[350] = { '\0' };
    cmdMessenger.copyString(buf, 350);
    if (buf[0]) {
      cmds[i] = String(buf);
    }
  }
  
  if (i != 0) {
    cmdMessenger.sendCmd(kERR, "Wrong number of arguments (1)");
    return;
  }
  
  char cmd[100];
  cmds[0].toCharArray(cmd, 100);
  
  long time = atol(cmd);
  
  if (time == 0) {
    return;
  }
  
  char msg[100];
  strcpy(msg, "motor.rotate_time(");
  strcat(msg, cmd);
  strcat(msg, ")");
  
  cmdMessenger.sendCmd(kACK, msg);

  motor.rotate_time(time);
}

// ------------------ E N D  C A L L B A C K  M E T H O D S ------------------

// ------------------ S E T U P ----------------------------------------------

void setup()
{
  // Listen on serial connection for messages from the pc
  // Serial.begin(57600);  // Arduino Duemilanove, FTDI Serial
  Serial.begin(115200); // Arduino Uno, Mega, with AT8u2 USB

  // cmdMessenger.discard_LF_CR(); // Useful if your terminal appends CR/LF, and you wish to remove them
  // cmdMessenger.print_LF_CR();   // Make output more readable whilst debugging in Arduino Serial Monitor

  // Attach default / generic callback methods
  cmdMessenger.attach(unknownCmd);
  cmdMessenger.attach(kfTESTER, tester);
  cmdMessenger.attach(kfROTATE, motor_rotate);
  cmdMessenger.attach(kfROTATE_STOP, motor_rotate_stop);
  cmdMessenger.attach(kfROTATE_CW, motor_rotate_cw);
  cmdMessenger.attach(kfROTATE_CCW, motor_rotate_ccw);
  cmdMessenger.attach(kfROTATE_TIME, motor_rotate_time);

  motor = Motor(Serial);
}

// ------------------ M A I N ( ) --------------------------------------------

// Timeout handling
long timeoutInterval = 2000; // 2 seconds
long previousMillis = 0;

void loop()
{
  // Process incoming serial data, if any
  cmdMessenger.feedinSerialData();
  
  motor.do_background_work();

  // handle timeout function, if any
  // if (  millis() - previousMillis > timeoutInterval )
  // {
  //   arduino_timeout();
  //   previousMillis = millis();
  // }
}


