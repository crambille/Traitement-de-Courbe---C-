//==============================================================================
//
// Title:       log.c
// Purpose:     A short description of the implementation.
//
// Created on:  08/04/2010 at 07:52:59 by Benoit Williot.
// Copyright:   JTEKT. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files


#include <windows.h>
#include <ansi_c.h>
#include <userint.h>
#include <utility.h>

#include "log.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables
static ptrLogFunc LogFunc;
static int totalLinesInserted = 0;
static int panelHandle;
static int box;

static char LineCache[512][1000];
static int CacheIndex = 0;
static int CacheEnable = 0;

CmtThreadLockHandle loglock;




//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions


void EnableLogCaching( void )
{
	CacheEnable = 1 ;
}

void LogCache( void )
{
	int topVisibleLine = 0;
	int numLinesInTextBox, TotalLines;
	CmtGetLock( loglock );

	for( int i = 0; i < CacheIndex; i++ )
	{
		InsertTextBoxLine ( panelHandle, box, -1, LineCache[i] );
	}

	GetCtrlAttribute ( panelHandle, box, ATTR_VISIBLE_LINES, &numLinesInTextBox );
	GetCtrlAttribute ( panelHandle, box, ATTR_TOTAL_LINES, &TotalLines );

	if( TotalLines >= numLinesInTextBox )
	{
		topVisibleLine = TotalLines - numLinesInTextBox + 1;
	}

	SetCtrlAttribute ( panelHandle, box, ATTR_FIRST_VISIBLE_LINE, topVisibleLine );
	CacheIndex = 0;
	CmtReleaseLock ( loglock );
}

void DisableLogCaching( void )
{
	CacheEnable = 0 ;

	if( CacheIndex > 0 )
	{
		LogCache();
	}
}

void CacheLine( char *line )
{
	CmtGetLock( loglock );
	strncpy( LineCache[CacheIndex], line, 512 );
	CacheIndex++;

	if( CacheIndex > 999 )
	{
		CacheIndex = 0;
	}

	CmtReleaseLock ( loglock );
}




const char *dateString( void )
{
	static char s[128];
	time_t t1;
	struct tm *t2;
	t1 = time( &t1 );
	t2 = localtime( &t1 );
	sprintf( s, "%02d%03d", t2->tm_year, t2->tm_yday );
	return s;
}


void SaveLogToFile( void )
{
	char fname[128];
	char *log;
	int len;
	FILE *f;
	sprintf( fname, "c:\\%s.mcu.log", dateString() );
	f = fopen ( fname, "a+" );

	if( f != NULL )
	{
		GetCtrlAttribute( panelHandle, box, ATTR_STRING_TEXT_LENGTH, &len );
		log = malloc( len + 1 );
		GetCtrlVal( panelHandle, box, log );
		fprintf( f, log );
		fclose( f );
		free( log );
	}
}


void LogToFile( char *line )
{
	char fname[128];
	FILE *f;
	sprintf( fname, "c:\\%s.mcu.log", dateString() );
	f = fopen ( fname, "a+" );

	if( f != NULL )
	{
		fprintf( f, line );
		fprintf( f, "\n" );
		fclose( f );
	}
}


void DummyLog( char *str )
{
}


void InitLog( int panel, int txtbox )
{
	if( panel == 0 && txtbox == 0 )
	{
		LogFunc = LogAddLineConsole;
	}
	else
	{
		panelHandle = panel;
		box = txtbox;
		DisableLog();
	}

	SetStdioWindowOptions ( 200, 0, 0 );
	CmtNewLock ( "LOG", OPT_TL_PROCESS_EVENTS_WHILE_WAITING, &loglock );
}

void SetLogFunction( ptrLogFunc f )
{
	if( f != NULL )
	{
		LogFunc = f;
	}
	else
	{
		LogFunc = DummyLog;
	}
}

void EnableLog( void )
{
	if( panelHandle == -1 || box == -1 )
	{
		LogFunc = LogAddLineConsole;
	}
	else
	{
		LogFunc = LogAddLine;
	}
}

void DisableLog( void )
{
	LogFunc = DummyLog;
}


void LogClear( int save )
{
	if( save != 0 )
	{
		SaveLogToFile();
	}

	ResetTextBox ( panelHandle, box, "" );
	totalLinesInserted = 0;
}




void LogAddLine ( char *line )
{
	int topVisibleLine = 0;
	int numLinesInTextBox, TotalLines;
	char str[2048];
	SYSTEMTIME t;
	GetLocalTime( &t );
	//Add a line to the Text Box and Scroll the TextBox to the bottom
	totalLinesInserted++;
	sprintf ( str, "%02d:%02d:%02d.%03d : %s", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds, line );

	if( CacheEnable == 1 )
	{
		CacheLine( str );
	}
	else
	{
		//LogToFile(str);
		InsertTextBoxLine ( panelHandle, box, -1, str );
		GetCtrlAttribute ( panelHandle, box, ATTR_VISIBLE_LINES, &numLinesInTextBox );
		GetCtrlAttribute ( panelHandle, box, ATTR_TOTAL_LINES, &TotalLines );

		if( TotalLines >= numLinesInTextBox )
		{
			topVisibleLine = TotalLines - numLinesInTextBox + 1;
		}

		SetCtrlAttribute ( panelHandle, box, ATTR_FIRST_VISIBLE_LINE, topVisibleLine );
		//ProcessDrawEvents();
	}
}



void LogAddLineConsole( char *line )
{
	printf( line );
	printf( "\n" );
}

void CopyLogBox( void )
{
	char *texte;
	int len;
	GetCtrlAttribute ( panelHandle, box, ATTR_STRING_TEXT_LENGTH, &len );
	texte = calloc( len + 1, sizeof( char ) );
	GetCtrlVal ( panelHandle, box, texte );
	ClipboardPutText ( texte );
	free( texte );
}

void lprintf( char *fmt, ... )
{
	va_list args;
	char buffer[1024];
	va_start( args, fmt );
	vsprintf( buffer, fmt, args );
	va_end( args );
	LogFunc( buffer );
}


unsigned int  GetCurrentTimeStamp( void )
{
	SYSTEMTIME t;
	GetLocalTime( &t );
	return t.wMilliseconds + t.wSecond * 1000 + t.wMinute * 60000;
}


