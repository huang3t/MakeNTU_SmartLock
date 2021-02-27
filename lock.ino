//update 3/28 01:16  GT
//update 3/31 11:21  GT

#include "stepper.h"
#include<SoftwareSerial.h>
//#include<Servo.h>
#include<MFRC522.h>
#include<SPI.h>
#include"note.h"

#define RST_PIN     A0
#define SS_PIN      10  //SDA
#define steps       2163
void scan_key();
void lock();
void unlock();
void buzzer_check(int);
void buzzer_fing(int);


MFRC522 *mfrc522;
SoftwareSerial BT(7, 6);//Rx, Tx
//Servo motor; // create servo object 

//setting pin
int LED_pin = 8;     // 5V
int buzzer_pin = 9;   // pin, GND
//int servo_pin = 10;   // yellow->pin  red->5V  black->GND
int hall_pin = A1;
int stepper_pin1 = 2;
int stepper_pin2 = 3;
int stepper_pin3 = 4;
int stepper_pin4 = 5;

stepper stp(stepper_pin1, stepper_pin2, stepper_pin3, stepper_pin4);

unsigned long previous_time = 0;
int previous_hall = 0;
int delta_hall = 0;
unsigned long lock_interval = 60000; // 300s(5min)

//motor parameter
//int steps = 180;  // motor turn for ceta degrees
//int pos = 0;  // variable to store the servo position

//buzzer parameter
int d = 250; // duration
int p = d*1.3; //delay
int i = 0; // index for tone
int note[]={G5, E5, E5, 0, F5, D5, D5, 0, C5, D5, E5, F5, G5, G5, G5, 0,
            G5, E5, E5, 0, F5, D5, D5, 0, C5, E5, G5, G5, E5, 0, 0, 0,
            D5, D5, D5, D5, D5, E5, F5, 0, E5, E5, E5, E5, E5, F5, G5, 0,
            G5, E5, E5, 0, F5, D5, D5, 0, C5, E5, G5, G5, C5, 0, 0, 0};
            
//RFID parameter
bool match = false; //right card detected
byte key[4] = {0XFF, 0XFF, 0XFF, 0XFF};// key card uid


char command = 'd'; // command from Bluetooth
/*
 * d:default
 * k:lock
 * u:unlock
 * B:buzzer on 
 * b:buzzer off
 * L:LED on
 * l:LED off
 */

// state
bool lock_state = false;  /* false for unlock   true for lock */
bool buzzer_state = false;
bool LED_state = false;
bool flash = false;


void scan_key(){
  //mfrc522->PICC_HaltA();
  delay(1000); // 三秒後掃描
  Serial.print("?");
  if(mfrc522->PICC_IsNewCardPresent() && mfrc522->PICC_ReadCardSerial()){
      Serial.print("!");
      if(mfrc522->uid.size == 4){
          Serial.println("key card uid: ");
          for(int i = 0; i < 4; i++){
              key[i] = mfrc522->uid.uidByte[i];
              Serial.println(key[i], HEX); 
          }
          buzzer_check(500);
          delay(500);
      }
  }
}


void lock(){
  stp.rotate(steps);
  stp.stop();
  lock_state = true;
  Serial.println("lock");
  delay(500); 
}

void unlock(){
   stp.rotate(-steps);
   stp.stop();
   previous_time = millis();
   lock_state = false;
   buzzer_state = false;
   Serial.println("unlock");
   delay(500);
}

void buzzer_check(int time){
  tone(buzzer_pin, 2000, time);
}

void buzzer_find(int index){
  tone(buzzer_pin, note[index], d);
  delay(p);
}


void setup() {
    Serial.begin(9600);
    BT.begin(9600); // setting bluetooth's braud rate
    //motor.attach(servo_pin); // setting servo's pin
    SPI.begin();
    mfrc522 = new MFRC522(SS_PIN, RST_PIN);
    mfrc522->PCD_Init();  //initialize mfrc522
    pinMode(A1, INPUT);
    pinMode(LED_pin, OUTPUT);
    pinMode(buzzer_pin, OUTPUT);
    pinMode(stepper_pin1, OUTPUT); 
    pinMode(stepper_pin2, OUTPUT);
    pinMode(stepper_pin3, OUTPUT);
    pinMode(stepper_pin4, OUTPUT);
}

