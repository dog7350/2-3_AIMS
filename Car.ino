#include <string.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define BAND 9600

#define T_Default 180
#define T_Degree 150

#define MIDDLE 45
#define BACKTIME 50

Servo Left;
Servo Right;
Servo Truck;

SoftwareSerial carBT(2, 3); //TX, RX

int buz = 5;

int Lir = A0;
int Rir = A1;

int bTime = 0;
int backCount = 0;

int spd = 50;

void setup() {
  Left.attach(13);
  Right.attach(12);
  Truck.attach(4);
  Truck.write(T_Default);

  pinMode(buz, OUTPUT);
  
  carBT.begin(BAND);
}

void loop() {
  char txt;
  
  if(carBT.available()){
    txt = carBT.read();
    if(txt == 't')//쓰레기 처리장
    {
      maneuver(spd, -spd, 300);
      lineTrace(txt);
      stopcar();
    }
    else if(txt == 'd')//택배 수취
    {
      maneuver(-spd, spd, 300);
      lineTrace(txt);
      stopcar();
    }
    else if(txt == 'f')//[관리자] 전진
    {
      maneuver(200, 200, 500);
      stopcar();
    }
    else if(txt == 'l')//[관리자] 좌회전
    {
      maneuver(-200, 200, 500);
      stopcar();
    }
    else if(txt =='r')//[관리자] 우회전
    {
      maneuver(200, -200, 500);
      stopcar();
    }
  }
}

void lineTrace(char txt)
{
  int LT = 1;
  int L;
  int R;
  
  if(txt == 't')
  {
    while(LT)
    {
      L = analogRead(Lir);
      R = analogRead(Rir);
      if(L <= MIDDLE && R <= MIDDLE)
      {
        bTime = 0;
        maneuver(spd, spd, 50);
      }
      else if(L > MIDDLE && R <= MIDDLE)
      {
        maneuver(spd, -spd, 30);
      }
      else if(L <= MIDDLE && R > MIDDLE)
      {
        maneuver(-spd, spd, 30);
      }
      else if(L > MIDDLE && R > MIDDLE)
      {
        bTime++;
        if(backCount == 1 && bTime >= BACKTIME)
        {
          stopcar();
          LT = 0;
        }
        
        maneuver(spd, spd, 10);
        
        if(bTime >= BACKTIME)
        {
          backCount = 1;
          backTurn();
          Truck_Move();
        }
      }
    }
  }
  else if(txt == 'd')
  {
    while(LT)
    {
      L = analogRead(Lir);
      R = analogRead(Rir);
      if(L <= MIDDLE && R <= MIDDLE)
      {
        bTime = 0;
        maneuver(spd, spd, 50);
      }
      else if(L > MIDDLE && R <= MIDDLE)
      {
        maneuver(spd, -spd, 30);
      }
      else if(L <= MIDDLE && R > MIDDLE)
      {
        maneuver(-spd, spd, 30);
      }
      else if(L > MIDDLE && R > MIDDLE)
      {
        bTime++;
        if(backCount == 1 && bTime >= BACKTIME)
        {
          stopcar();
          LT = 0;
        }
        
        maneuver(spd, spd, 10);
        
        if(bTime >= BACKTIME)
        {
          backCount = 1;
          backTurn();
          tone(buz, 3000, 1000);
          delay(2000);
          tone(buz, 3000, 1000);
          delay(1000);
        }
      }
    }
  }
}

void Truck_Move()
{
  tone(buz, 3000, 1000);
  Truck.write(T_Degree);
  delay(1000);
  Truck.write(T_Default);
  delay(1000);
  tone(buz, 3000, 1000);
  delay(1000);
}

void stopcar()
{
  bTime = 0;
  backCount = 0;
  Left.writeMicroseconds(1500);
  Right.writeMicroseconds(1500);
  delay(100);
}

void backTurn()
{
  int B_flag = 1;
  while(B_flag)
  {
    int L = analogRead(Lir);
    int R = analogRead(Rir);
    maneuver(spd, -spd, 10);
    if(L <= MIDDLE && R <= MIDDLE)
    {
      maneuver(0, 0, 100);
      B_flag = 0;
    }
  }
}

void maneuver(int spdLeft, int spdRight, int msTime)
{
  Left.writeMicroseconds(1500 + spdLeft);
  Right.writeMicroseconds(1500 - spdRight);

  if(msTime == -1)
  {
    Left.detach();
    Right.detach();
  }
  delay(msTime);
}
