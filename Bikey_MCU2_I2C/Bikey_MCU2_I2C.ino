/*Este microcontrolador verifica el sensor de velocidad, de presión de aceite,de revoluciones, de aire y de temperatura*/

#include <Wire.h> //Incluye la librería que permite la comunicación I2C

#define Direccion_Esclavo 0x10 //Define la dirección de esclavo del Microcontrolador como 0X10

/*********************** Definición de puertos de I/O **********************************/

const int temperatura   = A0; //Define el puerto A0 como la entrada analógica del sensor de temperatura
const int revoluciones  = 7; //Define el puerto 7 como la entrada digital del sensor de velocidad
const int aceite        = A2; //Define el puerto A2 como la entrada analógica del sensor de aceite
const int aire          = A3; //Define el puerto A3 como la entrada analógica del sensor de aire

/************************ Declaración de variables auxiliares ********************************/

int temperatura10b  = 0; //Variable en donde será almacenado el valor analógico de 10 bits de la temperatura
int temperatura8b   = 0; //Variable en donde será almacenado el valor analógico de 8 bits de la temperatura

int velocidad10b    = 0; //Variable en donde será almacenado el valor analógico de 10 bits de la velocidad
int velocidad8b     = 0; //Variable en donde será almacenado el valor analógico de 8 bits de la velocidad

int aceite10b       = 0; //Variable en donde será almacenado el valor analógico de 10 bits del aceite
int aceite8b        = 0; //Variable en donde será almacenado el valor analógico de 8 bits del aceite

int aire10b         = 0; //Variable en donde será almacenado el valor analógico de 10 bits del aire
int aire8b          = 0; //Variable en donde será almacenado el valor analógico de 8 bits del aire
/*
int atemperatura    = LOW; //Inicializa la variable atemperatura en LOW
int avelocidad      = LOW; //Inicializa la variable avelocidad en LOW
int aaceite         = LOW; //Inicializa la variable aaceite en LOW
int aaire           = LOW; //Inicializa la variable aaire en LOW
*/
unsigned long duracion;
unsigned long duracion_maxima;
float diametro      = 0.254; //Valor del diámetro de la llanta delantera
float pi            = 3.1416; //Valor de pi
float frecuencia    = 0;
float rpm           = 0;
float velocidad     = 0;
int PPR             = 1; //Valor de los pulsos por revolución

byte mandar[4] = {0,0,0,0}; //Arreglo de números que será enviado con la comunicación I2C

/**************************** Configuración del microcontrolador ************************/

void setup() {
  Wire.begin(Direccion_Esclavo); //Se inicializa la comunicación I2C con este microcontrolador como esclavo con dirección 0X10

  pinMode(revoluciones, INPUT_PULLUP);

  Wire.onRequest(Escribir); //Se ejecutará la función Escritura cada vez que el maestro lo pida
}

/****************************** Función Loop **********************************************/

void loop() {}

/******************************* Función Escribir *******************************************/

void Escribir() {
  
  //Obtención del valor de la velocidad
  duracion = pulseIn(revoluciones, HIGH);
  frecuencia = 1/duracion;
  rpm = (frecuencia*60)/PPR;
  velocidad = (pi * diametro * rpm * 60)/100;
  velocidad10b = round(velocidad); //Obtiene el valor de velocidad 
  mandar[0] = (byte)velocidad10b; //Asigna el valor de velocidad 10b a la posición 0 del arreglo mandar

  //Obtención del valor de la velocidad
  temperatura10b = analogRead(temperatura); //Obtiene el valor analógico de la temperatura
  temperatura8b = map(temperatura10b, 0, 1024, 0, 255); //Lo convierte a un valor de 8 bits
  if(temperatura10b <= 164){ //Si la temperatura está debajo de un umbral
    mandar[1] = (byte)250; //La temperatura del motor es muy elevada
  }
  else{
    mandar[1] = (byte)5; //La temperatura dle motor es adecuada
  }

  //Obtención del valor del sensor de presión aceite
  aceite10b = analogRead(aceite);
  aceite8b = map(aceite10b, 0, 1024, 0, 255);
  if(aceite10b <= 164){
    mandar [2] = (byte)250;
  }
  else{
    mandar[2] = (byte)5;
  }

  //Obtención del valor del sensor del filtro de aire
  aire10b = analogRead(aire);
  aceite8b = map(aceite10b, 0, 1024, 0, 255);
  if (aire10b <= 164){
    mandar[3] = (byte)250;
  }
  else{
    mandar[3] = (byte)5;
  }

  Wire.write(mandar, 4);
}

