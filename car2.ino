#include <mygsm.h>


#include <myGPS.h>
#include <myTTS.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal(rs, enable, d4, d5, d6, d7)                                                   
LiquidCrystal lcd(A5, A6, A7, A8, A9, A10);



const int vpin=1;
const int E1=6;
const int I1=5;
const int I2=4;

const int U0Trig=9;
const int U0Echo=10;

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

const String alertNumber="+918072797819";
const String alertMessage="Alert ";
const int obstacleDistance=20;
const int SlowdDistance=15;
const int accidentDistance=10;
const int gsmRX=68;// gsm of tx to rx of ard (A14)
const int gsmTX=69;// gsm of rx to tx of ard (A15)
const int gpsRX=12;// gps of max232 rx to rx of ard
const int gpsTX=11;//gps of max232 tx to tx of ard
const int T2SRX=65;//t2s of max232 rx to rx of ard (A11)
const int T2STX=66;//t2s of max232 tx to tx of ard (A12)



#define STOP 0
#define START 1 //full speed
#define SLOW1 2
#define SLOW2 3



//INPUT PARAMETERS FOR DECODERS
//CAR2 front photo detctor
const int frontPDd1=A0;
const int frontPDd2=A1;
const int frontPDd3=A2;
const int frontPDd4=A3;

//CAR2 top photo detector
const int topPDd1=31;
const int topPDd2=33;
const int topPDd3=35;
const int topPDd4=37;

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
|       2 | slow down   |
+---------+-------------+

|     255 | full motor speed (start motor)
|     127   half motor speed (slowdown)

*/
{


   int speedNum;

   if ( control == 0 ) { speedNum = 0; }
   if ( control == 1 ) { speedNum = 255; }
   if ( control == 2 ) { speedNum = 100; } //SLOW1
   if ( control == 3 ) { speedNum = 20; } //SLOW2
   analogWrite(E1 ,speedNum);
   digitalWrite(I1 , LOW);
   digitalWrite(I2 , HIGH);
  
}

String getGPSlocation()
{
  String ret="54.823 99.231";
  Serial.println("returning GPS location " + ret);
  myGPS oGps(gpsRX,gpsTX);
  //oGps = new myGPS(gpsRX,gpsTX);
  ret=oGps.readLoc();
   
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

  
   pinMode(E1,OUTPUT);
   pinMode(I1,OUTPUT);
   pinMode(I2,OUTPUT);
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

     
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
     int d0=0,d1=0,d2=0,d3=0;
     String lcdData,tmpStr;
     int motorControlValue=1;


lcdOutput("CAR2:Hi     ",2);


  if ( digitalRead(switchPin) == HIGH ) 
  {


      //Lets run the car
      Serial.println("starting CAR2");
      controlMotor(START);

      digitalWrite(U0Trig,HIGH);
      delayMicroseconds(1000);
      digitalWrite(U0Trig,LOW);
while(true)
{
      int duration = pulseIn(U0Echo,HIGH);
      int distance = abs((duration/2)/29.1);
      tmpStr = String(distance);
      Serial.println("Distance " + tmpStr);
      lcdOutput("DIST-U"+tmpStr,2);
      delay(500);
      if( ( distance < obstacleDistance) && ( distance >= accidentDistance))
      {
        //We are very close to accident situation
        lcdOutput("Danger !!!! ",1);
        Serial.println("OBSTACLE < DISTANCE > ACCIDENT");
        controlMotor(SLOW1);//slow down
        //delay(10000); DEBUG
            
      }

   
        if ( distance < accidentDistance)
        {
               //move these to the top level after things are working.
               myGsm gsmmod;
               gsmmod.gsmstart(gsmRX,gsmTX); 
               Serial.println("SENDING ALERT TO OWNER ");
               gsmmod.sendsms(alertNumber,"POSSIBLE ACCIDENT");

               controlMotor(SLOW2);//slowdown again
               delay(5000);
               controlMotor(STOP); //stop after 5 seconds

               delay(12000);//2 minutes to press the button to notify safety (12 seconds in prototype)
               int latch=digitalRead(latchData);
               if ( latch == 0 ) 
               {
                  String location=getGPSlocation();
                  gsmmod.sendsms(alertNumber,"ACCIENT OCCURED AT " + location );
                  Serial.println("SENT ACCIDENT OCCURED ");
               }
               else
               {
                digitalWrite(latchReset,HIGH); //reset latch
                Serial.println("ACCIDENT AVERTED");
               }
        }

        
        
     

      //String frontDEC=digitalStringRead(frontPDd1,frontPDd2,frontPDd3,frontPDd4);
      String topDEC=digitalStringRead(topPDd1,topPDd2,topPDd3,topPDd4);
      
      if ( ( topDEC[0]=='0' ) && ( topDEC[1]=='0' ) ) 
      {
        //slow down
        //TTS slow down
        controlMotor(SLOW1);//slowdown again
        //stepper motor slow down
        //send back led slow down
        delay(5000);
        controlMotor(STOP);
        delay(20000);
      }

      if ( distance > obstacleDistance ) { break; }
}//while   
  }
  else
  {
   //read front decoder and take motor action
   String frontDEC=digitalStringRead(frontPDd1,frontPDd2,frontPDd3,frontPDd4);
   //SLOW DOWN IS 0011 from front car
   if (( frontDEC[0] == '0' ) && ( frontDEC[1] == '0') && ( frontDEC[2]=='1' ) && (frontDEC[3] == '1' ) )
   {
    controlMotor(SLOW1);//slowdown
    delay(5000);
    controlMotor(STOP);
    
   }
   //GO FROM front car is 1001
if (( frontDEC[0] == '1' ) && ( frontDEC[1] == '0') && ( frontDEC[2]=='0' ) && (frontDEC[3] == '1' ) )
   {
    delay(3000);
    controlMotor(START);//slowdown
       
   }
   
  }
  
}//loop


