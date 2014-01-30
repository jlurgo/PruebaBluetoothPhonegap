#include <SoftwareSerial.h>
#include <Servo.h> 
#include <aJSON.h>

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
String caracteres_recibidos = "";
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
  
  int val_pote = analogRead(0);   
  val_pote = map(val_pote, 0, 1023, 0, 179); 
  if(ultimo_valor_pote > val_pote + 9 || ultimo_valor_pote < val_pote - 9){
    servo_1.write(val_pote); 
    String val_str = String(val_pote); 
    val_str  += '\n';
    EnviarMensajeATodosLosPuertosMenosA(val_str, -1);
    ultimo_valor_pote = val_pote;
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
