
#include <LiquidCrystal.h>

// TRAFFIC LIGHT USING LIFI
// AUTHOR: Sumeer Subramanian (sumeer@unixindia.com)
// Meenakshi Engineering College 

//TOP LEVEL DEBUG CONTROL 

int debug=1;

//BOARD DEFINES FRONT: DECOEDER
int d0pin=A0;
int d1pin=A1;
int d2pin=A2;
int d3pin=A3;

//BOARD DEFINES BACK: ENCODER
int e0pin=A8;
int e1pin=A9;
int e2pin=A10;
int e3pin=A11;

const int te=A7;
const int vt=A5;

const int FUsonicTrigger=2;
const int FUsonicEcho=3;
const int BUsonicTrigger=4;
const int BUsonicEcho=5;                                                                                                                                                                
int motorControlPin=10;



const String strSTOP="01";
const String strGO="10";
const String delay10="00";
const String delay20="01";
const String delay30="10";
const String delay40="11";

//LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 6, 7, 8, 9);

//OUTPUT CONSTANTS

const String lcdStrSignalSTOP="SIGNAL STOP";
const String lcdStrSignalGO="SIGNAL GO  ";
const String lcdStrEmerSTOP="EMERG STOP";
const String lcdStrEmerGO="               ";

const int case2Distance=15; //cms for obstruction OR cms for back side car
const int usonicTriggerDelay=1000;
                  
//pin 4
// Low => 
const int inputType=13;//Photo detector vs Ultrasonic input choice.

int d0val=0;
int d1val=0;
int d2val=0;
int d3val=0;

//================== START OF CODE ========================
void setup ()
{
 
   pinMode (d0pin,INPUT);
   pinMode (d1pin,INPUT);
   pinMode (d2pin,INPUT);
   pinMode (d3pin,INPUT);
   pinMode(e0pin,OUTPUT);
   pinMode(e1pin,OUTPUT);
   pinMode(e2pin,OUTPUT);
   pinMode(e3pin,OUTPUT);
   pinMode(te,OUTPUT);
   pinMode(vt,INPUT);
   pinMode(FUsonicTrigger,OUTPUT);
   pinMode(FUsonicEcho,INPUT);
   pinMode(BUsonicTrigger,OUTPUT);
   pinMode(BUsonicEcho,INPUT);
   pinMode(motorControlPin,OUTPUT);
   lcd.begin(16, 2);
   Serial.begin(115200);
   
   pinMode(inputType,INPUT);
}

