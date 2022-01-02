/*
*********************************************************************************************************************************************************************************************************************************************************************************************************
Project Name         : Safe Box with Magnetic Card Reader
Revision Date        : 22.12.2021
Author               : Yagiz Horasanli
*******************************************************************************************************************************************************************************************************************************************************************************************************
Purpose of Project   : A safe that can be opened with one or more magnetic cards.
********************************************************************************************************************************************************************************************************************************************************************************************************
System Work          : In the study, when the operation logic system is examined a RC522 RFID NFC sensor is used together with the microcontroller to open the safe door. First of all, the magnetic card is touched to the sensor.
Then, the RFID sensor reads the ID of the card and transmits to the microcontroller for decision.
Next, if the ID of the card has been previously introduced to the microcontroller, the green LED turns on and the microcontroller sends the necessary signal to the servo motor and the safe door opens. 
However, if the RFID sensor can not recognize the ID of the card, the red LED turns on and the safe door will not open.
*********************************************************************************************************************************************************************************************************************************************************************************************************
System Imput         :
                       1) RC522 RFID NFC MODULE : To detect magnetic card
System Output        :
                       1) BUZZER                : To alert with sound
                       2) LED                   : To indicate the system is active or not
                       3) SERVO MOTOR           : To open and close the system door
***********************************************************************************************************************************************************************************************************************************************************************************************************
*/


//**********************************************************************************************************************************************************************************************************************************************************************************************************
//DEFINITIONS

#include <SPI.h>﻿                                       
#include <MFRC522.h>                                    
#include <Servo.h>                                     
#define SS_PIN 10                                      // SS Pin of the RC522 connected to Digital 10 Pin.
#define RST_PIN 9                                      // RST Pin of the RC522 connected to Digital 9 Pin. 
#define LED_G 5                                        // Green LED is connected to Digital 5 Pin.
#define LED_R 4                                        // Red LED is connected to Digital 4 Pin.
#define BUZZER 2                                       // Buzzer is connected to Digital 2 Pin.
MFRC522 mfrc522(SS_PIN, RST_PIN);                      // Create MFRC522 instance.
Servo myServo;                                         // To define SERVO name.

//************************************************************************************************************************************************************************************************************************************************************************************************************

//SETUP CODE AREA

void setup()
{
  Serial.begin(9600);                                  // Sets the baud rate for serial data transmission.
  SPI.begin();                                         // Initiate  SPI bus.
  mfrc522.PCD_Init();                                  // Initiate PCD.
  myServo.attach(3);                                   // SERVO Pin.
  myServo.write(0);                                    // SERVO start position.
  pinMode(LED_G, OUTPUT);                              // Green LED Pin is defined as OUTPUT.
  pinMode(LED_R, OUTPUT);                              // Red LED Pin is defined as OUTPUT.
  pinMode(BUZZER, OUTPUT);                             // BUZZER is defined as OUTPUT.
  noTone(BUZZER);
  Serial.println("Read the card to the sensor");
}
//*************************************************************************************************************************************************************************************************************************************************************************************************************

//MAIN CODE AREA
void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
                                                       // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
                                                       // To show UID on serial monitor.
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "73 5D 48 07" )         // Authorized Card's ID.


                                                              



{
    Serial.println("DOOR OPENED!");
    Serial.println();
    delay(500);                                        
    digitalWrite(LED_G, HIGH);                         
    tone(BUZZER, 300);                                 // To set Buzzer tone.
    delay(300);                                        // To set the buzzer's playing time.
    noTone(BUZZER);                                     
    myServo.write(90);                                 // To adjust the rotation angle of the servo motor.
    delay(15000);                                       
    myServo.write(0);                                  // To set Servo Motor's ending position.
    digitalWrite(LED_G, LOW);                          
  }
 
 else   {
    Serial.println("INVALID CARD!");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 500);
    delay(1000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}
