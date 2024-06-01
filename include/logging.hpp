#pragma once
/*=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-
  @author Ryan Cosgrove (ryancosgrove@gmail.com)
  @version 1.0
\*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include <stdio.h>
#include <cstring>
#include <cstdarg>

enum class ELogSeverity
{
    Fatal = 0,
    Critical,
    Error,
    Warning,
    Info,
    Verbose,
    Debug,
    Count
};

/*zen logging function type-def*/
typedef void (*LoggingFunction_t)(FILE* fp, ELogSeverity Severity, const char* fmt, ...);

/*Logging variables*/
extern LoggingFunction_t LoggingFunctions[(int)ELogSeverity::Count];
extern FILE* LoggingFiles[(int)ELogSeverity::Count];

/*Toggle logging*/
extern void EnableLogging (ELogSeverity Severity);
extern void DisableLogging(ELogSeverity Severity);
extern void ToggleLogging (ELogSeverity Severity);
extern void LogConditional(bool Condition, ELogSeverity Severity, const char* TrueFormat, const char* FalseFormat, ...);

/*Set Logging variables*/
extern void SetLoggingFile(ELogSeverity Severity, FILE* fp);
extern void SetLoggingFunction(ELogSeverity Severity, LoggingFunction_t func);
extern void SetLoggingVariables(ELogSeverity Severity, FILE* fp, LoggingFunction_t func);

/*Default logging functions*/
extern void EnabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...);
extern void DisabledLoggingFunction(FILE* fp, ELogSeverity Severity, const char* fmt, ...);

/*=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=*/
//#define RM_ZLP_VERBOSE
#include <logging.inl>