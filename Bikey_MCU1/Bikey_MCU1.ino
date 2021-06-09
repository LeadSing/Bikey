const int int1    = 2; 
const int int2    = 3;
const int cuarto  = 4;
const int baja    = 5;
const int alta    = 6;
const int stop    = 7;
const int stand   = 8;
const int neutro  = 9;
const int izq     = 10;
const int der     = 11;

int rcuarto = HIGH;
int rbaja   = HIGH;
int ralta   = HIGH;
int rstop   = HIGH;
int rstand  = HIGH;
int rneutro = HIGH;
int rizq    = HIGH;
int rder    = HIGH;

int acuarto = LOW;
int abaja   = LOW;
int aalta   = LOW;
int astop   = LOW;
int astand  = LOW;
int aneutro = LOW;
int aizq    = LOW;
int ader    = LOW;

int PinA0 = A0;
int lectura = 0;
int alectura = 0;
int Ve =5;
float VR2 =0;
float R1 = 11000;
float R2 = 0;
float I = 0;
float relacion = 0;

void setup() {
  Serial.begin(9600);
  pinMode(int1,   INPUT_PULLUP);
  pinMode(int2,   INPUT_PULLUP);
  pinMode(cuarto, INPUT_PULLUP);
  pinMode(baja,   INPUT_PULLUP);
  pinMode(alta,   INPUT_PULLUP);
  pinMode(stop,   INPUT_PULLUP);
  pinMode(stand,  INPUT_PULLUP);
  pinMode(neutro, INPUT_PULLUP);
  pinMode(izq,    INPUT_PULLUP);
  pinMode(der,    INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(int1),fizq,LOW);
  attachInterrupt(digitalPinToInterrupt(int2),fder,LOW);
}
void loop() {

  lectura = analogRead(PinA0);
  if(lectura != alectura){
    relacion = lectura * Ve;
    VR2 = (relacion)/1024.0;
    relacion = (Ve/VR2)-1;
    R2= R1*relacion;
    I=Ve/ (R1+R2)*1000;
    Serial.println(R2); 
    alectura = lectura;
  }

  rcuarto = digitalRead(cuarto);
  rbaja   = digitalRead(baja);
  ralta   = digitalRead(alta);
  rstop   = digitalRead(stop);
  rstand  = digitalRead(stand);
  rneutro = digitalRead(neutro);
  rizq    = digitalRead(izq);
  rder    = digitalRead(der);

  if(acuarto!=rcuarto){
    Serial.println("C");
    acuarto = rcuarto;
  }
  if(abaja!=rbaja){
    Serial.println("B");
    abaja = rbaja;
  }
  if(aalta!=ralta){
    Serial.println("A");
    aalta = ralta;
  }
  if(astop!=rstop){
    Serial.println("Stop");
    astop = rstop;
  }
  if(astand!=rstand){
    Serial.println("S");
    astand = rstand;
  }
  if(aneutro!=rneutro){
    Serial.println("N");
    aneutro = rneutro;
  }
  if(aizq!=rizq){
    Serial.println("I");
    aizq = rizq;
  }
  if(ader!=rder){
    Serial.println("D");
    ader = rder;
  }
}

void fizq () {
  Serial.println("IINT1");
} 

void fder () {
  Serial.println("INT2");
}