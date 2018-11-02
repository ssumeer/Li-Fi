#include "myGPS.h"


myGPS::myGPS()
{


}


myGPS::myGPS(int rx,int tx)
{
        

        gps_ser=new SoftwareSerial(rx,tx,true);
        gps_ser->begin(9600);
        gps_ser->listen();

}


String myGPS::readLoc(void)
{
        gps_ser->listen();
        this->data="";
        float lat,lon;
        unsigned long age;
        while(gps_ser->available())
        {
            tgps.encode(gps_ser->read());
        }
        tgps.f_get_position(&lat, &lon, &age);
        this->data = "LAT:"+String(lat)+"  LON:"+String(lon);
        
        


return this->data;
}
