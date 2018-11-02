#include <mygsm.h>



void myGsm::send_data(String message)
{
  this->myser->listen();
  this->myser->println(message);
  delay(200);
}

__attribute__((noinline)) 
void myGsm::gsm_init()
{

  Serial.println("Starting GSM");
  boolean at_flag=1;
  this->myser->listen();
  while(at_flag)
  {
    this->myser->println("AT");
    while(this->myser->available()>0)
    {
      if(this->myser->find("OK"))
      at_flag=0;
    }
    
    Serial.println("Waiting for AT");
    delay(1000);
  }

  Serial.println("AT Done");
  delay(1000);
  boolean echo_flag=1;
  while(echo_flag)
  {
    this->myser->listen();
    this->myser->println("ATE1");
    while(this->myser->available()>0)
    {
      if(this->myser->find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }
  Serial.println("Echo off");

  delay(1000);
  boolean net_flag=1;
  while(net_flag)
  {
    this->myser->listen();
    this->myser->println("AT+CPIN?");
    while(this->myser->available()>0)
    {
      if(this->myser->find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
  delay(1000);
  Serial.println("GSM Ready");
}

__attribute__((noinline)) 
void myGsm::serialEvent()
{

	String instr="";
	int i=0,ring=0,sms_flag=0;
  this->myser->listen();
  while(this->myser->available())
  {
    char ch=this->myser->read();
    instr+=ch;
    i++;

    if(instr[i-4] == 'R' && instr[i-3] == 'I' && instr[i-2] == 'N' && instr[i-1] == 'G' )
    {
       ring=1;
    }

    if(instr.indexOf("NO CARRIER")>=0)
    {
       ring=0;
       i=0;
    }
    if(instr.indexOf("+CMTI: \"SM\"")>=0)
    {
      sms_flag=1;
    }
  }
}



//===============
__attribute__((noinline)) 
void myGsm::call(String number)
{
	String str1="";
     this->myser->listen();
     this->serialEvent();
        this->myser->print("ATD");
        this->myser->print(number);
        this->myser->println(";");
        long stime=millis()+5000;
        int ans=1;
        while(ans==1)
        {           
          while(this->myser->available()>0)
          {
            if(this->myser->find("OK"))
            {
              int l=0;
              str1="";
              while(ans==1)
              {
                while(this->myser->available()>0)
                {
                  char ch=this->myser->read();
                  str1+=ch;
                  if(str1.indexOf("NO CARRIER")>0)
                  {
                    delay(2000);
                    ans=0;
                    return;
                  }
                 }
                }
              }  
            }
        } 
}

__attribute__((noinline)) 
void myGsm::sms(String number, String msg)
{
  this->myser->listen();
  this->myser->println("AT+CMGF=1");
  delay(400);
  delay(2000);
  this->myser->print("AT+CMGS=\"");
  this->serialEvent();
  this->myser->print(number);
  this->myser->println("\"");
  delay(2000);
  this->myser->print(msg);
  this->myser->write(26);

}

myGsm::myGsm()
{
;	
}
__attribute__((noinline)) 
void myGsm::gsmstart(int rx,int tx)
{
	this->myser=new SoftwareSerial(rx,tx,false);
	this->myser->begin(115200);
    this->myser->listen();
	this->gsm_init();
}

__attribute__((noinline)) 
void myGsm::sendsms(String number, String msg)
{
	this->sms(number,msg);
}
