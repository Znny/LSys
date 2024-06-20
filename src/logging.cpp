#include "logging.hpp"

//color initialization is currently only needed when running on windows
#ifdef WIN32
#include <windows.h>
static bool bColorsInitialized = false;
#endif

///logging functions that each severity uses when logging a message
LoggingFunction_t LoggingFunctions[(int) ELogSeverity::Count]
        {
                DefaultEnabledLoggingFunction,
                DefaultEnabledLoggingFunction,
                DefaultEnabledLoggingFunction,
                DefaultEnabledLoggingFunction,
                DefaultEnabledLoggingFunction,
                DefaultEnabledLoggingFunction,
                DefaultEnabledLoggingFunction
        };

///files logged to for each severity, defaults to stdout
FILE* LoggingFiles[(int) ELogSeverity::Count]
        {
                stdout,
                stdout,
                stdout,
                stdout,
                stdout,
                stdout,
                stdout
        };


void InitializeLogColors()
{
#ifdef WIN32
    if (!bColorsInitialized)
    {
        bColorsInitialized = true;
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE)
        {
            DWORD hOutMode = 0;
            GetConsoleMode(hOut, &hOutMode);
            hOutMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, hOutMode);
        }
    }
#endif
}

void EnableLoggingForSeverity(const ELogSeverity Severity)
{
    LoggingFunctions[(int) Severity] = DefaultEnabledLoggingFunction;
}

void DisableLoggingForSeverity(const ELogSeverity Severity)
{
    LoggingFunctions[(int) Severity] = DefaultDisabledLoggingFunction;
}

void SetLoggingFileForSeverity(const ELogSeverity Severity, FILE* fp)
{
    LoggingFiles[(int) Severity] = fp;
}

void SetLoggingFunctionForSeverity(const ELogSeverity Severity, LoggingFunction_t func)
{
    LoggingFunctions[(int) Severity] = func;
}

//enabled logging function,
void DefaultEnabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...)
{
    const char* SeverityColors[(int) ELogSeverity::Count]
            {
                    "\x1b[38;2;255;0;255m",     //fatal
                    "\x1b[38;2;200;20;200m",    //critical
                    "\x1b[38;2;255;0;0m",       //error
                    "\x1b[38;2;255;255;0m",     //warning
                    "\x1b[38;2;255;255;255m",   //info
                    "\x1b[38;2;0;200;200m",     //verbose
                    "\x1b[38;2;0;128;0m",       //debug
            };

    const char* SeverityStrings[(int) ELogSeverity::Count]
            {
                    "FATAL",
                    "CRITICAL",
                    "Error",
                    "Warning",
                    "Info",
                    "Verbose",
                    "Debug"
            };

    constexpr int MaxEntryLength = 2048;
    char EntryBuffer[MaxEntryLength] = {0};

    sprintf(EntryBuffer, "%s%s:%s", bColorsInitialized ? SeverityColors[(int) Severity] : "", SeverityStrings[(int) Severity], fmt);

    //capture arguments and pass to vfprintf for output
    va_list arglist;
    va_start(arglist, fmt);
    vfprintf(fp, EntryBuffer, arglist);
    va_end(arglist);
}

void DefaultDisabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...)
{
    //intentionally left blank
}




