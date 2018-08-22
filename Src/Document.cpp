/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "Document.h"
#include "HACCP2002Rsc.h"
#include "MemoPadWriter.h"

Document * Document::_gDocument = 0;

Document * Document::GetInstance()
{
	if ( _gDocument == NULL )
	{
		_gDocument = new Document;
	}
	return _gDocument;
}

////////////////////////////////////////////////////////////////////////////////

Document::Document()
{
	SetStageId(0);
	SetUserId(0);
	SetBtnId(0);
	SetBtnPos(0);
	SetBtnSelection("");
	SetLstSelection(0);
	SetTempature(0);
	SetTimestamp(0);
	SetNote("");
	SetOutOfTemp(false);
	SetDataInPort(false);
	SetManualEntry(false);
	SetDemoMode(false);

	m_pszBuffer = new char[80];
	MemSet(m_pszBuffer, 0, 80);

	LoadStateMachine();
}

////////////////////////////////////////////////////////////////////////////////

Document::~Document()
{
	SaveStateMachine();

	if ( m_pszBuffer )
	{
		delete[] m_pszBuffer;
	}

	if ( _gDocument )
	{
		delete _gDocument;
	}
}

////////////////////////////////////////////////////////////////////////////////

PalmString Document::GetCurrentTimeStamp( void )
{
	PalmString str("");

	DateTimeType dt;
	UInt32 lTimeSeconds = TimGetSeconds();
	TimSecondsToDateTime(lTimeSeconds,	&dt);
	
	char szTmp[40];
	MemSet(szTmp, 0, 40);

	// #1995-06-12 12:55:00#

	StrPrintF(szTmp, "%d-%d-%d %d:%d:%d", 
			dt.year, dt.month, dt.day,
			dt.hour, dt.minute, dt. second);

	str = szTmp;

	return str;
}

PalmString Document::GetHour( void )
{
	PalmString str("");

	DateTimeType dt;
	UInt32 lTimeSeconds = TimGetSeconds();
	TimSecondsToDateTime(lTimeSeconds,	&dt);
	
	char szTmp[40];
	MemSet(szTmp, 0, 40);

	// #1995-06-12 12:55:00#

	StrPrintF(szTmp, "%d-%d-%d %d:%d:%d", 
			dt.year, dt.month, dt.day,
			dt.hour, 00, 00);

	str = szTmp;

	return str;
}

////////////////////////////////////////////////////////////////////////////////

void Document::LoadStateMachine( void )
{
	MemoPadWriter db;

	if ( !db.Open('hcCP', 'stat') )
		return;

	db.ReadLine(0, m_pszBuffer, 80);

	SetUserId( StrAToI(db.m_parse_array[0]) );
	SetBtnId( StrAToI(db.m_parse_array[1]) );
	SetBtnPos( StrAToI(db.m_parse_array[2]) );
	SetBtnSelection( StrAToI(db.m_parse_array[3]) );
	SetLstSelection( StrAToI(db.m_parse_array[4]) );
	SetStageId( StrAToI(db.m_parse_array[5]) );

	db.Close();	
}

////////////////////////////////////////////////////////////////////////////////

void Document::SaveStateMachine( void )
{
	MemoPadWriter db;

	if ( !db.OpenOrCreateDB("StateMachine-hcCP", 'hcCP', 'stat') )
		return;

	char buf[50];
	StrCopy(buf, GetBtnSelection().c_str());
	StrPrintF(m_pszBuffer, "%ld|%d|%d|%s|%d|%ld|" , 
			  GetUserId(),
			  GetBtnId(),
			  GetBtnPos(),
			  buf,
			  GetLstSelection(),
			  GetStageId());
	db.WriteLine(0, m_pszBuffer);

	db.Close();
}

////////////////////////////////////////////////////////////////////////////////

bool Document::IsBatterPowerLow(PalmString & strMsg)
{
	bool rc = false;

	unsigned char pct(0);
	SysBatteryInfo(false, (unsigned short*)0, (unsigned short*)0, 
                        (short*)0, (SysBatteryKind*)0, (unsigned char*)0, &pct);

	if ( pct <= 10 )
	{
		rc = true;
		strMsg = "Low battery detected, please save information by syncing with your PC.";
	}
	
	return rc;
}

////////////////////////////////////////////////////////////////////////////////

int Document::ConvertTemp( char * pszBuffer )
{
	int rc = 0;

	if ( pszBuffer )
	{
		char ch[2];

		if ( StrAToI(pszBuffer) >= 10000 )
		{
			ch[0] = pszBuffer[3];
			ch[1] = '\0';
			pszBuffer[3] = '\0';
			rc = StrAToI(pszBuffer);
		}
		else if ( StrAToI(pszBuffer) >= 1000 )
		{
			ch[0] = pszBuffer[2];
			ch[1] = '\0';
			pszBuffer[2] = '\0';
			rc = StrAToI(pszBuffer);
		}
		else
		{
			ch[0] = pszBuffer[1];
			ch[1] = '\0';
			pszBuffer[1] = '\0';
			rc = StrAToI(pszBuffer);
		}

		int n = StrAToI(ch);
		if ( n > 4 )
			rc++;	
	}

	return rc;
}

////////////////////////////////////////////////////////////////////////////////