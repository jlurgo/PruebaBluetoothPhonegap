#include <SoftwareSerial.h>
#include <Servo.h> 
#include <aJSON.h>
#include <MemoryFree.h>

#define RxD_ra 12
#define TxD_ra 13
#define RST 5
#define KEY 4

#define CANT_PUERTOS 2
SoftwareSerial red_arduino(RxD_ra, TxD_ra);

Servo servo_1;  

char caracteres_recibidos[256];
int proximo_caracter;
Stream* puertos[] = {&Serial, &red_arduino};

void setup()
{  
  servo_1.attach(10);  
  
  red_arduino.begin(9600);  
  Serial.begin(9600);
  
  servo_1.write(176);
  delay(500);  
  servo_1.write(0);
  
  for(int i=0;i<CANT_PUERTOS;i++){
    puertos[i]->flush();
    puertos[i]->println("Ahi va!");
  }  
  
  proximo_caracter = 0;
  caracteres_recibidos[0] = '\0'; 
  
  Serial.print(F("freeMemory="));
  Serial.println(freeMemory());  
}

void loop()
{
  for(int i=0;i<CANT_PUERTOS;i++){
    if(puertos[i]->available()){
      char caracter = puertos[i]->read();
      EnviarCharATodosLosPuertosMenosA(caracter, i);
      OnCaracterRecibido(caracter);
    }
  }  
}

void EnviarMensajeATodosLosPuertosMenosA(char* mensaje, int id_puerto){
    for(int i=0;i<CANT_PUERTOS;i++){
      if(i!=id_puerto)puertos[i]->print(mensaje);
    } 
}

void EnviarCharATodosLosPuertosMenosA(char caracter, int id_puerto){
    for(int i=0;i<CANT_PUERTOS;i++){
      if(i!=id_puerto)puertos[i]->write(caracter);
    } 
}

void OnCaracterRecibido(char caracter){  
    if (caracter != '\r' && caracter != '\n') {
      //caracteres_recibidos += caracter;
      caracteres_recibidos[proximo_caracter] = caracter;
      proximo_caracter++;
      if(proximo_caracter>255){
        proximo_caracter = 0;
        caracteres_recibidos[0]='/0';   
      }
    }
    else { //caracter == '\r' 
      caracteres_recibidos[proximo_caracter] = '\0';   
      aJsonObject* mensaje = aJson.parse(caracteres_recibidos);
      proximo_caracter = 0;
      caracteres_recibidos[0]='/0';  
      
      if (mensaje == NULL) {
        Serial.print(F("Error al parsear mensaje")); 
        return;
      }     
      
      aJsonObject* json_id_servo = aJson.getObjectItem(mensaje , "id_servo");
      int id_servo = json_id_servo->valueint;
      if(id_servo != 1) {
        aJson.deleteItem(mensaje); 
        return;
      }
      aJsonObject* json_angulo_servo = aJson.getObjectItem(mensaje , "angulo");
      if (json_angulo_servo == NULL) {
        Serial.print(F("Error al parsear angulo")); 
        aJson.deleteItem(mensaje); 
        return;
      }  
      
      int angulo_servo = json_angulo_servo->valueint;          
      if(angulo_servo>=0 && angulo_servo<=177){
         servo_1.write(angulo_servo);
      }
      
      aJson.deleteItem(mensaje); 
      
      //Serial.print(F("freeMemory="));
      //Serial.println(freeMemory());
    }  
}
