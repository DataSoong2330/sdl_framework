#ifndef __LOGFILE__
#define __LOGFILE__

#include <stdio.h>
#include <stdarg.h>

#define MAX_BUFFER 1024
#define L_FAIL false
#define L_OK true

enum FONTCOLORS { BLACK, RED, GREEN, BLUE, PURPLE };

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

        void createLogfile(const char *Logname);
        void WriteTopic(const char *Topic, int Size);
        void Textout(const char *Text);
        void Textout(int Color, const char *Text);
        void Textout(int Color, bool List, const char *Text);
        void fTextout(const char *Text, ...);
        void fTextout(int Color, const char *Text, ...);
        void fTextout(int Color, bool List, const char *Text, ...);
        void FunctionResult(const char *Name, bool Result);

        void quitLogging();

    protected:
    private:
        Logfile();
        ~Logfile();

        static Logfile *pLogfile;

        FILE *logfile;
};

#endif // __LOGFILE__
