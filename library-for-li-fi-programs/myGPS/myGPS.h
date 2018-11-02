#include "Arduino.h"
#include <SoftwareSerial.h>
#include "TinyGPS.h"
#ifndef __MYGSM__
#define __MYGSM__

class myGPS
{

        public: 
                myGPS(int,int);  //RX,TX
                myGPS();
                String getCoordinates(void);
                String data;
                String readLoc(void);
        private:
                SoftwareSerial *gps_ser;
                TinyGPS tgps;
};

#endif
