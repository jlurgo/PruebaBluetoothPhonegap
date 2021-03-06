#include <SoftwareSerial.h>
#include <Servo.h> 
#include <aJSON.h>
#include <MemoryFree.h>
#include "Vortex.h"

Servo servo; 

int ultimo_valor_pote = 0;  

void setup()
{  	Serial1.begin(9600);  
	Serial3.begin(9600);  
	Serial.begin(9600);
        vx.Setup();	
        vx.AgregarPuerto(&Serial);
        vx.AgregarPuerto(&Serial1);
        vx.AgregarPuerto(&Serial3);       
        vx.PedirMensajes(aJson.parse("{\"tipo\":\"EQ\",\"clave\":\"id_servo\",\"valor\":0}"), RecibirMensajeDeControlDeServo);
        vx.PedirMensajes(aJson.parse("{\"tipo\":\"EQ\",\"clave\":\"id_servo\",\"valor\":1}"), RecibirMensajeDeControlDeLed);
        
        pinMode(13, HIGH);
	servo.attach(9);  
	servo.write(176);
	delay(500);  
	servo.write(0);
}

void loop()
{ 
	vx.Tick();
        LeerPote();	
}

void LeerPote(){
        int val_pote = analogRead(0);   
	val_pote = map(val_pote, 0, 1023, 0, 179); 
	if(ultimo_valor_pote > val_pote + 9 || ultimo_valor_pote < val_pote - 9){
		aJsonObject *msg;
		msg=aJson.createObject();  
		aJson.addStringToObject(msg,"tipoDeMensaje", "control_servo");
		aJson.addNumberToObject(msg,"id_servo", 0);
		aJson.addNumberToObject(msg,"angulo", val_pote); 
		ultimo_valor_pote = val_pote;  
		vx.RecibirMensaje(msg, -1);
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




