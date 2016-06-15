int driveMode = 4;
/*
2 = off
 3 = phone controlled
 4 = autonomous roam
 5 = autonomous baby duck
 6 = map room
 */

#define RSpeedPin  11
#define RDirPin  8
#define LSpeedPin  10
#define LDirPin  12

#include <Servo.h> 
Servo myservo;
#define ServoPin  6

#define SonarTriggerPin  3
#define SonarEchoPin  2


#define START_CMD_CHAR '*'
#define CMD_DIGITALWRITE 10
#define CMD_ANALOGWRITE 11 

int Speed = 10;
int LeftTurn = 0;
int RightTurn = 0;

int servopos = 0;
int distance = 0;
int lastdist =0;
void setup() {
  Serial.begin(9600);
  Serial.flush();


  pinMode(RSpeedPin, OUTPUT);
  pinMode(RDirPin, OUTPUT);
  pinMode(LSpeedPin, OUTPUT);
  pinMode(LDirPin, OUTPUT);

  myservo.attach(ServoPin);

  pinMode(SonarTriggerPin, OUTPUT);
  pinMode(SonarEchoPin, INPUT);
}

void loop()
{

  Serial.flush();
  int ard_command = 0;
  int pin_num = 0;
  int pin_value = 0;

  // wait for incoming data
  if (Serial.available() >= 1){

    // parse incoming command start flag 
    char get_char = Serial.read();
    if (get_char == START_CMD_CHAR) {

      // parse incoming command type
      ard_command = Serial.parseInt(); // read the command

      // parse incoming pin# and value  
      pin_num = Serial.parseInt(); // read the pin
      pin_value = Serial.parseInt();  // read the value

      Serial.println(ard_command);
      if (ard_command == CMD_DIGITALWRITE){  
        driveMode = pin_num;
      }

      if (ard_command == CMD_ANALOGWRITE) { 
        if (pin_num == 11)  {
          Speed = pin_value;
        } 
        else if (pin_num == 10){
          LeftTurn = pin_value;
        } 
        else if (pin_num == 9){
          RightTurn = pin_value; 
        }
      }
    }
  }



  switch (driveMode) {
  case 2: //off
    stop();
    return;
    break;
  case 3: //phone controlled
    digitalWrite(RDirPin, HIGH);
    digitalWrite(LDirPin, HIGH);
    if (LeftTurn > RightTurn) {
      analogWrite(LSpeedPin, (255-Speed) + LeftTurn - RightTurn);
      analogWrite(RSpeedPin, (255-Speed));
    } 
    else {
      analogWrite(LSpeedPin, (255-Speed) );
      analogWrite(RSpeedPin, (255-Speed) + RightTurn - LeftTurn) ;
    }
    break;
  case 4:
   /* servopos 90;
    distance = 0;
    while (servopos <= 180 && distance < 40)
    {                       
      myservo.write(servopos);   
      delay(25);
      distance = (int) getDistance();  
      servopos +=1;      
    } */

stop();
      distance = (int) getDistance();
    delay(1);
    lastdist = (int) getDistance(); 
  
   Serial.print(distance);
   Serial.print(" ");
 Serial.println(lastdist);
 
      if (distance <40 && lastdist <40)  {
        
        myservo.write(45);   
      delay(500);
      int left = (int) getDistance();  
      
      myservo.write(135);   
      delay(500);
      
      int right = (int) getDistance();  
       
        myservo.write(90); 
      delay(500);
    while ( lastdist < 100 && distance < 100)
    {     
if (right > left) {     
      turn2();
} else {
      turn1();}
    delay(10);
      stop();
      lastdist = distance;
      distance = (int) getDistance();
      
         Serial.print(distance);
   Serial.print(" ");
 Serial.println(lastdist);
      
    } 
  
if (right > left) {     
      turn2();
} else {
      turn1();}
    delay(100);
      stop();



}
    else {
      forward();
      delay(100);
    }

  
    break;
  case 5:
  forward();
      delay(100);
    break;
  case 6:
    break;
  }


  delay(1);
  // getDistance(); 
}

long getDistance() {
  long duration, distance;
  digitalWrite(SonarTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(SonarTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(SonarTriggerPin, LOW);
  duration = pulseIn(SonarEchoPin, HIGH);
  distance = (duration/2) / 29.1;

  /* if (distance >= 400 || distance <= 2){
   Serial.println("Out of range");
   }
   else {
   Serial.print(distance);
   Serial.println(" cm");
   }*/

  return distance;
}

void stop() {
  digitalWrite( RSpeedPin, LOW );
  digitalWrite( RDirPin, LOW );
  digitalWrite( LSpeedPin, LOW );
  digitalWrite( LDirPin, LOW );
}

void forward()
{
  digitalWrite(RDirPin, HIGH);
  analogWrite(RSpeedPin, Speed);
  digitalWrite(LDirPin, HIGH);
  analogWrite(LSpeedPin,300-Speed);
}

void turn1()
{
  digitalWrite(RDirPin, LOW);
  analogWrite(RSpeedPin, 255-Speed);
  digitalWrite(LDirPin, HIGH);
  analogWrite(LSpeedPin, 255-Speed);
}

void turn2()
{
  digitalWrite(RDirPin, HIGH);
  analogWrite(RSpeedPin, Speed);
  digitalWrite(LDirPin, LOW);
  analogWrite(LSpeedPin, Speed);
}








