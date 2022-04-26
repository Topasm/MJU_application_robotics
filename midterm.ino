#include "HUSKYLENS.h"
#include <PRIZM.h>
#define GAIN 0.1
#define THRESHOLD 110

PRIZM prizm;
HUSKYLENS huskylens;
HUSKYLENSResult result;
int object = 10;
int have = 102;
int d = 0;
int s = 0;
int c = 1000;
int o_row = 123;
int o_col = 123;
int r_row = 1;
int r_col = 0;
int d_row = 1;
int d_col = 1;
int bsetpoint = 0;

int loc1 = 0; //첫번째 위치인 경우
void printResult(HUSKYLENSResult result);
void turn_right();
void turn_left();
void distance();
void forward();
// void backward();
void dforward();
void dbackward();
void linesensor();
void linefollow();
void stop();
void grab();
void up();
void down();
void release();
void turn_back();

void setup()
{
  prizm.PrizmBegin();
  prizm.setMotorInvert(1, 1);
  prizm.setServoSpeed(1, 25); // set servo 1 speed to 25%
  prizm.setServoSpeed(2, 25); // set servo 2 speed to 25%
  prizm.setServoPosition(3, 0);
  prizm.setServoPosition(4, 180);
  prizm.setServoPosition(2, 180);
  delay(1000);

  pinMode(A1, INPUT);

  Serial.begin(9600);
  Wire.begin();
  while (!huskylens.begin(Wire))
  {
    Serial.println(F("Begin Failed!"));
    delay(100);
  }
}

void loop()
{
  linefollow();
  if (s == 1 && o_col == 123 && o_row == 123)
  {

    forward();
    o_col = 0;
    o_row = 0;
    stop();
    distance();
    delay(50);
    s = 0;
  }
  // o_col 이 2 를 넘으면 안됨

  if (d == 0 && o_col != 123)
  {
    // i열 0행인 경우
    dforward();
    o_row = d + 2;
    stop();
    grab();
    up();

    stop();
    turn_back();
    forward();
    stop();

    goto btob;
    
  }
  else if (d == 3 && o_col != 123)
  {
    // 0열에는 없다
    turn_right();
    stop();

    o_col++;
    s = 0;

    while (1)
    {
      linefollow();
      if (s == 1)
      {
        // col 이동
        s = 0;
        forward();
        stop();
        turn_right();
        stop();
        forward();
        stop();
        turn_back();
        stop();
        dforward();
        delay(100);
        stop();
        distance();
        delay(50);

        break;
      }
    }
  }
  else if (d != 0 && d != 3 && o_col == 0)
  {
    o_row = d + 2;

    while (1)
    {
      linefollow();
      if (d == 0)
      {
        dforward();

        stop();
        grab();
        up();
        dbackward();
        turn_back();
        forward();
        goto btob;

        break;
      }
    }
  }
  else if (d != 0 && d != 3 && o_col != 0 && o_col != 123)
  {
    o_row = d + 2;
    forward();
    forward();
    while (1)
    {
      linefollow();
      if (d == 0)
      {

        dforward();
        stop();
        grab();
        up();
        dbackward();
        turn_back();
        forward();
        goto btob;

        break;
      }
    }
  }

btob:
  if (have == 1 && o_col != 123 && bsetpoint == 0)
  {
    //0열일때 셋포인트로
    while (have == 1 && r_col == 0) // && r_row > 1
    {
      linefollow();
      if (s == 1)
      {
        prizm.setMotorSpeeds(-200, -200);
        delay(1200);
        stop();
        r_row = 1;
        break;
      }
    }
    //0열이 아닐때, 3행 셋포인트로

    while (have == 1 && r_col != 0 && r_row == 3)
    {
      linefollow();
      if (s == 1)
      {
        forward();
        forward();
        forward();
        stop();
        r_row = 1;
      }
      if (s == 1 && r_row == 1)
      {
        prizm.setMotorSpeeds(-200, -200);
        delay(1200);
        stop();
        r_row = 1;
        break;
      }
    }

    if(have == 1 && r_col != 0 && r_row == 2)
    {

      forward();
      forward();
      stop();
      r_row = 1;
      //이부분 작동하나?
      while(1){
        linefollow();
      if (s == 1 && r_row == 1)
      {
        prizm.setMotorSpeeds(-200, -200);
        delay(1200);
        stop();
        r_row = 1;
        break;
        
      }
      }
    }

    
    bsetpoint = 1;
    Serial.println("back to set point");
    
    delay(100);
    // 1행에 전부 도착
  }



  if (have == 1 && o_col != 123 && bsetpoint == 1 )
  {

    while (have == 1 && r_row == 1 && r_col > d_col)
    {
      linefollow();
      if (s == 1)
      {
        
        s = 0;
        forward();
        stop();
        turn_right();
        stop();

      
        while (1)
        {
          s = 0;
          linefollow();
          if (s == 1)
          {
            r_col = 1;
            s = 0;
            forward();
            stop();
            turn_left();
            stop();
            down();
            release();
            prizm.setMotorSpeeds(-200, -200);
            delay(1850);
            stop();
            //딜레이값 조정 필요  ******
            turn_right();
            stop();

          
            break;
          }
        }
        break;
      }

      
    }

    while (have == 1 && r_row == 1 && r_col < d_col)
    {
      stop();
      turn_left();
      s = 0;
      while (1)
      {
        s = 0;
        linefollow();
        if (s == 1)
        {
          s = 0;
          forward();
          stop();
          r_col++;
        }
        if (r_col == d_col)
        {
          turn_right();
          break;
        }
      }
      break;
    }

    //캔이나 병의 열이 로봇의 열과 같을때
    while (have == 1 && r_row == 1 && r_col == d_col)
    {
      linefollow();

      if (s == 1)
      {
        s = 0;
        forward();
        stop();
        down();
        release();
        prizm.setMotorSpeeds(-200, -200);
        delay(1400);
        while (1)
        {
          s = 0;
          prizm.setMotorSpeeds(-200, -200);
          linesensor();

          if (s == 1)
          {
            s = 0;
            stop();
            forward();
            delay(50);
            stop();
            break;
          }
        }

        turn_right();
        break;
      }
    }
    //다 하고 나서

    while (have != 1 && r_row == 1)
    {
      linefollow();
      if (s == 1)
      {
        s = 0;
        forward();
        stop();
        turn_right();
        stop();
        r_col = 0;
        bsetpoint = 0;
        break;
      }
    }
  }
}

