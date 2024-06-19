#pragma once
#include <cstdio>
#include <cstring>
#include <cstdarg>

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

extern LoggingFunction_t LoggingFunctions[(int) ELogSeverity::Count];
extern FILE* LoggingFiles[(int) ELogSeverity::Count];

/*** EnableLogging
 *
 * @param Severity
 */
extern void EnableLogging(ELogSeverity Severity);

/*** DisableLogging
 *
 * @param Severity
 */
extern void DisableLogging(ELogSeverity Severity);

extern void SetLoggingFile(ELogSeverity Severity, FILE* fp);
extern FILE* GetLoggingFile(ELogSeverity Severity);

extern void SetLoggingFunction(ELogSeverity Severity, LoggingFunction_t func);
extern LoggingFunction_t GetLoggingFunction(ELogSeverity Severity);

extern void EnabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...);

extern void DisabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...);

#include <logging.inl>