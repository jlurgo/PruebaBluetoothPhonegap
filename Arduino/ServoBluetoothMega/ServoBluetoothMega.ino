#include <SoftwareSerial.h>
#include <Servo.h> 
#include <aJSON.h>
#include <MemoryFree.h>

#define RxD_bt 10
#define TxD_bt 11
#define RxD_ra 12
#define TxD_ra 13
#define RST 5 // Encendido del Modulo
#define KEY 4

#define CANT_PUERTOS 3
//SoftwareSerial BTSerial(RxD_bt, TxD_bt);
//SoftwareSerial red_arduino(RxD_ra, TxD_ra);

Servo servo_1; 

int ultimo_valor_pote = 0;
String caracteres_recibidos[CANT_PUERTOS] = "";
Stream* puertos[] = {&Serial, &Serial2, &Serial3};

void setup()
{  
  servo_1.attach(9);  

  Serial2.begin(9600);  
  Serial3.begin(9600);  
  Serial.begin(9600);
  
  servo_1.write(176);
  delay(500);  
  servo_1.write(0);
  
  for(int i=0;i<CANT_PUERTOS;i++){
    puertos[i]->flush();
    puertos[i]->println("Ahi va!");
  }    
  Serial.print(F("freeMemory="));
  Serial.println(freeMemory());
}

void loop()
{
  for(int i=0;i<CANT_PUERTOS;i++){
    if(puertos[i]->available()){
      char caracter = puertos[i]->read();
      OnCaracterRecibido(caracter, i);
    }
  }  
  
  int val_pote = analogRead(0);   
  val_pote = map(val_pote, 0, 1023, 0, 179); 
  if(ultimo_valor_pote > val_pote + 9 || ultimo_valor_pote < val_pote - 9){
    aJsonObject *msg;
    msg=aJson.createObject();  
    aJson.addStringToObject(msg,"tipoDeMensaje", "control_servo");
    aJson.addNumberToObject(msg,"id_servo", 1);
    aJson.addNumberToObject(msg,"angulo", val_pote); 
    ultimo_valor_pote = val_pote;  
    OnMensajeRecibido(msg, -1);
  }
}

void EnviarCharATodosLosPuertosMenosA(char caracter, int id_puerto){
    for(int i=0;i<CANT_PUERTOS;i++){
      if(i!=id_puerto)puertos[i]->write(caracter);
    } 
}

void EnviarMensajeATodosLosPuertosMenosA(String mensaje, int id_puerto){
    for(int i=0;i<CANT_PUERTOS;i++){
      if(i!=id_puerto)puertos[i]->print(mensaje);
    } 
}

void OnCaracterRecibido(char caracter, int id_puerto){  
    if (caracter != '\r' && caracter != '\n') {
      caracteres_recibidos[id_puerto] += caracter;
    }
    else { //caracter == '\r'     
        aJsonObject* mensaje = aJson.parse(&caracteres_recibidos[id_puerto][0]);
        OnMensajeRecibido(mensaje, id_puerto);      
        caracteres_recibidos[id_puerto] = "";   
    }  
}

void OnMensajeRecibido(aJsonObject* mensaje, int id_puerto){  
      if (mensaje == NULL) {
        Serial.print(F("Error al parsear mensaje")); 
        return;
      }  
      
      char* mensaje_str = aJson.print(mensaje);
      strcat(mensaje_str, "\n");
      EnviarMensajeATodosLosPuertosMenosA(mensaje_str, id_puerto);
      free(mensaje_str);
      
      aJsonObject* json_id_servo = aJson.getObjectItem(mensaje , "id_servo");
      int id_servo = json_id_servo->valueint;
      if(id_servo != 0) {
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
          
      Serial.print(F("freeMemory="));
      Serial.println(freeMemory());
}