void loop() {
  //Serial.print("hall: ");
  //Serial.println(analogRead(A1));
  //Serial.println(previous_hall);
  /////// Hall module////////
  //Serial.println(previous_time);
  if((millis() - previous_time) >= lock_interval && !lock_state){
    Serial.println("time's up");
    lock();
    previous_time = millis();
  }
  if(abs(analogRead(A1) - previous_hall)>=50/* || (BT.available() > 0)*/){
    previous_time = millis();
    Serial.println("moving"); 
  }
  previous_hall = analogRead(A1);      
  //////////Bluetooth//////////  
  
  command = 'd';
  if(BT.available() > 0){ // read command
    //Serial.print(command);  
    Serial.println("BT");
      while(BT.available() > 0){
        command = BT.read();
        Serial.print(command);
      }
      
  }
  
  
  switch(command){ // do command
      case 'B':
          if(!buzzer_state) buzzer_state = true;
          break;
      case 'b':
          if(buzzer_state) buzzer_state = false;
          break;
      case 'L':
          if(!LED_state) LED_state = true;
          break;
      case 'l':
          if(LED_state) LED_state = false;
          break;
      default:;
  }

  //setting uid key
  if(command == 's'){
    Serial.print("Set");
      scan_key();
      command = 'd';
      return 0;
  }
  
  // LED
  if(LED_state){ // 隔0.1s閃一下
    Serial.print("LED");
      if(flash) {
          digitalWrite(LED_pin, HIGH);
          delay(100);
          flash = false;
      }
      else{
          digitalWrite(LED_pin, LOW);
          delay(100);
          flash = true;
      }
  }
  else if(!LED_state){//看要不要外加開關
      digitalWrite(LED_pin, LOW);
      //Serial.print("close");
  }


//  //servo motor / stepper
  if(command == 'k' && !lock_state){
    Serial.print("lock");
      lock();
      lock_state = true;
      command = 'd';
      return 0;
  }
 
  else if(command == 'u' && lock_state){
    Serial.print("unlock");
      unlock();
      lock_state = false;
      command = 'd';
      return 0;
  }
//
//
  //buzzer
  if(buzzer_state){
    Serial.print("buzzer");
      if(i == sizeof(note)/sizeof(int)){
        i = 0;
      }
      buzzer_find(i);
      i++;
  }
  else i = 0;
  command = 'd';



//  /////////RFID////////
//  
//
//確認是否有新卡片
  if(mfrc522->PICC_IsNewCardPresent() && mfrc522->PICC_ReadCardSerial() ){
        buzzer_check(100);
        byte *id = mfrc522->uid.uidByte;  //get card UID
        byte idSize = mfrc522->uid.size;  //get UID length

//        Serial.print("PICC type: ");      // 顯示卡片類型
//        根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
//           MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
//           Serial.println(mfrc522.PICC_GetTypeName(piccType));

        Serial.print("UID Size: ");
        Serial.println(idSize);

        for(byte i = 0; i < idSize; i++){ // print UID
            Serial.print("id[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(id[i], HEX);
        }
        Serial.println();

        if(idSize == 4){
            for(byte i = 0 ; i < idSize; i++){
                if(id[i] != key[i]) {match=false; break;}
                else match = true;
            }
        }
    
        if(match && !lock_state){ // match -> lock 
            buzzer_check(500);
            lock();
            lock_state = true;
            delay(1000);
            return ;
        } 
        else if(match && lock_state){ // match -> unlock
            buzzer_check(200);
            delay(300);
            buzzer_check(200);
            unlock();
            lock_state = false;
            delay(1000);
            return ;
        }
        mfrc522->PICC_HaltA(); //卡片進入停止模式
        if(!match){
          Serial.print("match: fail");
        }
        else Serial.print("match: success");
    }
    match = false;
}
