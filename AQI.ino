#include <LiquidCrystal_I2C.h>                         //header file for the I2CLCD
LiquidCrystal_I2C lcd(0x27, 16,2);
#include <MQ135.h>                                     //header file for the MQ135
#include <SoftwareSerial.h>                            //header file for the GSM Module
SoftwareSerial mySerial(6,7);                          //Tx, Rx

int buzzer = 10;                                       //buzzer connected to pin 10
int ledR = 9;                                          //red led connected to pin 9
int ledG = 8;                                          //green led connected to pin 8

const int aqsensor = A0;                               //output of mq135 connected to A0 pin of Arduino
int threshold = 100;                                   //threshold level for Air Quality

void setup() 
{
  pinMode (buzzer,OUTPUT);                             //buzzer is connected as Output from Arduino
  pinMode (ledR,OUTPUT);                               //red led is connected as output from Arduino
  pinMode (ledG,OUTPUT);                               //green led is connected as output from Arduino
  pinMode (aqsensor,INPUT);                            //MQ135 is connected as INPUT to arduino
  
  mySerial.begin(9600);                                //GSM
  Serial.begin (9600);                                 //begin serial communication with baud rate of 9600

  lcd.clear();                                         //clear lcd
  lcd.begin();                                         //consider 16,2 lcd
}

void loop()
{
  int ppm = analogRead(aqsensor);                      //read MQ135 analog outputs at A0 and store it in ppm

  Serial.print("Air Quality: ");                       //print message in serail monitor
  Serial.println(ppm);                                 //print value of ppm in serial monitor

  lcd.setCursor(0,0);                                  //set cursor of lcd to 1st row and 1st column
  lcd.print("Air Quality: ");                          //print message on lcd
  lcd.print(ppm);                                      //print value of MQ135

  if (ppm > threshold)                                 //check wheather ppm is greater than threshold or not
    {
      lcd.setCursor(0,1);                              //jump here if ppm is greater than threshold
      lcd.print("AQ Level HIGH");
      Serial.println("AQ Level HIGH");     
      tone(ledR,1000,200);                             //blink red led with turn on time 1000mS, turn off time 200mS
      digitalWrite(buzzer,HIGH);                       //turn ON Buzzer
      SendTextMessage();
    }
  else
    {
      digitalWrite(ledR,LOW);                          //jump here if ppm is not greater than threshold and turn off LED
      digitalWrite(buzzer,LOW);                        //turn OFF Buzzer
      tone(ledG,1000,200);                             //blink green led with turn on time 1000mS, turn off time 200mS
      lcd.setCursor(0,1);
      lcd.print ("AQ Level Good");
      Serial.println("AQ Level Good");
    }  
  delay (1000);
}

void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");                      // text mode on
  delay(1000);                      
  mySerial.println("AT+CMGS=\"+917029161430\"\r");    //mobile munber to which sms will be sent
  delay(1000);
  mySerial.println("AQI Level at Pailan is High. Stay inside the room");
  delay(1000);
  mySerial.println((char)26);
  delay(1000);
}
