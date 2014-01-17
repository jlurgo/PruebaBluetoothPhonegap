/*
 *  Author: Jose Antonio Luceño Castilla
 *  Date  : Septempber 2013
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <SoftwareSerial.h>
#include <Servo.h> 

#define RxD 10
#define TxD 11
#define RST 5 // Encendido del Modulo
#define KEY 4

SoftwareSerial BTSerial(RxD, TxD);

Servo servo_1;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

 
int pos_servo_1 = 0;    // variable to store the servo position 
 
String caracteres_recibidos = "";

void setup()
{  
  servo_1.attach(9);  // attaches the servo on pin 9 to the servo object 
  // Configuracion del puerto serie por software
  // para comunicar con el modulo HC-05
  BTSerial.begin(9600);
  BTSerial.flush();
  delay(500);
  
  servo_1.write(pos_servo_1);
  
  // Configuramos el puerto serie de Arduino para Debug
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop()
{
  if(Serial.available()){
    OnCaracterRecibido(Serial.read());
  }
  if (BTSerial.available()){
    OnCaracterRecibido(BTSerial.read());
    BTSerial.flush();
  }
}

void OnCaracterRecibido(char caracter){  
    if (caracter != '\r') {
      Serial.println("recibido:" + caracter); 
      caracteres_recibidos += caracter;
    }
    else { //caracter == '\r'   
      Serial.println("recibidos:" + caracteres_recibidos);  
      pos_servo_1 = caracteres_recibidos.toInt();
      if(pos_servo_1>=0 && pos_servo_1<=177){
        servo_1.write(pos_servo_1);
      }
      caracteres_recibidos = "";   
    }  
}
