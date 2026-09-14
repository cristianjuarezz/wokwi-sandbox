/*
  Escenario I: Mejora de transiciones
  Como hubo algunos accidentes por las transiciones abruptas, la municipalidad solicitó mejorar las secuencias.
*/

enum Status {
    Verde,
    Amarillo,
    Rojo
};

typedef int PIN_ID;

struct Semaphore {
    Status status;
    PIN_ID red;
    PIN_ID yellow;
    PIN_ID green;
};

struct Semaphore semaphore1 = {Rojo, 13, 12, 11};
struct Semaphore semaphore2 = {Rojo, 4, 3, 2};

// Setters
void SetSemaphoreA(Status newStatus)
{
  semaphore1.status = newStatus;
  OffLEDs(semaphore1);
  TurnOnStatusLED(semaphore1);

}
void SetSemaphoreB(Status newStatus)
{
  semaphore2.status = newStatus;
  OffLEDs(semaphore2);
  TurnOnStatusLED(semaphore2);
}

void OffLEDs(Semaphore s)
{
  digitalWrite(s.red, LOW);
  digitalWrite(s.yellow, LOW);
  digitalWrite(s.green, LOW);
}

void TurnOnStatusLED(Semaphore s)
{
  if (s.status == Rojo) { digitalWrite(s.red, HIGH); }
  if (s.status == Amarillo) { digitalWrite(s.yellow, HIGH); }
  if (s.status == Verde) { digitalWrite(s.green, HIGH); }
}

void setup() // Habilito ambos semaforos
{
  // Habilitar pines de semaforo A
  pinMode(semaphore1.red, OUTPUT);
  pinMode(semaphore1.yellow, OUTPUT);
  pinMode(semaphore1.green, OUTPUT);

  // Habilitar pines de semaforo B
  pinMode(semaphore2.red, OUTPUT);
  pinMode(semaphore2.yellow, OUTPUT);
  pinMode(semaphore2.green, OUTPUT);
}

// ASignacion de fases
/*
  PSEUDOCODIGO:

  KeyPair values
  pares Dictionary: = {
    "Verde":    "Rojo", // Fase 1
    "Amarillo": "Rojo", // Fase 2
    "Rojo":     "Rojo", // Fase 3
    "Rojo":     "Amarillo", // Fase 4
    "Rojo":     "Verde", // Fase 5
  }

*/
void EstablecerFase1(){
  SetSemaphoreA(Verde);
  SetSemaphoreB(Rojo);
}
void EstablecerFase2(){
  SetSemaphoreA(Amarillo);
  SetSemaphoreB(Rojo);
}
void EstablecerFase3TodoRojo(){
  SetSemaphoreA(Rojo);
  SetSemaphoreB(Rojo);
}
// ---
void EstablecerFase4(){
  SetSemaphoreA(Rojo);
  SetSemaphoreB(Amarillo);
}
void EstablecerFase5(){
  SetSemaphoreA(Rojo);
  SetSemaphoreB(Verde);
}

// Loop
// digitalWrite(13, HIGH); 
// void RefreshSemaphoreA(){
//   digitalWrite(13, HIGH);
//   delay(300);
//   digitalWrite(13, LOW);
//   delay(300);
// }
// void RefreshSemaphoreB(){
//   digitalWrite(12, HIGH);
//   delay(300);
//   digitalWrite(12, LOW);
//   delay(300);
// }

// SUpongamos que: tiempo entre fases sea 7 segundos
void loop()
{
  CicloDeVida();
  // EstablecerFase1();
  // RefreshSemaphoreA();
  // RefreshSemaphoreB();
}

float segundosPorFase = 0.2;
void CicloDeVida(){
  EstablecerFase1();
  delay(1000*segundosPorFase);
  EstablecerFase2();
  delay(1000*segundosPorFase);
  EstablecerFase3TodoRojo();
  delay(1000*segundosPorFase);
  EstablecerFase4();
  delay(1000*segundosPorFase);
  EstablecerFase5();
  delay(1000*segundosPorFase);
}