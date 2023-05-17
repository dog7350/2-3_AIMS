#include <string.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define BAND 9600
#define MoBAND 4800

#define MVD 6500 //1300
#define RMVD 5000 //1700

#define F3 90000 //1300
#define F2 60000 //1300
#define F1 30000 //1300

#define STOP 5000

#define RF3 81000 //1700
#define RF2 54000 //1700
#define RF1 27000 //1700

SoftwareSerial mobBT(2, 3);
SoftwareSerial boardBT(4, 5); //TX, RX

Servo Flow;
Servo Dong;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int F_flag = 0;
int A_flag = 0;
int Car_flag = 0;

int D_State;

void setup() {
  boardBT.begin(BAND);
  mobBT.begin(MoBAND);

  Dong.attach(13);
  Flow.attach(12);

  lcd.begin();
  lcd.backlight();
}

void loop() {
  char txt;

  lcd.clear();
  lcd.print("DONG INPUT");
  delay(1000);
  
  if(mobBT.available())
  {
    txt = (char)mobBT.read();
    lcd.setCursor(0, 1);
    lcd.print(txt);
    if(txt == '1') //1동 이동(기본)
    {
      F_flag = 1; //층수 응답대기
      D_State = 1; //현재 1동 위치
      lcd.clear();
      lcd.print("FLOW INPUT");
      while(F_flag)
      {
        if(mobBT.available())
        {
          txt = (char)mobBT.read();
          lcd.setCursor(0, 1);
          lcd.print(txt);
          Flow_Move(txt);
        }
      }
    }
    else if(txt == '2') //2동 이동
    {
      Dong.writeMicroseconds(1300);
      delay(MVD);
      Dong.writeMicroseconds(1500);
      delay(100);
      F_flag = 1; //층수 응답대기
      D_State = 2; //현재 2동 위치
      lcd.clear();
      lcd.print("FLOW INPUT");
      while(F_flag)
      {
        if(mobBT.available())
        {
          txt = (char)mobBT.read();
          lcd.setCursor(0, 1);
          lcd.print(txt);
          Flow_Move(txt);
        }
      }
    }
    else if(txt == 'a') //층 정밀 조정
    {
      lcd.clear();
      lcd.print("ADMIN F CONTROL");
      F_flag = 1;
      while(F_flag)
      {
        if(mobBT.available())
        {
          txt = (char)mobBT.read();
          lcd.setCursor(0, 1);
          lcd.print(txt);
          Admin_Flow_Control(txt);
        }
      }
    }
    else if(txt == 'c')
    {
      lcd.clear();
      lcd.print("Cancle...");
    }

    lcd.clear();
    A_flag = 1; //행동 응답 대기
    lcd.print("ACTION INPUT");
    while(A_flag) //행동 선택
    {
      if(mobBT.available())
      {
        txt = (char)mobBT.read();
        lcd.setCursor(0, 1);
        lcd.print(txt);
        Action_Select(txt);
      }
    }
  }
}//loop END

void Flow_Move(char txt)
{
  if(txt == '4')
  {
    Flow.writeMicroseconds(1300);
    delay(F3);
    Flow.writeMicroseconds(1500);
    delay(STOP);
    Dong_Move();
    Flow.writeMicroseconds(1700);
    delay(RF3);
    Flow.writeMicroseconds(1500);
    delay(100);
  }
  else if(txt == '3')
  {
    Flow.writeMicroseconds(1300);
    delay(F2);
    Flow.writeMicroseconds(1500);
    delay(STOP);
    Dong_Move();
    Flow.writeMicroseconds(1700);
    delay(RF2);
    Flow.writeMicroseconds(1500);
    delay(100);
  }
  else if(txt == '2')
  {
    Flow.writeMicroseconds(1300);
    delay(F1);
    Flow.writeMicroseconds(1500);
    delay(STOP);
    Dong_Move();
    Flow.writeMicroseconds(1700);
    delay(RF1);
    Flow.writeMicroseconds(1500);
    delay(100);
  }

  F_flag = 0;
}//Flow_Move END

void Dong_Move()
{
  if(D_State == 2)
  {
    Dong.writeMicroseconds(1700);
    delay(RMVD);
    Dong.writeMicroseconds(1500);
    delay(100);
    D_State = 1;
  }
}//Dong_Move END

void Action_Select(char txt)
{
  if(txt == 't' || txt == 'd')
  {
    lcd.clear();
    lcd.print("ACTION");
    lcd.setCursor(0, 1);
    lcd.print(txt);
    boardBT.write(txt);
  }
  else if(txt == 'c')
  {
    lcd.clear();
    lcd.print("Cancle...");
    delay(3000);
    lcd.clear();
  }
  else if(txt == 'a')
  {
    lcd.clear();
    lcd.print("Car Control");
    lcd.setCursor(0, 1);
    Car_flag = 1;
    while(Car_flag)
    {
      if(mobBT.available())
      {
        char str = (char)mobBT.read();
        if(str == 'f')
        {
          lcd.print("forward");
          boardBT.write(str);
          delay(1000);
          lcd.print("                  ");
        }
        else if(str == 'l')
        {
          lcd.print("left");
          boardBT.write(str);
          delay(1000);
          lcd.print("                  ");
        }
        else if(str == 'r')
        {
          lcd.print("right");
          boardBT.write(str);
          delay(1000);
          lcd.print("                  ");
        }
        else if(str == 'c')
        {
          lcd.print("Cancle...");
          Car_flag = 0;
        }
      }
    }
  }

  A_flag = 0;
}//Action_Select END

void Admin_Flow_Control(char txt)
{
  if(txt == 'f')
  {
    Flow.writeMicroseconds(1300);
    delay(1000);
    Flow.writeMicroseconds(1500);
    delay(100);
  }
  else if(txt == 'r')
  {
    Flow.writeMicroseconds(1700);
    delay(1000);
    Flow.writeMicroseconds(1500);
    delay(100);
  }
  else if(txt == 'c')
  {
    F_flag = 0;
  }
}//Admin_Flow_Control END
