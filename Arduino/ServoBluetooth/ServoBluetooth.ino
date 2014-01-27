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

//SoftwareSerial BTSerial(RxD_bt, TxD_bt);
//SoftwareSerial red_arduino(RxD_ra, TxD_ra);

Servo servo_1;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

 
int pos_servo_1 = 0;    // variable to store the servo position 
int ultimo_valor_pote = 0;
String caracteres_recibidos = "";


void setup()
{  
  servo_1.attach(9);  

  Serial2.begin(9600);
  Serial2.flush();
  
  Serial3.begin(9600);
  Serial3.flush();
  
  Serial.begin(9600);
  Serial.println("Ready");
  
  delay(500);
  
  servo_1.write(pos_servo_1);
}

void loop()
{
  if(Serial.available()){
    char caracter = Serial.read();
    Serial2.write(caracter); 
    Serial3.write(caracter); 
    OnCaracterRecibido(caracter);
  }
  if (Serial2.available()){
    char caracter = Serial2.read();
    Serial.write(caracter); 
    Serial3.write(caracter); 
    Serial2.flush();
    OnCaracterRecibido(caracter);
  }
  if (Serial3.available()){
    char caracter = Serial3.read();
    Serial.write(caracter); 
    Serial2.write(caracter); 
    Serial3.flush();
    OnCaracterRecibido(caracter);
  }
  int val_pote = analogRead(0);   
  val_pote = map(val_pote, 0, 1023, 0, 179); 
  if(ultimo_valor_pote > val_pote + 9 || ultimo_valor_pote < val_pote - 9){
    servo_1.write(val_pote); 
    String val_str = String(val_pote); 
    val_str  += '\n';
    Serial.print(val_str);
    delay(10);
    Serial2.print(val_str);
    delay(10);
    Serial3.print(val_str);
    delay(10);
    ultimo_valor_pote = val_pote;
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
