#include "Arduino.h"
#include <SoftwareSerial.h>



class myTTS
{

        public:
                speak(String,bool);
                myTTS(int,int);
        private:
                SoftwareSerial *tts_ser;
                bool isRunning;
};

