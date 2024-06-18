//
// Created by ryan on 5/31/24.
//
#include "logging.hpp"
#include "LSystemAlphabet.h"
#include <windows.h>


#ifdef WIN32
static bool bColorsInitialized = false;
#endif

LoggingFunction_t LoggingFunctions[(int) ELogSeverity::Count]
        {
                EnabledLoggingFunction,
                EnabledLoggingFunction,
                EnabledLoggingFunction,
                EnabledLoggingFunction,
                EnabledLoggingFunction,
                EnabledLoggingFunction,
                EnabledLoggingFunction
        };

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


//enable logging for the given severity
void EnableLogging(ELogSeverity Severity)
{
    LoggingFunctions[(int) Severity] = EnabledLoggingFunction;
}

//disable logging for the given severity
void DisableLogging(ELogSeverity Severity)
{
    LoggingFunctions[(int) Severity] = DisabledLoggingFunction;
}

//toggle logging for the given severity
void ToggleLogging(ELogSeverity Severity)
{
    LoggingFunctions[(int) Severity] =
            LoggingFunctions[(int) Severity] == EnabledLoggingFunction
            ? DisabledLoggingFunction
            : EnabledLoggingFunction;
}

void LogConditional(bool Condition, ELogSeverity Severity, const char* TrueFormat, const char* FalseFormat, ...)
{

}

//set logging file for the given severity
void SetLoggingFile(ELogSeverity Severity, FILE* fp)
{
    LoggingFiles[(int) Severity] = fp;
}

//set custom logging function for the given severity
void SetLoggingFunction(ELogSeverity Severity, LoggingFunction_t func)
{
    LoggingFunctions[(int) Severity] = func;
}

//set logging variables (file/function) for the given severity
void SetLoggingVariables(ELogSeverity Severity, FILE* fp, LoggingFunction_t func)
{
    SetLoggingFile(Severity, fp);
    SetLoggingFunction(Severity, func);
}

//enabled logging function,
void EnabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...)
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

    enum class CLI_Brightness
    {
        NORMAL = 0,
        BRIGHT = 1,
        COUNT = 2
    };

    enum class CLI_Color
    {
        BLACK = 0,
        RED = 1,
        GREEN = 2,
        YELLOW = 3,
        BLUE = 4,
        MAGENTA = 5,
        CYAN = 6,
        WHITE = 7,
        COUNT
    };


    struct CLI_ForeGroundBackgroundColors
    {
        CLI_Brightness ForegroundBrightness = CLI_Brightness::NORMAL;
        CLI_Color ForegroundColor = CLI_Color::WHITE;

        CLI_Brightness BackgroundBrightness = CLI_Brightness::NORMAL;
        CLI_Color BackgroundColor = CLI_Color::BLACK;
    };

    //std::map<ELogSeverity, CLI_ForeGroundBackgroundColors> SomeMap =
    const CLI_ForeGroundBackgroundColors SeverityColors[(int) ELogSeverity::Count] =
            {
                    {CLI_Brightness::BRIGHT, CLI_Color::MAGENTA},   //Fatal
                    {CLI_Brightness::BRIGHT, CLI_Color::RED},       //Critical
                    {CLI_Brightness::BRIGHT, CLI_Color::YELLOW},       //Error
                    {CLI_Brightness::NORMAL, CLI_Color::YELLOW},       //Warning
                    {CLI_Brightness::NORMAL, CLI_Color::WHITE},       //Info
                    {CLI_Brightness::BRIGHT, CLI_Color::GREEN},       //Verbose
                    {CLI_Brightness::BRIGHT, CLI_Color::CYAN}        //Debug
            };

    const char* SeverityStrings[(int) ELogSeverity::Count]
            {
                    "Fatal",
                    "Critical",
                    "Error",
                    "Warning",
                    "Info",
                    "Verbose",
                    "Debug"
            };

    constexpr int MaxEntryLength = 2048;
    char EntryBuffer[MaxEntryLength] = {0};
    //color code : severity string : format : args
    const CLI_ForeGroundBackgroundColors& Color = SeverityColors[(int) Severity];

    const int ForegroundPrefix = Color.ForegroundBrightness == CLI_Brightness::NORMAL
                                 ? 3
                                 : 9;
    const int BackgroundPrefix = Color.BackgroundBrightness == CLI_Brightness::NORMAL
                                 ? 4
                                 : 10;

    /*
    sprintf(EntryBuffer, "\x1b[%d%dm\x1b[%d%dm%s:%s",
            ForegroundPrefix, (int)Color.ForegroundColor,
            BackgroundPrefix, (int)Color.BackgroundColor,
            SeverityStrings[(int)Severity], fmt);
    */

    sprintf(EntryBuffer, "%s", fmt);


    //background printf("\x1b[%d%dm",prefix_bg[br], _ZT_BG_COLOR_CURRENT_);
    //foreground printf("\x1b[%d%dm",prefix_fg[br], _ZT_FG_COLOR_CURRENT_);

    va_list arglist;

    va_start(arglist, fmt);
    vfprintf(fp, EntryBuffer, arglist);
    va_end(arglist);
}

void DisabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...)
{
    //intentionally left blank
}




