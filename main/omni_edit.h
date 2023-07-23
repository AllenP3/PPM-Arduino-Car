 
#include <PPMReader.h>

#define SPEED 140    
#define TURN_SPEED 160

#define speedPinR 9   //  Front Wheel PWM pin connect Right MODEL-X ENA 
#define RightMotorDirPin1  5    //Front Right Motor direction pin 1 to Right MODEL-X IN1  (K1)
#define RightMotorDirPin2  4   //Front Right Motor direction pin 2 to Right MODEL-X IN2   (K1)                                 
#define LeftMotorDirPin1  A4    //Front Left Motor direction pin 1 to Right MODEL-X IN3 (K3)
#define LeftMotorDirPin2  A5   //Front Left Motor direction pin 2 to Right MODEL-X IN4 (K3)
#define speedPinL 10   //  Front Wheel PWM pin connect Right MODEL-X ENB

#define speedPinRB 11   //  Rear Wheel PWM pin connect Left MODEL-X ENA 
#define RightMotorDirPin1B  13    //Rear Right Motor direction pin 1 to Left  MODEL-X IN1 ( K1)
#define RightMotorDirPin2B 6    //Rear Right Motor direction pin 2 to Left  MODEL-X IN2 ( K1) 
#define LeftMotorDirPin1B 8    //Rear Left Motor direction pin 1 to Left  MODEL-X IN3  (K3)
#define LeftMotorDirPin2B 7  //Rear Left Motor direction pin 2 to Left  MODEL-X IN4 (K3)
#define speedPinLB 12    //  Rear Wheel PWM pin connect Left MODEL-X ENB


int interruptPin = 2;
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


void FR_bck(int speed)  //front-right wheel forward turn
{
  digitalWrite(RightMotorDirPin1,HIGH);
  digitalWrite(RightMotorDirPin2,LOW); 
  analogWrite(speedPinR,speed);
}
void FR_fwd(int speed) // front-right wheel backward turn
{
  digitalWrite(RightMotorDirPin1,LOW);
  digitalWrite(RightMotorDirPin2,HIGH); 
  analogWrite(speedPinR,speed);
}
void FL_bck(int speed) // front-left wheel forward turn
{
  digitalWrite(LeftMotorDirPin1,HIGH);
  digitalWrite(LeftMotorDirPin2,LOW);
  analogWrite(speedPinL,speed);
}
void FL_fwd(int speed) // front-left wheel backward turn
{
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,HIGH);
  analogWrite(speedPinL,speed);
}

void RR_bck(int speed)  //rear-right wheel forward turn
{
  digitalWrite(RightMotorDirPin1B, HIGH);
  digitalWrite(RightMotorDirPin2B,LOW); 
  analogWrite(speedPinRB,speed);
}
void RR_fwd(int speed)  //rear-right wheel backward turn
{
  digitalWrite(RightMotorDirPin1B, LOW);
  digitalWrite(RightMotorDirPin2B,HIGH); 
  analogWrite(speedPinRB,speed);
}
void RL_bck(int speed)  //rear-left wheel forward turn
{
  digitalWrite(LeftMotorDirPin1B,HIGH);
  digitalWrite(LeftMotorDirPin2B,LOW);
  analogWrite(speedPinLB,speed);
}
void RL_fwd(int speed)    //rear-left wheel backward turn
{
  digitalWrite(LeftMotorDirPin1B,LOW);
  digitalWrite(LeftMotorDirPin2B,HIGH);
  analogWrite(speedPinLB,speed);
}

void go_advance(int speed){
   RL_fwd(speed);
   RR_fwd(speed);
   FR_fwd(speed);
   FL_fwd(speed); 
}
void go_back(int speed){
   RL_bck(speed);
   RR_bck(speed);
   FR_bck(speed);
   FL_bck(speed); 
}
void right_shift(int speed_fl_fwd,int speed_rl_bck ,int speed_rr_fwd,int speed_fr_bck) {
  FL_fwd(speed_fl_fwd); 
  RL_bck(speed_rl_bck); 
  RR_fwd(speed_rr_fwd);
  FR_bck(speed_fr_bck);
}
void left_shift(int speed_fl_bck,int speed_rl_fwd ,int speed_rr_bck,int speed_fr_fwd){
   FL_bck(speed_fl_bck);
   RL_fwd(speed_rl_fwd);
   RR_bck(speed_rr_bck);
   FR_fwd(speed_fr_fwd);
}

