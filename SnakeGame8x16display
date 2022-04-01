#include <ShiftRegister74HC595.h> // library download: http://shiftregister.simsso.de

//Please change this two value with total Shift Register no in row and column.
const int rowRegNo=1;
const int columnRegNo=2;

//In my Matrix Display
//DataPin  = Blue cable
//LatchPin = Green cable
//ClockPin = White cable
const int dpC=2,lpC=3,cpC=4;//Pins of Column
const int dpR=5,lpR=6,cpR=7;//Pins of Row

const int rowSize=rowRegNo*8;
const int columnSize=columnRegNo*8;
ShiftRegister74HC595<rowRegNo> row(dpR,cpR,lpR);
ShiftRegister74HC595<columnRegNo> col(dpC,cpC,lpC);

class Coord {
public:
  byte x,y;
  Coord () { x = 255, y = 255; }
  Coord (byte pX, byte pY) { x = pX, y = pY; }
};

uint8_t led[rowSize][columnSize];

byte snakeDir = 0, snakeLength = 1; // 0 = up, 1 = right, 2 = bottom, 3 = left
Coord snakePos (0,0), foodPos;
Coord snakeSeg[rowSize*columnSize];
//int totalRefreshes = 0;
long mspm = 1000, lastMove = 0; // milliseconds per move
boolean gameRunning = true;

//////////////IR Remote//////////
#include <IRremote.h>
int irRecPin = 10;
int gnd=11;
int v5=12;

const long topGo = 0xEC13BF00;
const long rightGo = 0xED12BF00;
const long bottomGo = 0xEB14BF00;
const long leftGo = 0xEE11BF00;
const long mspmGo = 0xF20DBF00;//Remote refresh button
const long power = 0xFF00BF00;

//////////////IR Remote end//////////

void setup() {
  irRemoteInit();
  restertGame();
}

void loop() {
  clicksListener();

  // mspm * totalRefreshes < millis() && gameRunning
  if (mspm + lastMove < millis() && gameRunning) { // time to move the snake
    lastMove = millis();
    mspm = analogRead(mspmGo);
    //totalRefreshes++;

    if (snakeDir == 0)      snakePos.y++;
    else if (snakeDir == 1) snakePos.x++;
    else if (snakeDir == 2) snakePos.y--;
    else if (snakeDir == 3) snakePos.x--;

    // boundary conditions
    if (snakePos.x == 255)        snakePos.x = rowSize-1;
    if (snakePos.x == rowSize)    snakePos.x = 0;
    if (snakePos.y == 255)        snakePos.y = columnSize-1;
    if (snakePos.y == columnSize) snakePos.y = 0;

    // check if snake eats itself
    if (snakeSegment(snakePos.x, snakePos.y)) gameRunning = false; 

    // check if snake eats food
    if (snakePos.x != foodPos.x || snakePos.y != foodPos.y) removeLastSegment();
    else spawnFood();

    addNewSegment();
    zeroLedArray();
    refreshLedArray();
  }

  gameRunning ? showDisplay() : gameOff();
}

void showDisplay(){
  for (int i=0; i<rowSize; i++){
      row.set(i,LOW);      
      for (int j=0; j<columnSize; j++) 
        if (led[i][j]) col.setNoUpdate(j,HIGH);
      col.updateRegisters();
      delayMicroseconds(50);
      col.setAllLow();
      row.set(i, HIGH);
    }
}

void addNewSegment() {
  snakeSeg[snakeLength].x = snakePos.x;
  snakeSeg[snakeLength].y = snakePos.y;
  snakeLength++;
}

void removeLastSegment() {
  for (int i = 1; i<snakeLength; i++) {
    snakeSeg[i-1].x = snakeSeg[i].x; 
    snakeSeg[i-1].y = snakeSeg[i].y;
  }
  snakeSeg[snakeLength].x = snakeSeg[snakeLength].y = 255;
  snakeLength--; 
}

void refreshLedArray() {
   for (int i=0; snakeSeg[i].x!=255; i++)
     led[snakeSeg[i].x][snakeSeg[i].y] = 1;

  led[foodPos.x][foodPos.y] = 1;
}

void zeroLedArray() {
  memset(led, 0, sizeof led);
}

void spawnFood() {
  // First time without check get a food position. that's why do{}while()
  do {
    foodPos.x=random(rowSize), foodPos.y=random(columnSize);
  } while (snakeSegment(foodPos.x, foodPos.y));
}

boolean snakeSegment(byte x, byte y) {
  for (int i = 0; i<snakeLength; i++) 
    if (snakeSeg[i].x == x && snakeSeg[i].y == y) return true;
  return false;
}

void restertGame(){
  snakeDir = 0, snakeLength = 1; // 0 = up, 1 = right, 2 = bottom, 3 = left
  //int totalRefreshes = 0;
  mspm = 1000, lastMove = 0; // milliseconds per move
  gameRunning = true;

  randomSeed(analogRead(0));
  snakePos.x=snakePos.x=0;
  snakeSeg[0].x = snakePos.x;
  snakeSeg[0].y = snakePos.y;
  zeroLedArray(); 
  spawnFood();
}

void gameOff(){
  gameRunning = false;
  row.setAllHigh();
  col.setAllLow();
}

//////////////IR Remote//////////
void irRemoteInit(){
  Serial.begin(9600);
  IrReceiver.begin(irRecPin, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode(gnd, OUTPUT), digitalWrite(gnd, LOW);
  pinMode(v5, OUTPUT), digitalWrite(v5, HIGH);
}

void clicksListener(){
  if (IrReceiver.decode()) {
    long clicked = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume(); // Receive the next value

    if (clicked==rightGo) snakeDir = 0;//right1
    if (clicked==bottomGo) snakeDir = 1;//bottom2
    if (clicked==leftGo) snakeDir = 2;//up3
    if (clicked==topGo) snakeDir = 3;//up0
    if (clicked==power) gameRunning ? gameOff(): restertGame();

    if (clicked==mspmGo) Serial.println("MsPM");
    Serial.println(clicked, HEX);
  }
}
