#pragma once
/*=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-
  @author Ryan Cosgrove (ryancosgrove@gmail.com)
  @version 1.0
\*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

// define REMOVE_ALL_LOGGING to remove all logging from source files
#ifdef REMOVE_ALL_LOGGING
#define REMOVE_CRITICAL_LOGGING
#define REMOVE_ERROR_LOGGING
#define REMOVE_WARNING_LOGGING
#define REMOVE_INFO_LOGGING
#define REMOVE_VERBOSE_LOGGING
#endif

//todo: make fatal and critical logs exit the program or send a sigkill/sigint, something along those lines

///Fatal logging////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef REMOVE_FATAL_LOGGING
constexpr int iFatal = (int) ELogSeverity::Fatal;
#define LogFatal(Format, ...) LoggingFunctions[iFatal](LoggingFiles[iFatal], ELogSeverity::Fatal, Format, ##__VA_ARGS__)
#define LogFatalConditional(chc, Format1, Format2, ...) LoggingFunctions[iFatal]( LoggingFiles[iFatal], (chc ? Format1 : Format2), ##__VA_ARGS__)
#else
#define LogFatal(Format, ...) //
#define LogFatalConditional(chc, Format1, Format2, ...) //
#endif

///Critical logging/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef REMOVE_CRITICAL_LOGGING
constexpr int iCritical = (int) ELogSeverity::Critical;
#define LogCritical(Format, ...) LoggingFunctions[iCritical](LoggingFiles[iCritical], ELogSeverity::Critical, Format, ##__VA_ARGS__)
#define LogCriticalConditional(chc, Format1, Format2, ...) LoggingFunctions[iCritical]( LoggingFiles[iCritical], (chc ? Format1 : Format2), ##__VA_ARGS__)
#else
#define LogCritical(Format, ...) //
#define LogCriticalConditional(chc, Format1, Format2, ...) //
#endif

///Error logging////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef REMOVE_ERROR_LOGGING
constexpr int iError = (int) ELogSeverity::Error;
#define LogError(Format, ...) LoggingFunctions[iError](LoggingFiles[iError], ELogSeverity::Error, Format, ##__VA_ARGS__)
#define LogErrorConditional(chc, Format1, Format2, ...) LoggingFunctions[iError]( LoggingFiles[iError], (chc ? Format1 : Format2), ##__VA_ARGS__)
#else
#define LogError(Format, ...) //
#define LogErrorConditional(chc, Format1, Format2, ...) //
#endif

///Warning logging//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef REMOVE_WARNING_LOGGING
constexpr int iWarning = (int) ELogSeverity::Warning;
#define LogWarning(Format, ...) LoggingFunctions[iWarning](LoggingFiles[iWarning], ELogSeverity::Warning, Format, ##__VA_ARGS__)
#define LogWarningConditional(chc, Format1, Format2, ...) LoggingFunctions[iWarning]( LoggingFiles[iWarning], (chc ? Format1 : Format2), ##__VA_ARGS__)
#else
#define LogWarning(Format, ...) //
#define LogWarningConditional(chc, Format1, Format2, ...) //
#endif

///Info logging/////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr int iInfo = (int) ELogSeverity::Info;
#ifndef REMOVE_INFO_LOGGING
#define LogInfo(Format, ...) LoggingFunctions[iInfo](LoggingFiles[iInfo], ELogSeverity::Info, Format, ##__VA_ARGS__)
#define LogInfoConditional(chc, Format1, Format2, ...) LoggingFunctions[iInfo]( LoggingFiles[iInfo], (chc ? Format1 : Format2), ##__VA_ARGS__)
#else
#define LogInfo(Format, ...) //
#define LogInfoConditional(chc, Format1, Format2, ...) //
#endif

///Verbose logging//////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr int iVerbose = (int) ELogSeverity::Verbose;
#ifndef REMOVE_VERBOSE_LOGGING
#define LogVerbose(Format, ...) LoggingFunctions[iVerbose]( LoggingFiles[iVerbose], ELogSeverity::Verbose, Format, ##__VA_ARGS__)
#define LogVerboseConditional(chc, Format1, Format2, ...) LoggingFunctions[iVerbose]( LoggingFiles[iVerbose], (chc ? Format1 : Format2), ##__VA_ARGS__)
#else
#define LogVerbose(Format, ...) //
#define LogVerboseConditional(chc, Format1, Format2, ...) //
#endif

///Debug logging////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef REMOVE_DEBUG_LOGGING
constexpr int iDebug = (int) ELogSeverity::Debug;
#define LogDebug(Format, ...) LoggingFunctions[iDebug](LoggingFiles[iDebug], ELogSeverity::Debug, Format, ##__VA_ARGS__)
#define LogDebugConditional(chc, Format1, Format2, ...) LoggingFunctions[iDebug]( LoggingFiles[iDebug], (chc ? Format1 : Format2), ##__VA_ARGS__)
#else
#define LogDebug(Format, ...) //
#define LogDebugConditional(chc, Format1, Format2, ...) //
#endif

