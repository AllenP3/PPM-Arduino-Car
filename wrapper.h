 
#include <PPMReader.h>
#define ML_Ctrl 2    
#define ML_PWM 5  
#define MR_Ctrl 4    
#define MR_PWM 6  
#define servoPin A3 
int L_pin = 11; //pins of left line tracking sensor
int M_pin = 7; //pins of middle line tracking sensor
int R_pin = 8; //pins of right line tracking sensor
int interruptPin = 3;
int channelAmount = 6;
int ch[6];
int trigPin = 12;
int echoPin = 13;
int pp1, pp2, pp3, pp4, pp5, pos, pulsewidth, duration, dist, val_L,val_R,val_M;
char rChar = '0';
boolean ch2_neutral = true, ch3_neutral =  true;
bool sysAlive = false, newData = false;
bool detect = true;
int var_maxPWM = 255;
PPMReader ppm(interruptPin, channelAmount);

void setup()
{
  Serial.begin(9600);
  rChar = "g";
  pinMode(ML_Ctrl, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  pinMode(MR_Ctrl, OUTPUT);
  pinMode(MR_PWM, OUTPUT);
  pinMode(servoPin, OUTPUT); 
  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);
  pinMode(L_pin,INPUT); 
  pinMode(M_pin,INPUT); 
  pinMode(R_pin,INPUT); 

  digitalWrite(ML_Ctrl, LOW);
  analogWrite(ML_PWM,0);
  digitalWrite(MR_Ctrl, LOW);
  analogWrite(MR_PWM,0);

  ch[2] = 1500;
  delay(500);
 
}

void sysCheck()
{
  if (ch[2] > 900 && ch[1] > 900)
  {
    sysAlive = true;
  }
  else
  {
    sysAlive = false;
  }
}
void readPPMData()
{
  /*for (int channel = 1; channel <= channelAmount; ++channel) {
        unsigned long value = ppm.latestValidChannelValue(channel, 0);
        Serial.print(String(value) + " ");
    }*/
    for (int channel = 1; channel <= channelAmount; ++channel)
  {
    unsigned long value = ppm.latestValidChannelValue(channel, 0);
    ch[channel - 1] = value;
  }
}

void move_forward()
{
      digitalWrite(ML_Ctrl, HIGH);
      analogWrite(ML_PWM, pp1);

      digitalWrite(MR_Ctrl, HIGH);
      analogWrite(MR_PWM, pp2);
      Serial.println("FORWARD");
}

void move_backward()
{
      digitalWrite(ML_Ctrl, LOW);
      analogWrite(ML_PWM, pp1);

      digitalWrite(MR_Ctrl, LOW);
      analogWrite(MR_PWM, pp2);
      Serial.println("BACKWARD");
}

void move_left()
{
      digitalWrite(ML_Ctrl, LOW);
      analogWrite(ML_PWM, pp3);

      digitalWrite(MR_Ctrl, HIGH);
      analogWrite(MR_PWM,pp4);
      Serial.println("LEFT");

}

void move_right()
{
   
      digitalWrite(ML_Ctrl, HIGH);
      analogWrite(ML_PWM, pp3);

      digitalWrite(MR_Ctrl, LOW);
      analogWrite(MR_PWM,pp4);
      Serial.println("RIGHT");
}

void neutral() 
{
  digitalWrite(ML_Ctrl, LOW);
  analogWrite(ML_PWM,0);

  digitalWrite(MR_Ctrl, LOW);
  analogWrite(MR_PWM,0);
}

void rotateHServo(int myangle) {
  pulsewidth = myangle * 11 + 500; //calculate the value of pulse width
  digitalWrite(servoPin,HIGH);
  delayMicroseconds(pulsewidth); //The duration of high level is pulse width
  digitalWrite(servoPin,LOW);
  delay((20 - pulsewidth / 1000)); //the cycle is 20ms, the low level last for the rest of time
}

void objectDetect(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //trigPin send a 10us high level
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  dist = (duration/2) / 29.1;

  if(dist < 5){
    detect = true;
  }
  else{
    detect = false;
  }
}

