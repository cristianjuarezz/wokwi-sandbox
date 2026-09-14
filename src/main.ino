/*
  Escenario II: Giro en intersecciones
  
  Se modificaron algunas calles de Hurlingham, incorporándole giro en algunas
  ocasiones. Como, por ejemplo, en la intersección de las calles Corraceros y Pedro
  Díaz, donde los semáforos permiten giros en las direcciones correspondientes, y
  los vehículos deben seguir las señales para evitar accidentes y mantener el flujo
  adecuado del tráfico. 

  Para girar a la izq, el contrario debe estar rojo
*/
#include <stdio.h>
using namespace std; 
enum Status {
    Verde,
    Amarillo,
    Rojo
};
const char* StatusNames[] = {
    "Verde",
    "Amarillo",
    "Rojo"
};

typedef int PIN_ID;

struct Semaphore {
    Status status;
    PIN_ID red;
    PIN_ID yellow;
    PIN_ID green;
};

struct TurnLeft {
    bool isOn;
    PIN_ID red;
};

struct Semaphore semaphore1 = {Rojo, 13, 12, 11};
struct Semaphore semaphore2 = {Rojo, 4, 3, 2};
struct Semaphore semaphore3 = {Rojo, 4, 3, 2};
struct Semaphore semaphore4 = {Rojo, 4, 3, 2};
struct TurnLeft ThreeToLeft = {false, 0};
struct TurnLeft TwoToLeft = {false, 0};

// Setters
void SetSemaphore(Semaphore s, Status newStatus)
{
  s.status = newStatus;
  OffLEDs(s);
  TurnOnStatusLED(s);
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

void EnableSemaphore(Semaphore s)
{
  pinMode(s.red, OUTPUT);
  pinMode(s.yellow, OUTPUT);
  pinMode(s.green, OUTPUT);
}

void setup() // Habilito ambos semaforos
{
  // Habilitar pines de semaforo
  EnableSemaphore(semaphore1);
  EnableSemaphore(semaphore2);
  EnableSemaphore(semaphore3);
  EnableSemaphore(semaphore4);
}

float segundosPorFase = 0.5;
void EsperarEtapa()
{
  printf("ESTADOS");
  printf(StatusNames[semaphore1.status]);
  printf(StatusNames[semaphore2.status]);
  printf(StatusNames[semaphore3.status]);
  printf(StatusNames[semaphore4.status]);
  printf("=======");
  delay(1000*segundosPorFase);
}

bool turnoDe3 = true;

// Fases
void VVRR(){
  SetSemaphore(semaphore1, Verde);
  SetSemaphore(semaphore4, Verde);
  SetSemaphore(semaphore2, Rojo);
  SetSemaphore(semaphore3, Rojo);
  AARR();
}
void AARR(){
  SetSemaphore(semaphore1, Amarillo);
  SetSemaphore(semaphore4, Amarillo);
  SetSemaphore(semaphore2, Rojo);
  SetSemaphore(semaphore3, Rojo);
  RRRR();
}
void RRRR(){
  SetSemaphore(semaphore1, Rojo);
  SetSemaphore(semaphore4, Rojo);
  SetSemaphore(semaphore2, Rojo);
  SetSemaphore(semaphore3, Rojo);
  RRXX();
}
void RRXX(){
  SetSemaphore(semaphore1, Rojo);
  SetSemaphore(semaphore4, Rojo);
  // O activar el 3 o activar el 2, nunca juntos
  if (turnoDe3) {
    RRAR();
  } else {
    RRRA();
  }
}
void RRAR(){
  SetSemaphore(semaphore1, Rojo);
  SetSemaphore(semaphore4, Rojo);
  SetSemaphore(semaphore2, Rojo);
  SetSemaphore(semaphore3, Amarillo);
  EsperarEtapa();
  SetSemaphore(semaphore3, Verde);
  EsperarEtapa();
  SetSemaphore(semaphore3, Amarillo);
  EsperarEtapa();
  turnoDe3 = false;
  RRRR();
}
void RRRA(){
  SetSemaphore(semaphore1, Rojo);
  SetSemaphore(semaphore4, Rojo);
  SetSemaphore(semaphore2, Amarillo);
  SetSemaphore(semaphore3, Rojo);
  EsperarEtapa();
  SetSemaphore(semaphore2, Verde);
  EsperarEtapa();
  SetSemaphore(semaphore2, Amarillo);
  EsperarEtapa();
  turnoDe3 = true;
  RRRR();
}

// SUpongamos que: tiempo entre fases sea 7 segundos
void loop() { 
  VVRR();
}