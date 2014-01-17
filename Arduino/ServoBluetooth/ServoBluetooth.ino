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

#define RxD_bt 10
#define TxD_bt 11
#define RxD_ra 12
#define TxD_ra 13
#define RST 5 // Encendido del Modulo
#define KEY 4

SoftwareSerial BTSerial(RxD_bt, TxD_bt);
SoftwareSerial red_arduino(RxD_ra, TxD_ra);

Servo servo_1;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

 
int pos_servo_1 = 0;    // variable to store the servo position
 
String caracteres_recibidos = "";

void setup()
{  
  servo_1.attach(9);  

  BTSerial.begin(9600);
  BTSerial.flush();
  delay(200);
  
  red_arduino.begin(9600);
  red_arduino.flush();
  delay(200);
  
  Serial.begin(9600);
  Serial.println("Ready");  
  delay(200);
  
  servo_1.write(pos_servo_1);
}

void loop()
{
  if(Serial.available()){
    char caracter = Serial.read();
    BTSerial.write(caracter); 
    red_arduino.write(caracter); 
    OnCaracterRecibido(caracter);
  }
  if (BTSerial.available()){
    char caracter = BTSerial.read();
    Serial.write(caracter); 
    red_arduino.write(caracter); 
    BTSerial.flush();
    OnCaracterRecibido(caracter);
  }
  if (red_arduino.available()){
    char caracter = red_arduino.read();
    Serial.write(caracter); 
    BTSerial.write(caracter); 
    red_arduino.flush();
    OnCaracterRecibido(caracter);
  }
}

void OnCaracterRecibido(char caracter){  
    if (caracter != '\r' && caracter != '\n') {
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