void left_turn(int speed){
   RL_bck(0);
   RR_fwd(speed);
   FR_fwd(speed);
   FL_bck(0); 
}
void right_turn(int speed){
   RL_fwd(speed);
   RR_bck(0);
   FR_bck(0);
   FL_fwd(speed); 
}
void left_back(int speed){
   RL_fwd(0);
   RR_bck(speed);
   FR_bck(speed);
   FL_fwd(0); 
}
void right_back(int speed){
   RL_bck(speed);
   RR_fwd(0);
   FR_fwd(0);
   FL_bck(speed); 
}
void clockwise(int speed){
   RL_fwd(speed);
   RR_bck(speed);
   FR_bck(speed);
   FL_fwd(speed); 
}
void countclockwise(int speed){
   RL_bck(speed);
   RR_fwd(speed);
   FR_fwd(speed);
   FL_bck(speed); 
}


 
void stop_Stop()    //Stop
{
  analogWrite(speedPinLB,0);
  analogWrite(speedPinRB,0);
  analogWrite(speedPinL,0);
  analogWrite(speedPinR,0);
}


//Pins initialize
void init_GPIO()
{
  pinMode(RightMotorDirPin1, OUTPUT); 
  pinMode(RightMotorDirPin2, OUTPUT); 
  pinMode(speedPinL, OUTPUT);  
 
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT); 
  pinMode(speedPinR, OUTPUT);
  pinMode(RightMotorDirPin1B, OUTPUT); 
  pinMode(RightMotorDirPin2B, OUTPUT); 
  pinMode(speedPinLB, OUTPUT);  
 
  pinMode(LeftMotorDirPin1B, OUTPUT);
  pinMode(LeftMotorDirPin2B, OUTPUT); 
  pinMode(speedPinRB, OUTPUT);
   
  stop_Stop();
}

void setup()
{
    init_GPIO();
  Serial.begin(9600);
  rChar = "g";
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

/*void move_forward()
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
}*/

/*
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


*/
void mobilize() 
{

  int one = ch[0];
  int two = ch[1];
  int three = ch[2];
  int four = ch[3];
  int five = ch[4];
  int six = ch[5];
  
  
  sysCheck();
  //objectDetect();

  // Check if system is alive
  if(sysAlive == true){


  //Object Detection
  /*if(detect == true){
    neutral();
    Serial.println("Object Detected");
    ch2_neutral = false;
    pp1 = 100;
    pp2 = pp1;
    move_backward();
    Serial.println("Auto-moving backward");
    delay(50);
  }*/
  
  //else{Serial.println("All Clear");}

  //Drive Controls
    //recvOneChar();
    //showNewData();

     /*if(detect == false){
    if(rChar == 'w'){
      pp5 = 180;
      for(int i = 0; i < 3; i++){
      rotateHServo(pp5);
      }
      Serial.println("got w");
      
      pp1 = 100;
      pp2 = pp1;

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
     pp1 = 100;
      pp2 = pp1;

      for(int i = 0; i < 5000; i++){
      ch2_neutral = false;
      move_backward();
      delayMicroseconds(10);
    }*/

    //}}
  if (two > 1485 && two < 1515) // stay idle
  {
    ch2_neutral = true;
  }

  else if (two <= 1450) // move backward
  {
    ch2_neutral = false;
    pp1 = abs(map(two, 1500, 1000, 0, var_maxPWM));
    go_back(pp1);
    Serial.println("Moving Backward");
    delayMicroseconds(100);
      }
  
  else if (two >= 1550 ) // move forward
  {
    ch2_neutral = false;
    pp1 = abs(map(two, 1500, 2000, 0, var_maxPWM)); // map PPM 
    go_advance(pp1);
    Serial.println("Moving Forward");
  }

  //Turn Controls
  if (one > 1485 && one < 1515 ) // stop 
  {
    ch3_neutral = true;
  }
  else if (one >= 1550 ) // move right
  {
    ch3_neutral = false;
    pp3 = abs(map(one, 1500, 2000,  0, var_maxPWM ));
    right_shift(pp3, pp3, pp3, pp3);
  }
  else if (one <= 1450) // move left
  {
    ch3_neutral = false;
    pp3 = abs(map(one, 1500, 1000, 0, var_maxPWM));
    left_shift(pp3, pp3, pp3, pp3);
  }
  else if(ch2_neutral == false && ch3_neutral == false){
    //turn();
  }

  if (ch2_neutral == true && ch3_neutral == true && sysAlive == true) // neutral gear 
  {
    stop_Stop();
  }

// Servo Control

  if( four < 1450  && four > 900){
    pp5 = abs(map(four, 1500, 1000, 0, 255));
   countclockwise(pp5);
    
  }
  else if(four > 1550 && four > 900){
    pp5 = abs(map(four, 1500, 2000, 0, 255));
    clockwise(pp5);
  }
 }
}

