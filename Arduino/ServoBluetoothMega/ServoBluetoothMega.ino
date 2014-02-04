#include <SoftwareSerial.h>
#include <Servo.h> 
#include <aJSON.h>
#include <MemoryFree.h>

#define CANT_PUERTOS 4
#define CANT_PEDIDOS 2

Servo servo; 

int ultimo_valor_pote = 0;

String caracteres_recibidos[CANT_PUERTOS] = "";

Stream* puertos[] = {&Serial, &Serial1 ,&Serial2, &Serial3};

struct pedido
{
    aJsonObject *filtro;
	void (*callback)(aJsonObject*);
} pedidos[CANT_PEDIDOS];

void setup()
{  
	Serial1.begin(9600);
	Serial2.begin(9600);  
	Serial3.begin(9600);  
	Serial.begin(9600);

	servo.attach(9);  
	servo.write(176);
	delay(500);  
	servo.write(0);

	
	pedidos[0].filtro=aJson.parse("{\"tipo\":\"EQ\",\"clave\":\"id_servo\",\"valor\":0}");
	pedidos[0].callback = RecibirMensajeDeControlDeServo;

    pedidos[1].filtro=aJson.parse("{\"tipo\":\"EQ\",\"clave\":\"id_servo\",\"valor\":1}");
	pedidos[1].callback = RecibirMensajeDeControlDeLed;
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
    aJson.addNumberToObject(msg,"id_servo", 0);
    aJson.addNumberToObject(msg,"angulo", val_pote); 
    ultimo_valor_pote = val_pote;  
    RecibirMensaje(msg, -1);
	aJson.deleteItem(msg);
  }
}

void RecibirMensajeDeControlDeServo(aJsonObject* mensaje){
	aJsonObject* obj_angulo_servo = aJson.getObjectItem(mensaje , "angulo");
	if (obj_angulo_servo == NULL) {
		Serial.print(F("Error al parsear angulo")); 
		return;
	}   
	int angulo_servo = obj_angulo_servo->valueint;      
	if(angulo_servo>=0 && angulo_servo<=177){
		servo.write(angulo_servo);
	}
}

void RecibirMensajeDeControlDeLed(aJsonObject* mensaje){
	aJsonObject* obj_angulo_servo = aJson.getObjectItem(mensaje , "angulo");
	if (obj_angulo_servo == NULL) {
		Serial.print(F("Error al parsear angulo")); 
		return;
	}   
	int angulo_servo = obj_angulo_servo->valueint;      
	if(angulo_servo>=90){
		digitalWrite(13, 1); 
	} else{
		digitalWrite(13, 0); 
	}
}

void EnviarCharATodosLosPuertosMenosA(char caracter, int id_puerto){
    for(int i=0;i<CANT_PUERTOS;i++){
      if(i!=id_puerto)puertos[i]->write(caracter);
    } 
}

void EnviarMensajeATodosLosPuertosMenosA(aJsonObject* mensaje, int id_puerto_excluido){
	char* mensaje_str = aJson.print(mensaje);
	strcat(mensaje_str, "\n");
    for(int i=0;i<CANT_PUERTOS;i++){
      if(i!=id_puerto_excluido)puertos[i]->print(mensaje_str);
    } 
	free(mensaje_str);
}

void OnCaracterRecibido(char caracter, int id_puerto){  
    if (caracter != '\r' && caracter != '\n') {
      caracteres_recibidos[id_puerto] += caracter;
    }
    else { //caracter == '\r' 
        aJsonObject* mensaje = aJson.parse(&caracteres_recibidos[id_puerto][0]);
		if (mensaje != NULL) {
			RecibirMensaje(mensaje, id_puerto); 
		} else{
			Serial.println(F("Error al parsear mensaje!")); 
		}		
        caracteres_recibidos[id_puerto] = "";  
		aJson.deleteItem(mensaje); 		
    }  
}

void RecibirMensaje(aJsonObject* mensaje, int id_puerto){   
	EnviarMensajeATodosLosPuertosMenosA(mensaje, id_puerto);

	for(int i=0; i<CANT_PEDIDOS; i++){
		if(ElMensajePasaElFiltro(mensaje, pedidos[i].filtro)){
			pedidos[i].callback(mensaje);
		}
	}
}

bool ElMensajePasaElFiltro(aJsonObject* mensaje, aJsonObject* filtro){
	if (filtro == NULL) {
		Serial.print(F("Error en el filtro")); 
		return false;
	}  
	aJsonObject* obj_tipo_filtro = aJson.getObjectItem(filtro , "tipo");
	String tipo_filtro = obj_tipo_filtro->valuestring;
	if(tipo_filtro=="EQ"){
		aJsonObject* obj_clave_filtro = aJson.getObjectItem(filtro , "clave");
		char* clave_filtro = obj_clave_filtro->valuestring;
		aJsonObject* obj_valor_filtro = aJson.getObjectItem(filtro , "valor");
	
		aJsonObject* obj_valor = aJson.getObjectItem(mensaje , clave_filtro);
		if(obj_valor->type==aJson_Int && obj_valor_filtro->type==aJson_Int){
			return(obj_valor->valueint == obj_valor_filtro->valueint);
		}	
	}
	return false;
}
