#pragma once
#include <cstdio>

//log severity enum, each severity has its own logging function and file which it outputs to
enum class ELogSeverity
{
    Fatal       = 0,
    Critical    = 1,
    Error       = 2,
    Warning     = 3,
    Info        = 4,
    Verbose     = 5,
    Debug       = 6,
    Count = 7
};

//logging function type definition. takes in a file pointer, severity, format, and variadic arguments
typedef void (* LoggingFunction_t)(FILE* fp, ELogSeverity Severity, const char* fmt, ...);

//arrays of logging functions and files, for each severity
extern LoggingFunction_t LoggingFunctions[(int) ELogSeverity::Count];
extern FILE* LoggingFiles[(int) ELogSeverity::Count];

//initialize ANSI escape code colors for use in logging. Colors are not used when logging directly to a file
extern void InitializeLogColors();

//enable/disable logging for the given severity
extern void EnableLogging(ELogSeverity Severity);
extern void DisableLogging(ELogSeverity Severity);

//set/get logging file for the given severity
extern void SetLoggingFile(ELogSeverity Severity, FILE* fp);
extern FILE* GetLoggingFile(ELogSeverity Severity);

//set/get logging function for the given severity
extern void SetLoggingFunction(ELogSeverity Severity, LoggingFunction_t func);
extern LoggingFunction_t GetLoggingFunction(ELogSeverity Severity);

//default logging functions
extern void DefaultEnabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...);
extern void DefaultDisabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...);

#include "utility/logging.inl"