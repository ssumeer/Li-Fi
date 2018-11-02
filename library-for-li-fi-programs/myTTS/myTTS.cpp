#include "myTTS.h"


myTTS::myTTS(int rx,int tx)
{

        tts_ser=new SoftwareSerial(rx,tx);

}


myTTS::speak(String v,bool block)
{

        tts_ser->listen();
        tts_ser->write(const_cast<char*>(v.c_str()));
        if (block)
        {
                while(tts_ser->available())
                { //blocking call till the voice speaks
                        if ( tts_ser->read() == '#' ) break;
                }
        }
}

