int rv = 13, av = 12, vv = 11;
int rp = 10, vp = 9;
int boton = 8;

unsigned long inicioVerde;

void vehiculo(int r, int a, int v)
{
  digitalWrite(rv, r);
  digitalWrite(av, a);
  digitalWrite(vv, v);
}

void peaton(int r, int v)
{
  digitalWrite(rp, r);
  digitalWrite(vp, v);
}

bool noche()
{
  return millis() % 60000 >= 30000;
}

void esperarBoton()
{
  while (digitalRead(boton) == LOW)
    ;
  delay(50);
}

void cruce(bool modoNoche)
{

  if (!modoNoche)
  {
    while (millis() - inicioVerde < 5000)
      delay(10);
  }

  peaton(HIGH, LOW);

  vehiculo(LOW, HIGH, LOW);
  delay(1500);

  vehiculo(HIGH, LOW, LOW);
  delay(500);

  peaton(LOW, HIGH);
  delay(5000);

  peaton(HIGH, LOW);
  delay(500);

  if (!modoNoche)
  {
    vehiculo(LOW, LOW, HIGH);
    inicioVerde = millis();
  }
}

void setup()
{

  pinMode(rv, OUTPUT);
  pinMode(av, OUTPUT);
  pinMode(vv, OUTPUT);

  pinMode(rp, OUTPUT);
  pinMode(vp, OUTPUT);

  pinMode(boton, INPUT_PULLUP);

  vehiculo(LOW, LOW, HIGH);
  peaton(HIGH, LOW);

  inicioVerde = millis();
}

void loop()
{

  if (noche())
  {

    peaton(HIGH, LOW);

    vehiculo(LOW, HIGH, LOW);
    delay(400);

    if (digitalRead(boton) == LOW)
    {
      esperarBoton();
      cruce(true);
      return;
    }

    vehiculo(LOW, LOW, LOW);
    delay(400);

    if (digitalRead(boton) == LOW)
    {
      esperarBoton();
      cruce(true);
      return;
    }
  }
  else
  {

    vehiculo(LOW, LOW, HIGH);
    peaton(HIGH, LOW);

    if (digitalRead(boton) == LOW)
    {
      esperarBoton();
      cruce(false);
    }
  }
}