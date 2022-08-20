#include "../header/SystemTime.hpp"

MyTime* MyTime::myTime = 0;

// empty ctor
MyTime::MyTime()
{
}

// emtpy ctor
MyTime::~MyTime()
{
}

// get the date as a string
std::string MyTime::getDate()
{
    this->output.clear();

    this->timeStamp = time(0);
    this->now = localtime(&timeStamp);

    this->output << this->now->tm_mday << "." << this->now->tm_mon+1 << "." << this->now->tm_year+1900;

    this->output >> this->temp;

    return this->temp;
}

// get the time as a string
std::string MyTime::getTime()
{
    //std::stringstream input;
    this->output.clear();

    this->timeStamp = time(0);
    this->now = localtime(&timeStamp);

    this->output << this->now->tm_hour << ":" << this->now->tm_min << ":" << this->now->tm_sec;

    this->output >> this->temp;

    return this->temp;
}

// get date and time as a string
std::string MyTime::getDateTime()
{
    this->output.clear();

    this->timeStamp = time(0);
    this->now = localtime(&timeStamp);

    this->output << this->now->tm_mday << "." << this->now->tm_mon+1 << "." << this->now->tm_year+1900;
    this->output << "-" << this->now->tm_hour << ":" << this->now->tm_min << ":" << this->now->tm_sec;

    this->output >> this->temp;

    return this->temp;
}

// get the time and date in american format
std::string MyTime::getASCTime()
{
    this->output.clear();

    //this->output.str(asctime(this->now));

    this->temp = asctime(this->now);

    //this->output >> this->temp;

    return this->temp;
}

// starts a timer
void MyTime::startTimer()
{
    this->start = clock();
}

// ends a started timer
void MyTime::endTimer()
{
    this->end = clock();
}

// gets a value from a ended timer
std::string MyTime::getTimerValue()
{
    this->output.clear();

    this->output << this->end << ":" << CLOCKS_PER_SEC;

    this->output >> this->temp;

    return this->temp;
}
