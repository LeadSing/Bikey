#include <Wire.h>
#define Direccion_Esclavo 0x3C

const int temperatura = A0;
int valor10b;
int valor8b;
const int revoluciones = A0;
int velocidad10b;
int velocidad8b;
unsigned long duracion;
unsigned long duracion_maxima;
float diametro = 0.254; //En metros
float pi = 3.1416;
float frecuencia;
float rpm;
float velocidad;
 
void setup() {
  Wire.begin(Direccion_Esclavo);
  Serial.begin(9600);
  Wire.onRequest(Sensor_Temperatura);
  
  Wire.begin(Direccion_Esclavo);
  Wire.onRequest(Velocimetro);
}

void loop() {
  valor10b = analogRead(temperatura);
  valor8b = map(valor10b, 0, 1023, 0, 255);
  Serial.println(valor10b);
  Serial.println(valor8b);
  if(valor10b <= 164){
      Serial.println("La temperuta excede los 130°C");
    }
  else {
      Serial.println("La temperatura es adecuada");
    }
  delay(1000);
}


void Sensor_Temperatura (){
  valor10b = analogRead(temperatura);
  valor8b = map(valor10b, 0, 1023, 0, 255);
  Wire.write(valor8b);
  }


void Velocimetro(){
  duracion = pulseIn(revoluciones, HIGH);
  frecuencia = 1/duracion;
  rpm = frecuencia*60;
  velocidad = (pi * diametro * rpm * 60)/100;
  velocidad8b = round(velocidad);
  
  Wire.write(velocidad8b);
  }