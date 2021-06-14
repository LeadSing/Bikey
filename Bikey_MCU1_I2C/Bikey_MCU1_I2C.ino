#include <Wire.h> //Incluye la librería que permite la comunicación I2C

#define Direccion_Esclavo 0x08 //Define la dirección de esclavo del Microcontrolador como 0X08

/************************ Definición de puertos I/O **************************/

const int cuarto  = 4; //Define el puerto 4 como la entrada de los cuartos
const int baja    = 5; //Define el puerto 5 como la entrada de la luz baja
const int alta    = 6; //Define el puerto 6 como la entrada de la luz alta
const int stand   = 8; //Define el puerto 8 como la entrada del indicador de la palanca lateral
const int neutro  = 9; //Define el puerto 9 como la entrada del indicador del Neutro del motor
const int izq     = 10; //Define el puerto 10 como la entrada de la direccional izquierda
const int der     = 11; //Define el puerto 11 como la entrada de la direccional derecha.
const int gas     = A0; //Define el puerto A0 como la entrada de la lectura del combustivel

/************************ Inicialización de variables **************************/

int rcuarto   = HIGH; //Inicializa la variable rcuarto en HIGH
int rbaja     = HIGH; //Inicializa la variable rbaja en HIGH
int ralta     = HIGH; //Inicializa la variable ralta en HIGH
int rstand    = HIGH; //Inicializa la variable rstand en HIGH
int rneutro   = HIGH; //Inicializa la variable rneutro en HIGH
int rizq      = HIGH; //Inicializa la variable rizq en HIGH
int rder      = HIGH; //Inicializa la variable ider en HIGH

int acuarto   = LOW; //Inicializa la variable acuarto en LOW
int abaja     = LOW; //Inicializa la variable abaja en LOW
int aalta     = LOW; //Inicializa la variable aalta en LOW
int astand    = LOW; //Inicializa la variable astand en LOW
int aneutro   = LOW; //Inicializa la variable aneutro en LOW
int aizq      = LOW; //Inicializa la variable aizq en LOW
int ader      = LOW; //Inicializa la variable ader en LOW

/************************ Declaración de variables auxiliares **************************/

int lectura     = 0; //Variable en donde se almacenará el valor de la lectura analógica 
int alectura    = 0; //Variable en donde será almacenado el valor anterior de la lectura analógica
int Ve          = 5; //voltaje lógico del microcontrolador
float relacion  = 0;
float VR2       = 0; //Variable en donde será almacenado el valor de la lectura analógica convertida a voltaje
float R1        = 11000; //Valor de R1
int R2          = 0; //Variable en donde será almacenado el valor de la resistencia del combustivel
byte mandar[8] = {0,0,0,0,0,0,0,0}; //Arreglo de bytes que será mandado mediante el protocolo I2C

/************************ Configuración del microcontrolador **************************/

void setup() {
  Wire.begin(Direccion_Esclavo); //Se inicializa la comunicación I2C definiendo a este microcontrolador como esclavo con la dirección 0X08
  //Se define cada puerto de I/O como entrada y se activan las resistencias de pull-up internas
  //pinMode(int1,   INPUT_PULLUP);
  //pinMode(int2,   INPUT_PULLUP);
  pinMode(cuarto, INPUT_PULLUP);
  pinMode(baja,   INPUT_PULLUP);
  pinMode(alta,   INPUT_PULLUP);
  pinMode(stand,  INPUT_PULLUP);
  pinMode(neutro, INPUT_PULLUP);
  pinMode(izq,    INPUT_PULLUP);
  pinMode(der,    INPUT_PULLUP);

  Wire.onRequest(Escribir); //Se inicializa la función que ejecutará la función Escribir cada vez que el maestro se lo pida

  //attachInterrupt(digitalPinToInterrupt(int1),fizq,LOW); //Se inicializa una interrupción que ejecutará la función fizq cuando el valor int1 esté en LOW
  //attachInterrupt(digitalPinToInterrupt(int2),fder,LOW); //Se inicializa una interrupción que ejecutará la función fder cuando el valor int2 esté en LOW
}

/************************ Función Loop **************************/

void loop() {} //Se deja la función Loop vacía porque no es necesaria para la comunicación mediante I2C

/************************ Función Escribir **************************/

void Escribir (){ //Esta es la función que será ejecutada cuando el maestro lo pida

  //Obtención del valor del sensor de combustible
  lectura = analogRead(gas); //Obtiene la lectura analógica del sensor de combustible
  if (lectura != alectura){ //Cuando haya un cambio en el valor de la lectura
    relacion = lectura * Ve; 
    VR2 = (relacion)/1024.0;
    relacion = (Ve/VR2)-1;
    R2 = R1 *relacion;
    mandar[0] = (byte)R2; //Se le asigna el valor de R2 (Combustivel) a la posición 0 del arreglo mandar
    alectura = lectura;
  }

  //Obtención del valor de los cuartos
  rcuarto = digitalRead(cuarto); //Obtiene el valor digital de los cuartos
  if (acuarto != rcuarto){ //Cuando haya un cambio en el valor de los cuartos
    mandar[1] = (byte)65; //Se le asigna el valor de 65 (A) a la posición 1 del arreglo a mandar
    acuarto = rcuarto;
  }

  //Obtención del valor de la luz baja
  rbaja = digitalRead(baja); //Se obtiene el valor digital de la luz baja
  if (abaja != rbaja){ //Cuando haya un cambio en el valor de la luz baja
    mandar[2] = (byte)67; //Se le asigna el valor de 67 (C) a la posición 2 del arreglo a mandar
    abaja = rbaja;
  }

  //Obtención del valor de la luz alta
  ralta = digitalRead(alta); //Obtiene el valor digital de la luz alta
  if (aalta != ralta){ //Cuando haya un cambio en el valor de la luz baja
    mandar[3] = (byte)69; //Se le asigna el valor de 69 (E) a la posición 3 del arreglo a mandar
    aalta = abaja;
  }

  //Obtención del valor del indicador de la palanca lateral
  rstand = digitalRead(stand); //Obtiene el valor digital de la palanca lateral
  if (astand != rstand){ //Cuando cambie el valor de la palanca lateral
    mandar[5] = (byte)71; //Se le asigna el valor de 73 (I) a la posición 5 del arreglo a mandar
    astand = rstand;
  }

  //Obtención del valor del indicador del Neutro
  rneutro = digitalRead(neutro); //Obtiene el valor digital del Neutro
  if (aneutro != rneutro){ //Cuando haya un cambio en el valor del Neutro
    mandar[6] = (byte)73; //Se le asigna el valor de 75 (K) a la posición 6 del arreglo a mandar
    aneutro = rneutro;
  }

  //Obtención del valor de la direccional izquierda
  rizq = digitalRead(izq); //Otiene el valor digital de la direccional izquierda
  if (aizq != rizq){ //Cuando haya un cambio en el valor de la direccional izquierda
    mandar[7] = (byte)75; //Se le asigna el valor de 77 (M) a la posición 6 del arreglo a mandar
    aizq = rizq;
  }

  //Obtención del valor de la direccional derecha
  rder = digitalRead(der); //Obtiene el valor digital de la direccional derecha
  if (ader != rder){ //Cuando haya un cambio en el valor de la direccional derecha
    mandar[8] = (byte)77; //Se le asigna el valor de 79 (O) a la posición 7 del arreglo a mandar
    ader = rder; 
  }

  Wire.write(mandar, 9); //Envía los 9 datos del arreglo mandar al maestro

}
