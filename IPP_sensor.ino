int sensorPin =; //LDR till en analog pin 
int sensorValue = 0; // lagrar jusvärde
int threshold=; // tröskelvärde för "mörker"

int colorBalance = 100; //0-255, varmt till kallt ljus
bool autoMode = true // automatisk ljusstyrning

void setup() 
{
  // put your setup code here, to run once:
  pinMode (ledPin, OUTPUT);
  Serial.begin(9600); //Bluetooth använder samma port
}

void loop() 
{
  if (Serial.available())
    {
      char c = Serial.read();


    }

  // put your main code here, to run repeatedly:



  // Skicka status till appen
  Serial.print("Ljus: ");
  Serial.print(lightValue);
  Serial.print(", ljusstyrka: ");
  Serial.print(brightness);
  Serial.print(", temp: ");
  Serial.pointln(colorBalance);

  delay(200);
}
