int led = 13;
int buzzer = 12;
int boton = 11;

bool alarma = false;

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(boton, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(boton) == LOW)
  {
    alarma = !alarma;
    while (digitalRead(boton) == LOW)
      ;
    delay(50);
  }

  if (alarma)
  {
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(led, HIGH);
      tone(buzzer, 1000);
      delay(200);
      digitalWrite(led, LOW);
      noTone(buzzer);
      delay(200);
    }
    delay(600);
  }
  else
  {
    digitalWrite(led, LOW);
    noTone(buzzer);
  }
}