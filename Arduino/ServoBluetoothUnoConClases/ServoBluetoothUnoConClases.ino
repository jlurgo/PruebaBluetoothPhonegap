#include <SoftwareSerial.h>
#include <Servo.h> 
#include <aJSON.h>
#include <MemoryFree.h>
#include "Vortex.h"

Servo servo; 

#define RxD_ra 11
#define TxD_ra 12
SoftwareSerial red_arduino(RxD_ra, TxD_ra);

void setup()
{ 	
        Serial.begin(9600);
	red_arduino.begin(9600);  

        vx.Setup();	
        vx.AgregarPuerto(&red_arduino); 
        vx.AgregarPuerto(&Serial);    
        vx.PedirMensajes(aJson.parse("{\"tipo\":\"EQ\",\"clave\":\"id_servo\",\"valor\":1}"), RecibirMensajeDeControlDeServo);
        vx.PedirMensajes(aJson.parse("{\"tipo\":\"EQ\",\"clave\":\"id_servo\",\"valor\":0}"), RecibirMensajeDeControlDeLed);
        
        pinMode(13, HIGH);
	servo.attach(9);  
	servo.write(176);
	delay(500);  
	servo.write(0);
}

void loop()
{ 
	vx.Tick();	
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




