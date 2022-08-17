#ifndef __MYTIME__
#define __MYTIME__

#include <sstream>
#include <string>
#include <time.h>

class MyTime
{
    public:
        static MyTime* Instance()
        {
            if(myTime == 0)
                myTime = new MyTime();

            return myTime;
        }

        std::string getDate();
        std::string getTime();
        std::string getDateTime();
        std::string getASCTime();

        void startTimer();
        void endTimer();
        std::string getTimerValue();

    protected:
    private:
        MyTime();
        ~MyTime();

        static MyTime *myTime;

        time_t timeStamp;
        tm *now;

        clock_t start, end;

        std::stringstream output;
        std::string temp;
};

/*
struct tm {
   int tm_sec;      Sekunden - [0,61]
   int tm_min;      Minuten - [0,59]
   int tm_hour;     Stunden - [0,23]
   int tm_mday;     Tag des Monats - [1,31]
   int tm_mon;      Monat im Jahr - [0,11]
   int tm_year;     Jahr seit 1900
   int tm_wday;     Tage seit Sonntag (Wochentag) - [0,6]
   int tm_yday;     Tage seit Neujahr (1.1.) - [0,365]
   int tm_isdst;    Sommerzeit-Flag
}
*/

#endif // __MYTIME__
