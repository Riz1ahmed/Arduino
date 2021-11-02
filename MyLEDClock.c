#include<Wire.h>
#include<DS3231.h>
DS3231 rtc(SDA, SCL);
Time t;

bool glowSec=false;
int milliSec=0;

int gnd=2, v5=3;
int dataPin=4,latchPin=5, clockPin=6;
int g1=7; //4 no segment gnd
int g2=8; //3 no segment gnd
int g3=9; //second segment gnd
int g4=10;//2 no segment gnd
int g5=11;//1 no segment gnd

int digits[]={
 //gfedcba
  B0111111,//63
  B0110000,//48
  B1101101,//109
  B1111001,//121
  B1110010,//114
  B1011011,//91
  B1011111,//95
  B0110001,//49
  B1111111,//127
  B1111011 //123
};

void setup() {
  rtc.begin();
  Serial.begin(9600);
  
  pinMode(gnd,OUTPUT),digitalWrite(gnd,LOW);
  pinMode(v5,OUTPUT),digitalWrite(v5,HIGH);
  
  pinMode(dataPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);

  pinMode(g1,OUTPUT),digitalWrite(g1,HIGH);
  pinMode(g2,OUTPUT),digitalWrite(g2,HIGH);
  pinMode(g3,OUTPUT),digitalWrite(g3,HIGH);
  pinMode(g4,OUTPUT),digitalWrite(g4,HIGH);
  pinMode(g5,OUTPUT),digitalWrite(g5,HIGH);
}

int i=1;
void loop() {
  if (milliSec>500) glowSec=!glowSec, milliSec=0;
  milliSec++;
  t = rtc.getTime();
  showTime();
}

void showTime(){
   
   int m=t.min;
   showDigit(g5, digits[m%10]);
   showDigit(g1, digits[m/10]);

   //blinkSecond();
   //if (glowSec) showDigit(g3,1);

   int h=t.hour%12;
   if (!h) h=12;
   showDigit(g2, digits[h%10]);
   showDigit(g4, digits[h/10]);
    
  Serial.println(milliSec);
}

void blinkSecond(){
  if (milliSec>500) glowSec=!glowSec, milliSec=0;
  milliSec++;
  if (glowSec) showDigit(g3,1);
}

void blinkLED(int x){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin,clockPin, LSBFIRST, x);
  digitalWrite(latchPin, HIGH);
}

void showDigit(int dGnd, int digitValue){
  digitalWrite(dGnd,0);
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin,clockPin, LSBFIRST, digitValue);
  digitalWrite(latchPin, HIGH);
  
  digitalWrite(dGnd,1);
}
