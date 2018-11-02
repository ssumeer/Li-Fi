// Author sumeer@unixindia.com
#include <mygsm.h>

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal(rs, enable, d4, d5, d6, d7)                                                   
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

const int d0pin=10;
const int d1pin=10;
const int d2pin=10;
const int d3pin=10;

const int vpin=1;
const int motorControlPin=1;

const int U0Trig=12;
const int U0Echo=13;

const String lcdStrSignalSTOP="SIGNAL STOP";
const String lcdStrSignalGO="SIGNAL GO  ";
const String lcdStrEmerSTOP="EMERG STOP";
const String lcdStrSLOWDOWN="SLOW DOWN ";
const String lcdStrEmerGO="               ";
const String strSTOP="01";
const String strGO="10";
const String strSLOW="11";
const String delay10="00";
const String delay20="01";
const String delay30="10";
const String delay40="11";

const String alertNumber="+919444961083";
const String alertMessage="Alert ";
const int obstacleDistance=20;
const int SlowdDistance=15;
const int accidentDistance=10;
const int gsmRX=2;
const int gsmTX=3;

myGsm gsmmod;


//INPUT PARAMETERS FOR DECODERS
//CAR2 front photo detctor
const int frontPDd1=10;
const int frontPDd2=10;
const int frontPDd3=10;
const int frontPDd4=10;

//CAR2 top photo detector
const int topPDd1=11;
const int topPDd2=11;
const int topPDd3=11;
const int topPDd4=11;

//CAR2 case 2/3 switch pin
const int switchPin=13;
const int latchData=14;
const int latchReset=14;


void lcdOutput(String data,int line)
{
  
  line--;  
  lcd.setCursor(0,line);
  lcd.print(data);
  
}

void controlMotor(int control)
/*
+---------+-------------+
| control |    Notes    |
+---------+-------------+
|       0 | Stop motor  |
|       1 | Start motor |
+---------+-------------+
*/
{

   digitalWrite(motorControlPin,control);
  

}

String getGPSlocation()
{
  String ret="54.823 99.231";
  return ret;
}

String digitalStringRead(int pin1,int pin2, int pin3, int pin4)
{
  String ret;
  int d1=0,d2=0,d3=0,d4=0;
  d1=digitalRead(pin1);
  d2=digitalRead(pin2);
  d3=digitalRead(pin3);
  d4=digitalRead(pin4);

  ret=String("0"+d1)+String("0"+d2)+String("0"+d3)+String("0"+d4);
  
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.

   pinMode(d0pin,INPUT);
   pinMode(d1pin,INPUT);
   pinMode(d2pin,INPUT);
   pinMode(d3pin,INPUT);
   pinMode(motorControlPin,OUTPUT);

   pinMode(A0,OUTPUT);
   pinMode(A1,OUTPUT);
   pinMode(A2,OUTPUT);
   pinMode(A3,OUTPUT);
   pinMode(A4,OUTPUT);
   pinMode(A5,OUTPUT);
   
   pinMode(U0Trig,OUTPUT);
   pinMode(U0Echo,INPUT);

   pinMode(frontPDd1,INPUT);
   pinMode(frontPDd2,INPUT);
   pinMode(frontPDd3,INPUT);
   pinMode(frontPDd4,INPUT);

   pinMode(topPDd1,INPUT);
   pinMode(topPDd2,INPUT);
   pinMode(topPDd3,INPUT);
   pinMode(topPDd4,INPUT);
   
   //pinMode(gsmRX,INPUT);
   //pinMode(gsmTX,OUTPUT);
   
   
   Serial.begin(9600);

   gsmmod.gsmstart(gsmRX,gsmTX);   
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
     int d0=0,d1=0,d2=0,d3=0;
     String lcdData,tmpStr;
     int motorControlValue=1;


lcdOutput("CAR2:Hi Sumeer",2);


  if ( digitalRead(switchPin) == HIGH ) 
  {


      digitalWrite(U0Trig,HIGH);
      delayMicroseconds(1000);
      digitalWrite(U0Trig,LOW);

      int duration = pulseIn(U0Echo,HIGH);
      int distance = abs((duration/2)/29.1);
      tmpStr = String(distance);
      Serial.println("Distance " + tmpStr);
      delay(500);
      if( ( distance < obstacleDistance) && ( distance > accidentDistance))
      {
        //We are very close to accident situation
        lcdOutput("Danger !!!! ",1);
            
      }

   
        if ( distance < accidentDistance)
        {
               gsmmod.sendsms(alertNumber,"POSSIBLE ACCIDENT");
               delay(6000);
               int latch=digitalRead(latchData);
               if ( latch == 0 ) 
               {
                  String location=getGPSlocation();
                  gsmmod.sendsms(alertNumber,"ACCIENT OCCURED AT " + location );
               }
               else
               {
                digitalWrite(latchReset,HIGH); //reset latch
               }
        }

        
        
     

      //String frontDEC=digitalStringRead(frontPDd1,frontPDd2,frontPDd3,frontPDd4);
      String topDEC=digitalStringRead(topPDd1,topPDd2,topPDd3,topPDd4);
      
      if ( ( topDEC[0]=="0" ) && ( topDEC[1]=="0" ) ) 
      {
        //slow down
        //TTS slow down
        //stepper motor slow down
        //send back led slow down
      }
    
  }
  else
  {
    
  }
  
}//loop


