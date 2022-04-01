#include<Wire.h>
#include<DS3231.h>

DS3231 rtc(SDA, SCL);
Time t;

int milliSec=0;

int gnd=2, v5=3; //Register Power.
int dataPin=4,latchPin=5, clockPin=6;

int dGnds[]={7,8,9,10,11};//From Left to Right
int digits[]={
 //gfedcba
  B00111111,//0=63
  B00110000,//1=48
  B01101101,//2=109
  B01111001,//3=121
  B01110010,//4=114
  B01011011,//5=91
  B01011111,//6=95
  B00110001,//7=49
  B01111111,//8=127
  B01111011,//9=123
  B00000001 //:=For second
};

void setup() {
  rtc.begin();
  Serial.begin(9600);

  //setTime(); //For set time

  pinMode(gnd,OUTPUT),digitalWrite(gnd,LOW);
  pinMode(v5,OUTPUT),digitalWrite(v5,HIGH);
  
  pinMode(dataPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);

  pinMode(dGnds[0],OUTPUT),digitalWrite(dGnds[0],HIGH);
  pinMode(dGnds[1],OUTPUT),digitalWrite(dGnds[1],HIGH);
  pinMode(dGnds[2],OUTPUT),digitalWrite(dGnds[2],HIGH);
  pinMode(dGnds[3],OUTPUT),digitalWrite(dGnds[3],HIGH);
  pinMode(dGnds[4],OUTPUT),digitalWrite(dGnds[4],HIGH);
}
void loop() {
  milliSec>1000 ? milliSec=0 : milliSec+=5;
  
  t = rtc.getTime();
  if (milliSec==950) Serial.print(t.sec, DEC);
  showTime();
}

void showTime(){
   int m=t.min;
   int h=t.hour%12;
   if (!h) h=12;
   //Minute
   showDigit(dGnds[0], digits[m%10]);
   showDigit(dGnds[1], digits[m/10]);
   //Blinking Second
   if (milliSec>500) showDigit(dGnds[2],digits[10]);
   //Houre
   showDigit(dGnds[3],digits[h%10]);
   showDigit(dGnds[4], digits[h/10]);
}

void showDigit(int digitGnd, int digitValue){
  //ResetGnd
  for (int i=0; i<5; i++) digitalWrite(dGnds[i], 1);
  //Digit High
  digitalWrite(latchPin, 0);
  shiftOut(dataPin,clockPin, LSBFIRST, digitValue);
  digitalWrite(latchPin, 1);
  //Selected segment Gnd High, So that, Show digit.
  digitalWrite(digitGnd, 0);
  delayMicroseconds(1000);//Stay digit for 1 millisecond.
}

void setTime(){
  //For set time please call this function from setup.
   rtc.setDOW(SATURDAY);//Day name
   rtc.setTime(12,55,00);//hh,mm,ss
   rtc.setDate(03,03,2022); //dd,MM,yyyy
}
