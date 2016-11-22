#include <SoftwareSerial.h> 

// important: l66  do 
// #define SERIALCOMMANDBUFFER 24 
// to allow long commands to be parsed
#include <SerialCommand.h>


#define BAUD 115200
#define N_MOTORS 14
#define DEFAULT_POWER 1000

int timers[N_MOTORS] = {0};

//unsigned int powers[N_MOTORS];

unsigned long t0 = 0;
unsigned long t1 = 0;

SerialCommand SCmd;

void setup() {
  Serial.begin(BAUD);
  for (unsigned int i = 0; i != N_MOTORS; ++i){
    pinMode(i, OUTPUT);
  }
  SCmd.addCommand("M",moveMotorSerial); //      
}

void loop() { 
    SCmd.readSerial(); 
    for (unsigned int i = 0; i != N_MOTORS; ++i){
     if(timers[i] > 0){
      digitalWrite(i,HIGH);
     }
     else{
      digitalWrite(i,LOW);
     }  
    }

  delay(50);
  
  t0 = t1;
  t1 = millis();
  
  //fixme overflow of time!
  // i.e. if t0 > t1
  unsigned int tick =  t1 - t0;
  
  for (unsigned int i = 0; i != N_MOTORS; ++i){
     timers[i] -= tick;
     if(timers[i] <0)
      timers[i] = 0;
    }
}


void moveMotor(unsigned int idx, unsigned int duration){
  timers[idx] = duration; // in ms
  
}

void moveMotorSerial(){
  char *arg;
  arg = SCmd.next();  
  unsigned int motor_id = 0;
  unsigned int duration = 0;
  if (arg != NULL) 
    motor_id = atoi(arg) ;
  else
   return; 
   
  arg = SCmd.next();   
  if (arg != NULL)
    duration = atoi(arg) ;
  else
    return;
  moveMotor(motor_id, duration);
  }

