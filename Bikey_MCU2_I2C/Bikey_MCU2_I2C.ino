/*Este microcontrolador verifica el sensor de velocidad, de presión de aceite,de revoluciones, de aire y de temperatura*/

#include <Wire.h> //Incluye la librería que permite la comunicación I2C

#define Direccion_Esclavo 0x10 //Define la dirección de esclavo del Microcontrolador como 0X10

/*********************** Definición de puertos de I/O **********************************/

const int temperatura   = 4; //Define el puerto 4 como la entrada analógica del sensor de temperatura
const int revoluciones  = 5; //Define el puerto 5 como la entrada digital del sensor de velocidad
const int aceite        = 6; //Define el puerto 6 como la entrada analógica del sensor de aceite
const int aire          = 7; //Define el puerto 7 como la entrada analógica del sensor de aire

/************************ Declaración de variables auxiliares ********************************/

int rtemperatura    = HIGH;
int raceite         = HIGH;
int raire           = HIGH;

int atemperatura    = LOW; //Inicializa la variable atemperatura en LOW
int aaceite         = LOW; //Inicializa la variable aaceite en LOW
int aaire           = LOW; //Inicializa la variable aaire en LOW

unsigned long duracion;
float diametro      = 0.254; //Valor del diámetro de la llanta delantera
float pi            = 3.1416; //Valor de pi
float frecuencia    = 0;
float rpm           = 0;
float velocidad     = 0;
int velocidadRound  = 0;
int PPR             = 1; //Valor de los pulsos por revolución

byte mandar[] = {0,0,0,0}; //Arreglo de números que será enviado con la comunicación I2C

/**************************** Configuración del microcontrolador ************************/

void setup() {
  Wire.begin(Direccion_Esclavo); //Se inicializa la comunicación I2C con este microcontrolador como esclavo con dirección 0X10

  pinMode(temperatura, INPUT_PULLUP);
  pinMode(revoluciones, INPUT_PULLUP);
  pinMode(aceite, INPUT_PULLUP);
  pinMode(aire, INPUT_PULLUP);

  Wire.onRequest(Escribir); //Se ejecutará la función Escritura cada vez que el maestro lo pida
}

/****************************** Función Loop **********************************************/

void loop() {}

/******************************* Función Escribir *******************************************/

void Escribir() {
  
  //Obtención del valor de la velocidad
  duracion = pulseIn(revoluciones, HIGH);
  frecuencia = 1.0/duracion;
  rpm = (frecuencia*60)/PPR;
  velocidad = (pi * diametro * rpm * 60)/100; //Obtiene el valor de la velocidad
  velocidadRound = round(velocidad); //Redondea el valor de la velocidad.
  mandar[0] = (byte) (velocidadRound & 0XFF); //Asigna el valor del primer byte de la velocidad a la posición 0 del arreglo mandar

  //Obtención del valor de la velocidad
  rtemperatura = digitalRead(temperatura); //Obtiene la lectura digital del valor de la temperatura
  if(atemperatura != rtemperatura){ //Si hay un cambio en el valor de la temperatura
    mandar[1] = (byte)79; //Se le asigna el valor de 81 (P) a la posición 2 del arreglo a mandar
  }

  //Obtención del valor del sensor de presión aceite
  raceite = digitalRead(aceite); //Obtiene la lectura digital del valor del aceite
  if(aaceite != raceite){ //Si hay un cambio en el valor del aceite
    mandar[2] = (byte)81; //Asigna el valor de 83 (R) a la posición 3 del arreglo a mandar
  }

  //Obtención del valor del sensor del filtro de aire
  raire = digitalRead(aire); //Obtiene la lectura digital del valor del aire
  if (aaire != raire){  //Cuando haya un cambio en el valor del aire
    mandar[3] = (byte)83; //Asigna el valor de 85 (T) a la posición 4 del arreglo a mandar
  }

  Wire.write(mandar, 4);
}

