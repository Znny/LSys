#pragma once
/*=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-
  @author Ryan Cosgrove (ryancosgrove@gmail.com)
  @version 1.0
\*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include <stdio.h>
#include <cstring>
#include <cstdarg>

typedef enum ZEN_LOG_PRIORITY
{
	ZLP_VERBOSE=0,
	ZLP_DEBUG,
	ZLP_INFO,
	ZLP_WARNING,
	ZLP_ERROR,
	ZLP_CRITICAL,
	ZLP_CNT
}ZL_PRIORITY;

/*zen logging function type-def*/
typedef void (*ZEN_LogFunction)(FILE* fp, const char* fmt, ...);

/*Logging variables*/
extern ZEN_LogFunction 	ZEN_LogFunc[ZLP_CNT];
extern FILE* 		ZEN_LogFile[ZLP_CNT];

/*Toggle logging*/
extern void ZEN_EnableLogging (ZL_PRIORITY priori);
extern void ZEN_DisableLogging(ZL_PRIORITY priori);
extern void ZEN_ToggleLogging (ZL_PRIORITY priori);
extern void ZEN_ConditionalLog(bool cond, ZL_PRIORITY priori, const char* fmt1, const char* fmt2, ...);


/*Set Logging variables*/
extern void ZEN_SetLogFile(ZL_PRIORITY priori, FILE* fp);
extern void ZEN_SetLogFunc(ZL_PRIORITY priori, ZEN_LogFunction func);
extern void ZEN_SetLogVars(ZL_PRIORITY priori, FILE* fp, ZEN_LogFunction func);


/*Default logging functions*/
extern void ZLF_Enabled(FILE* fp, const char* fmt, ...);
extern void ZLF_Disabled(FILE* fp, const char* fmt, ...);

/*=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=*/
//#define RM_ZLP_VERBOSE
#include <logging.inl>