//Author sumeer@unixindia.com
#include <Wire.h>
const byte slaveId = 2;


//I2C data
String i2cData="";
boolean isi2cDataAvbl=false;

//I2C data is received as
// SPEED:XY
// 20-30 1100
// 30-40 1101
// 40-50 1110

//encoder digital pins 

const int encD0=10;
const int encD1=10;
const int encD2=10;
const int encD3=10;

void setup() {
  Wire.begin(slaveId);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  pinMode(encD0,OUTPUT);
  pinMode(encD1,OUTPUT);
  pinMode(encD2,OUTPUT);
  pinMode(encD3,OUTPUT);
  Serial.begin(9600);           // start serial for output
}

void loop() {

  if ( isi2cDataAvbl)
  {
    int len=i2cData.length();
    if ( i2cData.substring(0, 5) == "SPEED:" )
    {
      String sub=i2cData.substring(6,len);
      int sped=sub.toInt();
      String data2ENC="";
      if ( sped <10 ) { data2ENC="1100"; }
      if ( ( sped >=10 ) && ( sped <20 ) ) { data2ENC="1101"; }
      if ( ( sped >=20 ) && ( sped <30 ) ) { data2ENC="1110"; }
      digitalWrite(encD0, data2ENC[3]-'0');
      digitalWrite(encD1, data2ENC[2]-'0');
      digitalWrite(encD2, data2ENC[1]-'0');
      digitalWrite(encD3, data2ENC[0]-'0');
      
      
    }
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {

  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    i2cData=i2cData+c;
    if ( c == 0 ) {
      isi2cDataAvbl=true;
      return;
    }
    
    
  }
  //Serial.println("");    // print the character
 // int x = Wire.read();    // receive byte as an integer
  //Serial.println(x);         // print the integer
}
