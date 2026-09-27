#include <Servo.h>

int trig = 10;
int echo = 9;
int led = 8;
int motor = 6;

Servo barrera;

float distancia()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long tiempo = pulseIn(echo, HIGH);

  return tiempo * 0.034 / 2;
}

void setup()
{

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led, OUTPUT);

  barrera.attach(motor);

  barrera.write(0);
  digitalWrite(led, LOW);
}

void loop()
{

  float d = distancia();

  if (d <= 20)
  {

    barrera.write(90);
    digitalWrite(led, HIGH);
  }
  else
  {

    barrera.write(0);
    digitalWrite(led, LOW);
  }

  delay(200);
}