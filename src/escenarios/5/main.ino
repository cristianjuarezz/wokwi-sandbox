#include <DHT.h>

int ledTemp = 13;
int ledHumedad = 12;
int buzzer = 11;
int sensor = 10;

DHT dht(sensor, DHT22);

void setup()
{

  pinMode(ledTemp, OUTPUT);
  pinMode(ledHumedad, OUTPUT);
  pinMode(buzzer, OUTPUT);

  dht.begin();

  Serial.begin(9600);
}

void loop()
{

  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" C  Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  if (temperatura > 25)
    digitalWrite(ledTemp, HIGH);
  else
    digitalWrite(ledTemp, LOW);

  if (humedad > 70)
    digitalWrite(ledHumedad, HIGH);
  else
    digitalWrite(ledHumedad, LOW);

  if (temperatura > 25 || humedad > 70)
    tone(buzzer, 1000);
  else
    noTone(buzzer);

  delay(1000);
}