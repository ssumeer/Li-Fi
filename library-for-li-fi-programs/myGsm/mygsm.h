
#ifndef __MYGSM__
#define __MYGSM__

#include <SoftwareSerial.h>
#include "Arduino.h"
class myGsm
{
	public:
		void gsmstart(int,int); //1->RX, 2->TX
		void sendsms(String,String);
		void makecall(String);
		myGsm();
	private:
		SoftwareSerial *myser;
		void send_data(String);
		void gsm_init();
		void serialEvent();
		void call(String);
		void sms(String,String);


};

#endif
