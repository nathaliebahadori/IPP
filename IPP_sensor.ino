#include <Wire.h>
#include <Adafruit_APDS9960.h>
#include <SoftwareSerial.h>

Adafruit_APDS9960 apds;
SoftwareSerial bluetooth (2, 3); //RX TX

int ledPin = 13; //PWM LED pin
int sensorValue = 0; 

int lagra_ljusvarde = 0; // lagrar ljusvärde
int troskel= 500; // tröskelvärde för att tända lampa
int ljusstyrka = 0; //LED ljusstyrka
int fargtemperatur = 100; //Färgtemperatur (0-255)

bool autoMode = true; //automatisk ljusstyrka
bool manualOverride= false; //ny flagga för manuell ljusstyrka

unsigned long timer = 0; //tid för timer
unsigned long langdTimer = 0; //timer för att tända/släcka lampan



void setup() 
{
  pinMode (ledPin, OUTPUT);

  Serial.begin(9600); //USB serial för debugging
  bluetooth.begin(9600); //Bluetooth-kommunikation

  if(!apds.begin())
  {
    Serial.println("APDS-9960 init misslyckades");
    while (1);
  }
  apds.enableColor(true);
}


void loop() 
{
  //Läs ljusvärde
  //ljusstyrka = map(sensorValue, 0, 1023, 0, 255);
  uint16_t r, g, b, c;
  apds.getColorData(&r, &g, &b, &c); 
  lagra_ljusvarde = c;

  Serial.print("Ljus: ");
  Serial.println(lagra_ljusvarde);
  

  //hantera automatiosk ljusstyrning (om autoMode är aktiverat)
  if(autoMode && !manualOverride) //Endast om autoMode är på och ongen manuell ändrig gjorts
  {
    ljusstyrka = map(c, 0, 65535, 0, 255);
    analogWrite(ledPin, ljusstyrka); //Justera ljusstyrka
  }

  //timer-funktion (tänd/släck vid förutbestämd tid)
  if(millis() - timer >= langdTimer && langdTimer > 0)
  {
    analogWrite(ledPin, 0); //släcker lampa när tiden är ute
    langdTimer = 0; //återställ timer
  }

  //Bluetooth-kommunikation för att styra lampa
  if(Serial.available()) //kom ihåg att ändra till bluetooth istället för serial vid tillgång till app
  {
    char c = Serial.read();//kom ihåg att ändra till bluetooth istället för serial vid tillgång till app
    if (c == '\n' || c == '\r') return;

    switch (c)
    {
      case 'A':
      {
        autoMode = !autoMode; //välja mellan auto och manuellt läge
        manualOverride = false; //återställ manuell överstyrning
        Serial.println(autoMode ? "Automatiskt läge" : "Manuellt läge");//kom ihåg att ändra till bluetooth istället för serial vid tillgång till app
        break;
      }
      case 'T':
      {
        langdTimer =60000; //exempel på en tier på 1 minut
        timer = millis(); //starta timer
        Serial.println("Timer på 1 minut aktiverad");//kom ihåg att ändra till bluetooth istället för serial vid tillgång till app
        break;
      }
      case 'S':
      {
        ljusstyrka = constrain(ljusstyrka + 10, 0, 255); //öka ljusstyrka
        manualOverride = true; //sätt flagga vid manuell ändring
        Serial.println("Ljustyrkan ökad till: ");//kom ihåg att ändra till bluetooth istället för serial vid tillgång till app
        Serial.println(ljusstyrka);//kom ihåg att ändra till bluetooth istället för serial vid tillgång till app
        analogWrite(ledPin, ljusstyrka);
        break;
      }
      case 'D':
      {
        ljusstyrka = constrain(ljusstyrka - 10, 0, 255); //minska ljusstyrka
        manualOverride = true; //sätt flagga vid manuell ändring
        Serial.println("Ljustyrkan minskad till: ");//kom ihåg att ändra till bluetooth istället för serial vid tillgång till app
        Serial.println(ljusstyrka);//kom ihåg att ändra till bluetooth istället för serial vid tillgång till app
        analogWrite(ledPin, ljusstyrka);
        break;
      }
      default:
      {
        Serial.println ("Okänt kommando"); //kom ihåg att ändra till bluetooth istället för serial vid tillgång till app
        break;
      }
    }
  }

  // Skicka status till appen
  Serial.print(" Ljus: ");
  Serial.print(lagra_ljusvarde);
  Serial.print(", ljusstyrka: ");
  Serial.print(ljusstyrka);

  delay(1000);
}
