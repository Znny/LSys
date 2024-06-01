#pragma once
/*=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-
  @author Ryan Cosgrove (ryancosgrove@gmail.com)
  @version 1.0
\*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

/*removal definitions*/
#ifdef RM_ZLP_ALL
	#define RM_ZLP_VERBOSE
	#define RM_ZLP_WARNING
	#define RM_ZLP_INFO
	#define RM_ZLP_ERROR
	#define RM_ZLP_CRITICAL
#endif

/*VERBOSE*\--------------------------------------------------*/
#ifndef RM_ZLP_VERBOSE
	#define zlogv(fmt, ...) ZEN_LogFunc[ZLP_VERBOSE]( ZEN_LogFile[ZLP_VERBOSE], fmt, ##__VA_ARGS__)
	#define zclogv(chc, fmt1, fmt2, ...) ZEN_LogFunc[ZLP_VERBOSE]( ZEN_LogFile[ZLP_VERBOSE], (chc ? fmt1 : fmt2), ##__VA_ARGS__)
#else
	#define zlogv(fmt, ...) //
	#define zclogv(chc, fmt1, fmt2, ...) //
#endif

/*DEBUG*\----------------------------------------------------*/
#ifndef RM_ZLP_DEBUG
	#define zlogd(fmt, ...) ZEN_LogFunc[ZLP_DEBUG](ZEN_LogFile[ZLP_DEBUG], fmt, ##__VA_ARGS__)
	#define zclogd(chc, fmt1, fmt2, ...) ZEN_LogFunc[ZLP_DEBUG]( ZEN_LogFile[ZLP_DEBUG], (chc ? fmt1 : fmt2), ##__VA_ARGS__)
#else
	#define zlogd(fmt, ...) //
	#define zclogd(chc, fmt1, fmt2, ...) //
#endif

/*INFO*\-----------------------------------------------------*/
#ifndef RM_ZLP_INFO
	#define zlogi(fmt, ...) ZEN_LogFunc[ZLP_INFO](ZEN_LogFile[ZLP_INFO], fmt, ##__VA_ARGS__)
	#define zclogi(chc, fmt1, fmt2, ...) ZEN_LogFunc[ZLP_INFO]( ZEN_LogFile[ZLP_INFO], (chc ? fmt1 : fmt2), ##__VA_ARGS__)
#else
	#define zlogi(fmt, ...) //
	#define zclogi(chc, fmt1, fmt2, ...) //
#endif

/*WARNING*\--------------------------------------------------*/
#ifndef RM_ZLP_WARNING
	#define zlogw(fmt, ...) ZEN_LogFunc[ZLP_WARNING](ZEN_LogFile[ZLP_WARNING], fmt, ##__VA_ARGS__)
	#define zclogw(chc, fmt1, fmt2, ...) ZEN_LogFunc[ZLP_WARNING]( ZEN_LogFile[ZLP_WARNING], (chc ? fmt1 : fmt2), ##__VA_ARGS__)
#else
	#define zlogw(fmt, ...) //
	#define zclogw(chc, fmt1, fmt2, ...) //
#endif

/*ERROR*\----------------------------------------------------*/
#ifndef RM_ZLP_ERROR
	#define zloge(fmt, ...) ZEN_LogFunc[ZLP_ERROR](ZEN_LogFile[ZLP_ERROR], fmt, ##__VA_ARGS__)
	#define zcloge(chc, fmt1, fmt2, ...) ZEN_LogFunc[ZLP_ERROR]( ZEN_LogFile[ZLP_ERROR], (chc ? fmt1 : fmt2), ##__VA_ARGS__)
#else
	#define zloge(fmt, ...) //
	#define zcloge(chc, fmt1, fmt2, ...) //
#endif

/*CRITICAL*\--------------------------------------------------*/
#ifndef RM_ZLP_CRITICAL
	#define zlogc(fmt, ...) ZEN_LogFunc[ZLP_CRITICAL](ZEN_LogFile[ZLP_CRITICAL], fmt, ##__VA_ARGS__)
	#define zclogc(chc, fmt1, fmt2, ...) ZEN_LogFunc[ZLP_CRITICAL]( ZEN_LogFile[ZLP_CRITICAL], (chc ? fmt1 : fmt2), ##__VA_ARGS__)
#else
	#define zlogc(fmt, ...) //
	#define zclogc(chc, fmt1, fmt2, ...) //
#endif
