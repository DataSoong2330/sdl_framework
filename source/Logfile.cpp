#include <debug/macros.h>

#include "../header/Logfile.hpp"

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
void Logfile::createLogfile(const char *Logname)
{
    // Logfile leeren und Kopf schreiben
    this->logfile = fopen(Logname, "w");
    Textout("<html><head><title>Logfile</title></head>");
    Textout("<body><font face='courier new'>");
    WriteTopic("Logfile", 3);

    // Aktuelle Build-konfiguration ausgeben
    #ifdef NDEBUG
        Textout("BUILD: DEBUG<br>");
    #else
        Textout("BUILD: RELEASE<br>");
    #endif // _DEBUG

    // Link für E-Mail-Adresse schreiben
    Textout("<a href='mailto:data.soong.2330@gmail.com?subject=Logfile'>");
    Textout("Send E-Mail to me</a><br><br>");

    // Logfile schließen und mit append wieder öffnen
    fclose(this->logfile);
    this->logfile = fopen(Logname, "a");
}

// WriteTopic
// Überschrift erzeugen
void Logfile::WriteTopic(const char *Topic, int Size)
{
    // Überschrift schreiben und flushen
    Textout("<table cellspacing='0' cellpadding='0' width='100%%' ");
    Textout("bgcolor='#DFDFE5'>\n<tr>\n<td>\n<font face='arial' ");
    fTextout("size='+%i'>\n", Size);
    Textout(Topic);
    Textout("</font>\n</td>\n</tr>\n<table>\n<br>");
    fflush(this->logfile);
}

// Textout
// Text in schwarz ins Logfile schreiben
void Logfile::Textout(const char *Text)
{
    fputs(Text, this->logfile);
    fflush(this->logfile);
}

// Textout
// Text farbig ins Logfile schreiben
void Logfile::Textout(int Color, const char *Text)
{
    Textout(Color, false, Text);
}

// Textout
// Text farbig in eine Liste schreiben
void Logfile::Textout(int Color, bool List, const char *Text)
{
    // Listentag schreiben
    if(List == true)
        Textout("<li>");

    // Farbtag schreiben
    switch(Color)
    {
        case BLACK:     Textout("<font color=black>");
                        break;
        case RED:       Textout("<font color=red>");
                        break;
        case GREEN:     Textout("<font color=green>");
                        break;
        case BLUE:      Textout("<font color=blue>");
                        break;
        case PURPLE:    Textout("<font color=purple>");
                        break;
    }

    // Text schreiben
    Textout(Text);
    Textout("</font>");

    if(List == false)
        Textout("<br>");
    else
        Textout("</li>");
}

// fTextout
// Formatierten Text ins Logfile schreiben
void Logfile::fTextout(const char *Text, ...)
{
    // char-buffer
    char buffer[MAX_BUFFER];
    // Liste der übergebenen Argumente
    va_list pArgList;

    // String aus den Argumenten erstellen
    va_start(pArgList, Text);
    vsprintf(buffer, Text, pArgList);
    va_end(pArgList);

    // Erzeugten String schreiben
    Textout(buffer);
}

// fTextout
// formatierten Text farbig ins Logfile schreiben
void Logfile::fTextout(int Color, const char *Text, ...)
{
    // char-buffer
    char buffer[MAX_BUFFER];
    // Liste der übergebenen Argumente
    va_list pArgList;

    // String aus den Argumenten erstellen
    va_start(pArgList, Text);
    vsprintf(buffer, Text, pArgList);
    va_end(pArgList);

    // Erzeugten String schreiben
    Textout(Color, buffer);
}

// ftextout
// formatierten Text farbig als Liste ins Logfile schreiben
void Logfile::fTextout(int Color, bool List, const char *Text, ...)
{
    // char-buffer
    char buffer[MAX_BUFFER];
    // Liste der übergebenen Argumente
    va_list pArgList;

    // String aus den Argumenten erstellen
    va_start(pArgList, Text);
    vsprintf(buffer, Text, pArgList);
    va_end(pArgList);

    // Erzeugten String ins Logfile schreiben
    Textout(Color, List, buffer);
}

// FunctionResult
//
void Logfile::FunctionResult(const char *Name, bool Result)
{
    if(L_OK == Result)
    {
        Textout("<table width='100%%' cellspacing='1' cellpadding='5'");
        Textout(" border='0' bgcolor='#C0C0C0'><tr><td bgcolor=");
        fTextout("'#FFFFFF' width='35%%'>%s</td>", Name);
        Textout("<td bgcolor='#FFFFFF' width='30%%'><font color=");
        Textout("'green'>OK</font></td><td bgcolor='#FFFFFF' ");
        Textout("width='35%%'>-/-</td></tr></table>");
    }
    else
    {
        Textout("<table width='100%%' cellspacing='1' cellpadding='5'");
        Textout(" border='0' bgcolor='#C0C0C0'><tr><td bgcolor=");
        fTextout("'#FFFFFF' width='35%%'>%s</td>", Name);
        Textout("<td bgcolor='#FFFFFF' width='30%%'><font color=");
        Textout("'red'>ERROR</font></td><td bgcolor='#FFFFFF' ");
        Textout("width='35%%'>-/-</td></tr></table>");
    }
}

void Logfile::quitLogging()
{
    Textout("<br><br>End of Logfile</font></body></html>");
    fclose(this->logfile);
}
