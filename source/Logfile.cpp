#include <debug/macros.h>

#include "../header/Logfile.hpp"
#include "../header/SystemTime.hpp"

Logfile* Logfile::pLogfile = 0;

// Fauler Konstruktor, der tut doch nix
Logfile::Logfile()
{
}

// Der Destruktor schreibt Ende-Meldung und schließt die Logfile
Logfile::~Logfile()
{
    this->quitLogging();
}

// Create Logfile
// Logfile erstellen und Kopf schreiben
void Logfile::createLogfile(std::string filename)
{
    this->logfileName = filename;

    // Aktuelle Build-konfiguration ausgeben
    #ifdef NDEBUG
        Textout("BUILD: DEBUG");
    #else
        Textout("Application", "BUILD", "RELEASE");
    #endif // _DEBUG

    // Link für E-Mail-Adresse schreiben
    Textout("Contact","email", "data.soong.2330@gmail.com");
}

// Textout
// Text in schwarz ins Logfile schreiben
/*void Logfile::Textout(std::string key, std::string value, bool withTime)
{
    if(withTime)
    {
        std::string time = MyTime::Instance()->getTime();
        this->logfile[time][key] = value;
    }
    else
    {
        this->logfile[key] = value;
    }
}*/

void Logfile::Textout(std::string category, std::string key, std::string value)
{
    this->logfile[category][key] = value;
}

void Logfile::quitLogging()
{
    std::ofstream o(this->logfileName);
    o << std::setw(4) << this->logfile << std::endl;
}
