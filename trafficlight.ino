// Author sumeer@unixindia.com
int e0=A0;
int e1=A1;
int e2=A2;
int e3=A3;

 const int te=A5;
 const int redled=12 ;
 const int greenled=8;

void setup ()
{
 
   pinMode (redled, OUTPUT);
   pinMode (greenled, OUTPUT);
   pinMode(te,OUTPUT);
   pinMode (e0,OUTPUT);
    pinMode (e1,OUTPUT);
    pinMode (e2,OUTPUT);
    pinMode (e3,OUTPUT);
    Serial.begin(9600);
}
void loop ()
{
  
  //GREEN SIGNAL
  Serial.println("GREEN");
  digitalWrite (te, LOW);
  
  digitalWrite (e0, HIGH);
  digitalWrite (e1, LOW);
  digitalWrite (e2, HIGH);
  digitalWrite (e3, LOW);
  digitalWrite (te, HIGH); //This is connected as NOT(TE from Arduio)
  digitalWrite (redled, LOW);
  digitalWrite (greenled, HIGH);
  delay (8000);
 
  

   //RED SIGNAL
  Serial.println("RED");
  digitalWrite (te, LOW);
  digitalWrite (e0, LOW);
  digitalWrite (e1, HIGH);
  digitalWrite (e2, LOW);
  digitalWrite (e3, HIGH);
  digitalWrite (te, HIGH);
  digitalWrite (redled, HIGH );
  digitalWrite (greenled, LOW);
  
  delay (8000);
  
  
 

}
  
  