//*****************************************************************

//*****************************************************************

void linesensor()
{
  float s1, s2, s3, s4, s5;
  s1 = prizm.readLineSensor(3);
  s2 = prizm.readLineSensor(3);
  s3 = prizm.readLineSensor(3);

  s4 = s1 + s2 + s3;
  s5 = s4 / 3;
  if (s5 >= 0.5)
  {
    s = 1;
  }
  else
  {
    s = 0;
  }
}
void linefollow()
{
  int sensor = analogRead(1);
  float sig = (sensor - THRESHOLD) * GAIN;
  prizm.setMotorSpeeds(200 - sig, 200 + sig);
  linesensor();
  distance();
}

void release()
{
  prizm.setServoPosition(3, 0);
  prizm.setServoPosition(4, 180);
  delay(1000);
  have = 100;
  object = 123;
  r_row = 1;
  r_col = d_col;
  o_col = 0; // 2번째 실행
}

void grab()
{
  prizm.setServoPosition(3, 70);
  prizm.setServoPosition(4, 95);
  husky();
  delay(1000);
  have = 1;
  r_row = o_row - 1;
  r_col = o_col;
}

void up()
{
  prizm.setServoPosition(2, 110);
  delay(1000);
}

void down()
{
  prizm.setServoPosition(2, 180);
  delay(1000);
}

void stop()
{
  prizm.setMotorSpeeds(0, 0);
  delay(100);
  distance();
  Serial.print("o_row");
  Serial.println(o_row);
  Serial.print("o_col");
  Serial.println(o_col);
  Serial.print("r_row");
  Serial.println(r_row);
  Serial.print("r_col");
  Serial.println(r_col);
  Serial.print("distance");
  Serial.println(d);

  delay(100);
}
void forward()
{
  prizm.setMotorSpeeds(200, 200);
  delay(870);
}
void dforward()
{
  prizm.setMotorSpeeds(200, 200);
  delay(600);
}

void dbackward()
{
  prizm.setMotorSpeeds(-200, -200);
  delay(600);
}

void turn_right()
{
  prizm.setMotorSpeeds(200, -200);
  delay(1550);
  prizm.setMotorSpeeds(0, 0);
}
void turn_left()
{
  prizm.setMotorSpeeds(-200, 200);
  delay(1550);
  prizm.setMotorSpeeds(0, 0);
}
void turn_back()
{
  prizm.setMotorSpeeds(200, -200);
  delay(3100);
  prizm.setMotorSpeeds(0, 0);
}

void husky()
{

  if (!huskylens.request() || !huskylens.isLearned() || !huskylens.available())
  {
    Serial.println(F("Recheck the connections"));
  }
  else
  {
    while (huskylens.available())
    {
      HUSKYLENSResult result = huskylens.read();
      if (result.command == COMMAND_RETURN_BLOCK)
      {
        Serial.println(String() + F(",ID=") + result.ID);
        if (result.ID == 1)
        {

          object = 0;
          d_row = 0;
          d_col = 1;
        }
        else if (result.ID == 2)
        {

          object = 1;
          d_row = 0;
          d_col = 2;
        }
        else
        {
        }
      }
      else
      {
        Serial.println("object unknown");
      }
    }
  }
}
// 90 도 돌면 x to y y to x 180 도 돌면 부호 반대
void distance()
{
  if (prizm.readSonicSensorCM(2) <= 7)
  {
    d = 0;
  }
  else if (prizm.readSonicSensorCM(2) <= 50)
  {
    d = 1;
  }
  else if (prizm.readSonicSensorCM(2) <= 120)
  {
    d = 2;
  }
  else
  {
    d = 3;
  }
}

//잡으면 무조건 180도