//
// Created by ryan on 5/31/24.
//
#include "logging.hpp"

LoggingFunction_t LoggingFunctions[(int)ELogSeverity::Count]
{
    EnabledLoggingFunction
};

FILE* LoggingFiles[(int)ELogSeverity::Count]
{
    stdout
};

//enable logging for the given severity
void EnableLogging (ELogSeverity Severity)
{
    LoggingFunctions[(int)Severity] = EnabledLoggingFunction;
}

//disable logging for the given severity
void DisableLogging(ELogSeverity Severity)
{
    LoggingFunctions[(int)Severity] = DisabledLoggingFunction;
}

//toggle logging for the given severity
void ToggleLogging (ELogSeverity Severity)
{
    LoggingFunctions[(int)Severity] = LoggingFunctions[(int)Severity] == EnabledLoggingFunction ? DisabledLoggingFunction : EnabledLoggingFunction;
}

void LogConditional(bool Condition, ELogSeverity Severity, const char* TrueFormat, const char* FalseFormat, ...)
{

}

//set logging file for the given severity
void SetLoggingFile(ELogSeverity Severity, FILE* fp)
{
    LoggingFiles[(int)Severity] = fp;
}

//set custom logging function for the given severity
void SetLoggingFunction(ELogSeverity Severity, LoggingFunction_t func)
{
    LoggingFunctions[(int)Severity] = func;
}

//set logging variables (file/function) for the given severity
void SetLoggingVariables(ELogSeverity Severity, FILE* fp, LoggingFunction_t func)
{
    SetLoggingFile(Severity, fp);
    SetLoggingFunction((Severity, func);
}

//enabled logging function,
void EnabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...)
{
    va_list arglist;

    va_start( arglist, fmt );
    vfprintf( fp, fmt, arglist );
    va_end( arglist );
}

void DisabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...)
{
    //intentionally left blank
}




