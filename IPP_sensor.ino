#include <Wire.h>
#include <Adafruit_APD9960.h>
#include <softwareSerial.h>

Adafruit_ADP9960 apds;
SoftwareSerial bluetooth (2, 3); //RX, TX

int sensorPin =A0; //LDR till en analog pin 
int ledPin = 9; //PWM LED pin
int lagra_ljusvarde = 0; // lagrar ljusvärde
int troskel= 500; // tröskelvärde för att tända lampa
int ljusstyrka = 0; //LED ljusstyrka
int fargtemperatur = 100; //Färgtemperatur (0-255)
int rodPIN = 10; //röd kanal för RGB LED
int gronPIN = 11; //grön kanal för RGB LED
int blaPIN = 12; //blå kanal för RGB LED

bool autoMode = true; //automatisk ljusstyrka

unsigned long timer = 0; //tid för timer
unsigned long langdTimer = 0; //timer för att tända/släcka lampan

void setup() 
{
  pinMode (ledPin, OUTPUT);
  pinMode (rodPin, OUTPUT);
  pinMode (gronPin, OUTPUT);
  pinMode (blaPin, OUTPUT);
  Serial.begin(9600); //USB serial för debugging
  bluetooth.begin(9600); //Bluetooth-kommunikation

  if(!apds.begin())
  {
    Serial.println("APDS-9960 init misslyckades");
    while (1);
  }
  Serial.println("Ljussensor initierad");

void loop() 
{
  //Läs ljusvärde
  sensorValue = analogRead(sensorPin);
  ljusstyrka = map(sensorValue, 0, 1023, 0, 255);

  //hantera automatiosk ljusstyrning (om autoMode är aktiverat)
  if(autoMode)
  {
    if(sensorValue < troskel)
    {
      analogWrite(rodPin, ljusstyrka); //tänd lampan med justerad ljusstyrka
      analogWrite(gronPin, ljusstyrka); // grön kanal
      analogWrite(blaPin, ljusstyrka); //blå kanal
    }
    else
    {
      analogWrite(rodPin, 0); //släck lampan
      analogWrite(gronPin, 0); //släck lampan
      analogWrite(blaPin, 0); //släck lampan
    }
  }

  //timer-funktion (tänd/släck vid förutbestämd tid)
  if(millisek() - timer >= langdTimer)
  {
    analogWrite(redPin, 0);
    analogWrite(gronPin, 0);
    analogWrite(blaPin, 0);
  }

  //Bluetooth-kommunikation för att styra lampa
  if(bluetooth.available())
  {
    char c = bluetooth.read();
    switch (c)
    {
      case 'A':
      {
        autoMode = !autoMode; //väla mellan auto och manuellt läge
        break;
      }
      case 'B':
      {
        fargtemperatur = constrain(fargtemperatur + 10, 0, 255); //öka färgtemperatur
        break;
      }
      case 'C':
      {
        fargtemperatur = constrain(fargtemperatur - 10, 0, 255); //minska färgtemperatur
        break;
      }
      case 'T':
      {
        langdTimer =60000; //exempel på en tier på 1 minut
        timer = millisek(); //starta timer
        break;
      }
      case 'S':
      {
        ljusstyrka = constrain(ljusstyrka + 10, 0, 255); //öka ljusstyrka
        break;
      }
      case 'D':
      {
        ljusstyrka = constrain(ljusstyrka - 10, 0, 255); //minska ljusstyrka
        break;
      }
    }
  }

  // Skicka status till appen
  Serial.print("Ljus: ");
  Serial.print(lightValue);
  Serial.print(", ljusstyrka: ");
  Serial.print(brightness);
  Serial.print(", temp: ");
  Serial.pointln(colorBalance);

  delay(200);
}
