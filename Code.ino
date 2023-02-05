#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);
int calibrationTime = 10;
long unsigned int lowIn;
long unsigned int pause = 5000;
boolean takeLowTime;
boolean lockLow = true;
int pirsensor = 5;
int smokeA0 = A5;
int sensorThres = 1100;
String textForSMS;

void setup()
{
  randomSeed(analogRead(0));
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("Logging Time Completed!");
  pinMode(pirsensor, INPUT);
  pinMode(smokeA0, INPUT);
  digitalWrite(pirsensor, LOW);
  Serial.print("Calibrating Sensors ");
  for(int i = 0; i < calibrationTime; i++)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nDone");
  Serial.println("SENSORS ACTIVE");
  delay(50);
}
void loop()
{
 if ( digitalRead(pirsensor) == HIGH)
  { 
    textForSMS =  "Motion Triggered";
    sendSMS(textForSMS);
    Serial.println(textForSMS);
    delay(1000);
    Serial.println("Message sent.");
    delay(9000);
  }
  takeLowTime = true;
  if(digitalRead(pirsensor) == LOW) 
  {
    if(takeLowTime)
    {
       lowIn = millis(); //save the time of the transition from high to LOW
       takeLowTime = false;
    }
    //if the sensor is low for more than the given pause,we assume that no more motion is going to happen
    if(!lockLow && millis() - lowIn > pause)
    {
      //makes sure this block of code is only executed again after a new motion sequence has been detected
      lockLow = true;
    }
  }
  int analogSensor = analogRead(smokeA0);
  if (analogSensor > sensorThres) // Checks if it has reached the threshold value
  {
    textForSMS =  "Smoke Detected!";
    sendSMS(textForSMS);
    Serial.println(textForSMS);
    delay(1000);
    Serial.println("Message sent.");
    delay(5000);
  }
  else
  {
    delay(1000);
  }
  delay(1000);
}
void sendSMS(String message)
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1 second
  mySerial.println("AT+CMGS=\"+918921644146\"\r");
  delay(1000);
  mySerial.println(message);// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
  delay(1000);
}
