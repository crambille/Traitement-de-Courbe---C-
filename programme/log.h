//==============================================================================
//
// Title:       log.h
// Purpose:     A short description of the interface.
//
// Created on:  08/04/2010 at 07:52:59 by Benoit Williot.
// Copyright:   JTEKT. All Rights Reserved.
//
//==============================================================================

#ifndef __log_H__
#define __log_H__

#ifdef __cplusplus
extern "C" {
#endif

	//==============================================================================
	// Include files

#include "cvidef.h"

	//==============================================================================
	// Constants

	//==============================================================================
	// Types
	typedef void ( *ptrLogFunc )( char *str );

	//==============================================================================
	// External variables

	//==============================================================================
	// Global functions

	void InitLog( int panel, int txtbox );
	void SetLogFunction( ptrLogFunc f );
	void EnableLog( void );
	void DisableLog( void );
	void LogClear( int save );
	void LogAddLine ( char *line );
	void LogAddLineConsole( char *line );
	void CopyLogBox( void );
	void lprintf( char *fmt, ... );
	const char *dateString( void ) ;

	void EnableLogCaching( void );
	void DisableLogCaching( void );
	unsigned int  GetCurrentTimeStamp( void );

#ifdef __cplusplus
}
#endif

#endif  /* ndef __log_H__ */