void debugPrint(String dat)
{
   if ( debug == 1 ) { Serial.println(dat); }
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

void lcdOutput(String data,int line)
{
  
  line--;  
  lcd.setCursor(0,line);
  lcd.print(data);
  
}


void sendSignal(String signal, String timer)
{
  
/*
SIGNAL DEFINITION REFERENCE:
+----+----+----+----+-------------------------------------+
| d0 | d1 | d2 | d3 |                INFO                 |
+----+----+----+----+-------------------------------------+
|  0 |  0 | X  | X  | NOT USED                            |
|  0 |  1 | X  | X  | STOP (d2,d3 defines the delay time) |
|  1 |  0 | X  | X  | GO (d2,d3 defines the delay time)   |
|  1 |  1 | X  | X  | SLOW                                |
+----+----+----+----+-------------------------------------+

in case of SLOW d0=1 d1=1 the next two bits define the below:

+----+----+----+-----+--------------------------------------------+
| d0 | d1 | d2 | d3  |                   notes                    |
+----+----+----+-----+--------------------------------------------+
|  1 |  1 |  0 |   1 | Transmission from Front Light of CAR       |
|  1 |  1 |  1 |   0 | Information from street light to SLOW DOWN |
+----+----+----+-----+--------------------------------------------+


*/
  
  int e0val=0, e1val=0,e2val=0,e3val=0;
  e0val=signal[0]-'0';
  e1val=signal[1]-'0';

  e2val=timer[0] - '0';
  e3val=timer[1] - '0';

  // The TE pin of encoder HT12E is active when LOW. We use a transistor to reverse this 
  // so that TE  becomes an active HIGH instead of active LOW.
  
  digitalWrite(te,LOW); //disable transmission of encoder
  digitalWrite(e0pin,e0val);
  digitalWrite(e1pin,e1val);
  digitalWrite(e2pin,e2val);
  digitalWrite(e3pin,e3val);
  digitalWrite(te,HIGH); // enable encoder transmission
    
}


void loop ()
{

 //Start reading the input
 
 //We first decide if this is case 1 (photo detector) or case 2 (ultrasonic sensor)

  int tmpInt=0;
  String tmpStr;
  String strDistance;
  
  //BELOW are the variables to be populated for data cascading
  String lcdData;//To be printed in LCD
  int motorControlValue=1; //By default motor is running
  String cascadeSignal;
  String cascadeSignalDelay,cmdTimer;
  
  debugPrint("====== LOOP START  ========");
  if (digitalRead(inputType) == LOW) 
  {
   //case 1: Photodetector
   debugPrint("CASE1");
   lcdOutput("CASE:1",1);
   //if (pulseIn(vt,HIGH,1000) > 1) //If we see a VT transmission, we got an input from photodiode
   if (1) //If we see a VT transmission, we got an input from photodiode
   {
     debugPrint("GOT VT");
     int d0=0,d1=0,d2=0,d3=0;
     d0=digitalRead(d0pin);
     d1=digitalRead(d1pin);
     d2=digitalRead(d2pin);
     d3=digitalRead(d3pin);
     
     tmpStr=String(char(d0+'0'))+String(char(d1+'0'))+String(char(d2+'0'))+String(char(d3+'0'));   
     debugPrint("DECODER says:"+tmpStr);
     
     if (( d0 == 1 ) && ( d1 == 1 ) && ( d2 == 1 ) && ( d3 == 1 )) { return; }
     if (( d0 == 0 ) && ( d1 == 0 ) && ( d2 == 0 ) && ( d3 == 0 )) { return; }
     
  
     String command=tmpStr.substring(0,2);
     String cmdTimer=tmpStr.substring(2,4);
     debugPrint("got command:"+command);
     debugPrint("Got delay:" + cmdTimer);
     if (command == strSTOP )
     {  lcdData=lcdStrSignalSTOP; 
        motorControlValue=0;
        cascadeSignal=strSTOP; 
        cascadeSignalDelay=cmdTimer;   
     }
     if (command == strGO )
     {   lcdData=lcdStrSignalGO; 
         motorControlValue=1; 
         cascadeSignal=strGO;
         cascadeSignalDelay=cmdTimer;

     }
     delay(1000);
   }//of pulse vt
  }
  else
  {
    //case 2#
    Serial.print("CASE2\n");
    
    lcdOutput("CASE:2",1);
    digitalWrite(FUsonicTrigger, HIGH);
    delayMicroseconds (usonicTriggerDelay);
    digitalWrite(FUsonicTrigger, LOW);
    int duration = pulseIn(FUsonicEcho, HIGH);
    int distance = abs((duration/2)/29.1);
    if (distance <1 ) return;
    tmpStr=String(distance);
    strDistance=tmpStr;
    debugPrint("Got FUS distance as:" + tmpStr);
    if ( distance  <= case2Distance ) 
    {
      lcdData=lcdStrEmerSTOP+"("+strDistance+")         ";  
      motorControlValue=0;
      cascadeSignal=strSTOP; 
      cascadeSignalDelay="00";   
      
    }
    else
    {
      lcdData=lcdStrEmerGO;
      motorControlValue=1;
      cascadeSignal=strGO;
      cascadeSignalDelay="00";
      
    }
    
  }
    
  //We have received data.
  //do actions
 
   debugPrint("LCD DISPLAY:"+lcdData);
   debugPrint("MOTOR CONTOL:" +String(motorControlValue));
   
   lcdOutput(lcdData,2);
   controlMotor(motorControlValue);
   
   {//Begin looking for back car. 
    debugPrint("TAIL CAR LOGIC BEGIN");
    digitalWrite(BUsonicTrigger, HIGH);
    delayMicroseconds (usonicTriggerDelay);
    digitalWrite(BUsonicTrigger, LOW);
    int duration = pulseIn(BUsonicEcho, HIGH);
    int distance = abs((duration/2)/29.1);
    if (distance < 1 ) return; // we ignore spurious untriggered values
    tmpStr=String(distance);
//    debugPrint("yes");
    debugPrint("Got BUS distance as: "+tmpStr);
    
    if ( distance < case2Distance) 
    {
      debugPrint("CAR in TAIL");
      sendSignal(cascadeSignal,cascadeSignalDelay);
      debugPrint("SEND data to tail car");
    }
    else
    {
       digitalWrite(te,LOW);
       debugPrint("STOP data to tail car");
    }
//    Serial.print("TAIL CAR LOGIC END\n");
   }
   delay(500);
//   Serial.print("  ");
     debugPrint("######## LOOP END  ========\n");
}
