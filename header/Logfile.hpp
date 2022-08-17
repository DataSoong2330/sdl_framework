#ifndef __LOGFILE__
#define __LOGFILE__

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

// Klassendeklaration
class Logfile
{
    public:
        static Logfile* Instance()
        {
            if(pLogfile == 0)
                pLogfile = new Logfile();

            return pLogfile;
        }

        void createLogfile(std::string filename);
        //void Textout(std::string key, std::string value, bool withTime = false);
        void Textout(std::string category, std::string key, std::string value);

        void quitLogging();

    protected:
    private:
        Logfile();
        ~Logfile();

        static Logfile *pLogfile;

        std::string logfileName;
        nlohmann::json logfile;
};

#endif // __LOGFILE__
