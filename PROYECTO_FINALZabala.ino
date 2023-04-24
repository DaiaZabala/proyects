/* Proyecto final arduino 2023. Zabala Daiana

  OBJETIVO: Realizar una lampara portatil con un ventilador con conexion usb.
  Lampara: tira de Leds reutilizados.
  -se encendera con un control remoto y tambien tactil con el sensor ultrasonico.
  -Encender cuando se acerca la mano
  -prender la luz
  -apagar la luz


  ventilador: se realizará con un motor.
  -Encender ventilador
  -Apagar ventilador
  -3 velocidades de ventilador.

*/

#include <IRremote.h>

//DECLARO VARIABLES
const int led1 = 2; //LED
const int disparo = 4;
const int echo = 3;
const int BIA = 9;
const int BIB = 10;
int ReceptorIR = 5;

long tiempoPulso;
float distancia;

IRrecv receptorIr(ReceptorIR);
decode_results codigoLeido;
decode_results codigoLeidoNuevo;


void setup() {
  Serial.begin(9600);
  receptorIr.enableIRIn();

  pinMode(led1, OUTPUT);//INICIALIZO LOS PINES DE SALIDA
  pinMode(disparo, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(disparo, LOW);
  analogWrite(BIA, 0);
  analogWrite(BIB, 0);
}

void loop() {
  if (receptorIr.decode(&codigoLeidoNuevo))
  {
    receptorIr.resume();
  }

  medirDistancia();
  encenderLuz();


  if (codigoLeidoNuevo.value != codigoLeido.value &&
      codigoLeidoNuevo.value != 0xFFFFFFFF) {

    Serial.println(codigoLeido.value, HEX);
    codigoLeido = codigoLeidoNuevo;
    switch (codigoLeido.value) {
      
      //boton 0 apagar ventilador
      case 0xFF6897:
        Serial.print("Ventilador apagado");
        frenar();
        break;
      
      //BOTON 1
      case 0xFF30CF:
        Velocidad1();
        break;
      
      //boton 2
      case 0xFF18E7:
        Velocidad2();
        break;
     
      //boton 3
      case 0xFF7A85:
        Velocidad3();
        break;
      
      //boton - apagar luz
      case 0xFFE01F:
        Serial.print("Luz apagada");
        digitalWrite(led1, LOW);
        break;

      //boton + encender luz
      case 0xFFA857:
        Serial.print("Luz encendida");
        digitalWrite(led1, HIGH);
        break;
    }

  }
}


void medirDistancia() {
  digitalWrite(disparo, HIGH);
  delay(500);
  digitalWrite(disparo, LOW);
  tiempoPulso = pulseIn(echo, HIGH);
  distancia =  0.0340 / 2 * tiempoPulso;

  Serial.print(tiempoPulso);
  Serial.print(" / ");
  Serial.println(distancia);
}



void encenderLuz() {
  //SI LA DISTANCIA ES MENOR A 5 LO ENCIENDO LO APAGO CON EL CONTROL.
  if (distancia < 5) {
    digitalWrite(led1, HIGH);
  }
}


void Velocidad1() {
  Serial.println("Velocidad 1");
  analogWrite(BIA, 120);
  analogWrite(BIB, 0);
}

void Velocidad2() {
  Serial.println("Velocidad 2");
  analogWrite(BIA, 170);
  analogWrite(BIB, 0);
}

void Velocidad3() {
  Serial.println("Velocidad 3");
  analogWrite(BIA, 255);
  analogWrite(BIB, 0);
}


void frenar() {
  Serial.println("Frenar");
  analogWrite(BIA, 255);
  analogWrite(BIB, 255);
  delay(500);
}
