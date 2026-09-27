int r1 = 13, a1 = 12, v1 = 11;
int r2 = 10, a2 = 9, v2 = 8, g2 = 7;
int r3 = 6, a3 = 5, v3 = 4;
int r4 = A3, a4 = A2, v4 = A1, g4 = A0;

void semaforo(int r, int a, int v, int prender)
{
  digitalWrite(r, LOW);
  digitalWrite(a, LOW);
  digitalWrite(v, LOW);
  digitalWrite(prender, HIGH);
}

void apagarGiros()
{
  digitalWrite(g2, LOW);
  digitalWrite(g4, LOW);
}

void todosRojo()
{
  apagarGiros();
  semaforo(r1, a1, v1, r1);
  semaforo(r2, a2, v2, r2);
  semaforo(r3, a3, v3, r3);
  semaforo(r4, a4, v4, r4);
}

void abrir(int rA, int aA, int vA, int rB, int aB, int vB)
{
  apagarGiros();
  semaforo(rA, aA, vA, aA);
  semaforo(rB, aB, vB, aB);
  delay(1000);
  semaforo(rA, aA, vA, vA);
  semaforo(rB, aB, vB, vB);
}

void cerrar(int rA, int aA, int vA, int rB, int aB, int vB)
{
  semaforo(rA, aA, vA, aA);
  semaforo(rB, aB, vB, aB);
  delay(1000);
  semaforo(rA, aA, vA, rA);
  semaforo(rB, aB, vB, rB);
}

void giro(int g)
{
  todosRojo();
  delay(1000);
  digitalWrite(g, HIGH);
  delay(2000);
  apagarGiros();
  delay(1000);
}

void setup()
{
  for (int pin = 4; pin <= A3; pin++)
    pinMode(pin, OUTPUT);
  todosRojo();
}

void loop()
{
  abrir(r1, a1, v1, r4, a4, v4);
  delay(3000);
  cerrar(r1, a1, v1, r4, a4, v4);
  delay(1000);
  giro(g2);
  abrir(r2, a2, v2, r3, a3, v3);
  delay(3000);
  cerrar(r2, a2, v2, r3, a3, v3);
  delay(1000);

  giro(g4);
}