void lineDetect(){
  val_L = digitalRead(L_pin);// read the L_pin
  val_R = digitalRead(R_pin);// read the R_pin
  val_M = digitalRead(M_pin);// read the M_pin
}

void recvOneChar() {
    if (Serial.available() > 0) {
        rChar = Serial.read();
        newData = true;
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(rChar);
        newData = false;
    }
}



void mobilize() 
{

  int one = ch[0];
  int two = ch[1];
  int three = ch[2];
  int four = ch[3];
  int five = ch[4];
  int six = ch[5];
  
  
  sysCheck();
  objectDetect();

  // Check if system is alive
  if(sysAlive == true){


  //Object Detection
  if(detect == true){
    neutral();
    Serial.println("Object Detected");
    ch2_neutral = false;
    pp1 = 100;
    pp2 = pp1;
    move_backward();
    Serial.println("Auto-moving backward");
    delay(50);
  }
  
  //else{Serial.println("All Clear");}

  //Drive Controls
    recvOneChar();
    //showNewData();

    if(detect == false){
    if(rChar == 'w'){
      pp5 = 180;
      for(int i = 0; i < 3; i++){
      rotateHServo(pp5);
      }
      Serial.println("got w");
      
      /*pp1 = 100;
      pp2 = pp1;*/

     // for(int i = 0; i < 5000; i++){
      
      //ch2_neutral = false;
      //move_forward();
      //delayMicroseconds(10);
   //}
    rChar = "g";
    }

    else if(rChar == 's'){
      pp5 = 0;
      for(int i = 0; i < 3; i++){
      rotateHServo(pp5);
      }
      Serial.println("got s");
      /*pp1 = 100;
      pp2 = pp1;

      for(int i = 0; i < 5000; i++){
      ch2_neutral = false;
      move_backward();
      delayMicroseconds(10);
    }*/

    }}
  if (two > 1485 && two < 1515) // stay idle
  {
    ch2_neutral = true;
  }

  else if (two <= 1450) // move backward
  {
    ch2_neutral = false;
    pp1 = abs(map(two, 1500, 1000, 0, var_maxPWM));
    pp2 = pp1;
    move_backward();
    Serial.println("Moving Backward");
    delayMicroseconds(100);
      }
  
  else if (two >= 1550  && detect == false) // move forward
  {
    ch2_neutral = false;
    pp1 = abs(map(two, 1500, 2000, var_maxPWM, 0)); // map PPM 
    pp2 = pp1;
    move_forward();
    Serial.println("Moving Forward");
  }

  //Turn Controls
  if (one > 1485 && one < 1515 ) // stop 
  {
    ch3_neutral = true;
  }
  else if (one >= 1550  && detect == false ) // move right
  {
    ch3_neutral = false;
    pp3 = abs(map(one, 1500, 2000, var_maxPWM, 0 ));
    pp4 = abs(map(pp3, var_maxPWM, 0, 0, var_maxPWM));
    move_right();
  }
  else if (one <= 1450  && detect == false) // move left
  {
    ch3_neutral = false;
    pp3 = abs(map(one, 1500, 1000, 0, var_maxPWM));
    pp4 = abs(map(pp3, 0, var_maxPWM, var_maxPWM , 0));
    move_left();
  }
  else if(ch2_neutral == false && ch3_neutral == false){
    //turn();
  }

  if (ch2_neutral == true && ch3_neutral == true && sysAlive == true) // neutral gear 
  {
    neutral();
  }

// Servo Control
  if(four > 1485 && four < 1550 && four > 900){
     digitalWrite(servoPin,LOW);
  }
  else if( four < 1450  && four > 900){
    pp5 = abs(map(four, 1500, 1000, 90, 180));
    rotateHServo(pp5);
    
  }
  else if(four > 1550 && four > 900){
    pp5 = abs(map(four, 1500, 2000, 90, 0));
    rotateHServo(pp5);
  }
 }
}

