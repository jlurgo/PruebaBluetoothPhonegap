#include <SoftwareSerial.h>
#include <Servo.h> 
#include <aJSON.h>

#define RxD_ra 12
#define TxD_ra 13
#define RST 5
#define KEY 4

#define CANT_PUERTOS 2
SoftwareSerial red_arduino(RxD_ra, TxD_ra);

Servo servo_1;  

String caracteres_recibidos = "";
Stream* puertos[] = {&Serial, &red_arduino};

void setup()
{  
  servo_1.attach(9);  
  
  red_arduino.begin(9600);  
  Serial.begin(9600);
  
  servo_1.write(176);
  delay(500);  
  servo_1.write(0);
  
  for(int i=0;i<CANT_PUERTOS;i++){
    puertos[i]->flush();
    puertos[i]->println("Ahi va!");
  }  
}

void loop()
{
  for(int i=0;i<CANT_PUERTOS;i++){
    if(puertos[i]->available()){
      char caracter = puertos[i]->read();
      EnviarMensajeATodosLosPuertosMenosA(&caracter, i);
      OnCaracterRecibido(caracter);
    }
  }  
}

void EnviarMensajeATodosLosPuertosMenosA(String mensaje, int id_puerto){
    for(int i=0;i<CANT_PUERTOS;i++){
      if(i!=id_puerto)puertos[i]->print(mensaje);
    } 
}

void OnCaracterRecibido(char caracter){  
    if (caracter != '\r' && caracter != '\n') {
      caracteres_recibidos += caracter;
    }
    else { //caracter == '\r'     
      aJsonObject* mensaje = aJson.parse(&caracteres_recibidos[0]);
      if (mensaje != NULL) {
        aJsonObject* angulo_servo = aJson.getObjectItem(mensaje , "angulo");
        if (angulo_servo != NULL) {
          int pos_servo = angulo_servo->valueint;
          
          if(pos_servo>=0 && pos_servo<=177){
            servo_1.write(pos_servo);
          }
        }
      }
      caracteres_recibidos = "";   
    }  
}
