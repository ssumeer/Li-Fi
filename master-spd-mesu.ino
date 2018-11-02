// Author sumeer@unixindia.com
#include <Wire.h>

int vccpin=11;
int firstechoPin = 13;
int firsttriggerPin = 12;
int secondechoPin = 3;
int secondtriggerPin = 2;


//Road specific variables

//Distance betwSerial.println("");    // print the charactereen two Usonic sensors

const int roadDistance=30; //in meters

//temporary variables


boolean isCarpresent(int triggerpin, int echopin)
{

  int duration=0;
  float time=0;
  float distance=0;

  while(1)
  {
    digitalWrite( triggerpin ,HIGH);
    delay(3000);
    //delayMicroseconds (30);
    digitalWrite(triggerpin, LOW);

    duration = pulseIn(echopin, HIGH);
    time = pulseIn(echopin, HIGH,1000);

    distance = (duration/2) / 29.1;                                                 

    Serial.println("distance "+String(int(distance)));
    if ( ( distance >1 ) && ( distance < 20 ) )
    { 
      return true; 
    }
    //delay(1000);
  }

}




void setup()

{ pinMode(vccpin,OUTPUT);
  pinMode ( firstechoPin, INPUT);
  pinMode ( firsttriggerPin, OUTPUT);
  pinMode ( secondechoPin, INPUT);
  pinMode ( secondtriggerPin, OUTPUT);
  Serial.begin (9600);
  Wire.begin();
  // speed = (seconddistance-firstdistance)/(firsttime-secondtime)
}


//====================== MAIN ==========================
void loop()
{ 


  unsigned long time1=0,time2=0;
  float spd=0;
  long duration=0;
 digitalWrite(vccpin,HIGH); 
  isCarpresent(firsttriggerPin,firstechoPin);

  time1=millis();
  Serial.println("Car  1 found ");
  isCarpresent(secondtriggerPin,secondechoPin);
  Serial.println("Car  2 found ");
  time2=millis();

  duration=(time2 - time1)/1000;
  
  Serial.println("Duration " + String(duration));
  spd=(roadDistance/duration);
  char *strSpd=(char*)malloc(20);
  dtostrf(spd,3,3,strSpd);
  Serial.print("Speed ");
  Serial.println(spd);
  Wire.beginTransmission(1);
  Wire.write(strSpd);
  Wire.endTransmission();
  
  delay (500);
  
 Wire.beginTransmission(2);
  Wire.write(strSpd);
  Wire.endTransmission();

  delay(500);
  
}
