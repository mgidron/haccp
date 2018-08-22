/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef MemoPadWriter_H
#define MemoPadWriter_H

#include <PalmOS.h>
#include "PalmString.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


class MemoPadWriter
{
public:  // constructors
  
  MemoPadWriter(void);
  ~MemoPadWriter(void);

public:  // operations

  bool OpenOrCreateDB(const char * pName, UInt32 lCreator, UInt32 lType);
  bool Open(UInt32 lCreator, UInt32 lType);
  void Close(void);

  int GetNumRecords(void);
  unsigned long GetRecordSize(unsigned short index);

  bool WriteLine(unsigned short index, const char * str);
  // AppendLine - append a record onto the end of the list
  bool AppendLine(const char * str);
  int ReadLine(unsigned short index, char * str, unsigned int str_size);

  bool WriteBinary(unsigned short index, const char * buf, int buf_len);
  bool AppendBinary(const char * buf, int buf_len);
  int ReadBinary(unsigned short index, char * buf, int buf_size);
  
  bool DeleteLine(unsigned short index);
  void DeleteAllRecords(void);
  long StringSearch(const char * buf);
  void SortDatabase(void);

  char * m_parse_array[20];
  void ParseBuffer(const char * buf);
  bool FindInSortedDatabase(void * findRec, UInt32 * recordNumber);
  bool FindInSortedDatabaseField2(void * findRec, UInt32 * recordNumber);
  bool FindInSortedDatabaseQuery2(void * findRec, UInt32 * recordNumber); 

private:  // data

  DmOpenRef m_db_ref;
  char * m_buf;

};

static char szRc[20];

//-----------------------------------------------------------------------------

static const char * GetFirstIndex(const char * buf)
{
	if ( !buf )
		return NULL;
		
	MemSet(szRc, 0, 20);

	int i = 0;
	while ( *buf != '|' )
	{		
		szRc[i] = *buf;
		buf++;
		i++;
	}
	szRc[i] = 0;

	return &szRc[0];
}

//-----------------------------------------------------------------------------

static const char * GetSecondIndex(const char * buf)
{
	if ( !buf )
		return NULL;
		
	MemSet(szRc, 0, 20);

	char pBuffer[1024];
	StrCopy(pBuffer, StrChr(buf, '|'));
		
	int i = 0;
	int j = 1;
	while ( pBuffer[j] != '|' )
	{		
		szRc[i] = pBuffer[j];
		i++;
		j++;
	}
	szRc[i] = 0;

	return &szRc[0];
}

//-----------------------------------------------------------------------------

static short CompareIndexFunc(void * pRec1, void * pRec2, short i, 
	SortRecordInfoPtr s1, SortRecordInfoPtr s2, _opaque * appInfoH)
{	
	int rc = 0;
	if ( !pRec1  ||  !pRec2 )
		return 0;

	long lValue1 = StrAToI(GetFirstIndex((char*)pRec1));
	long lValue2 = StrAToI(GetFirstIndex((char*)pRec2));

	long iDiff = lValue1 - lValue2;

	if ( iDiff < 0)
	{
		rc = -1;
	}
	else if ( iDiff > 0)
	{
		rc = 1;
	}

	return (short)rc;
}

//-----------------------------------------------------------------------------

static short CompareDoubleIndexFunc(void * pRec1, void * pRec2, short i, 
	SortRecordInfoPtr s1, SortRecordInfoPtr s2, _opaque * appInfoH)
{	
	int rc = 0;
	if ( !pRec1  ||  !pRec2 )
		return 0;
	
	long lValue1 = StrAToI(GetFirstIndex((char*)pRec1));
	long lValue2 = StrAToI(GetFirstIndex((char*)pRec2));
	long lValue3 = StrAToI(GetSecondIndex((char*)pRec1));
	long lValue4 = StrAToI(GetSecondIndex((char*)pRec2));

	int iDiff = lValue1 - lValue2;
	int iDiff2 = lValue3 - lValue4;

	if ( (iDiff < 0) || (iDiff2 < 0) )
	{
		rc = -1;
	}
	else if ( (iDiff > 0) || (iDiff2 > 0) )
	{
		rc = 1;
	}	
	
	return (short)rc;
}

//-----------------------------------------------------------------------

static short CompareIndexFuncField2(void * pRec1, void * pRec2, short i, 
	SortRecordInfoPtr s1, SortRecordInfoPtr s2, _opaque * appInfoH)
{	
	int rc = 0;
	if ( !pRec1  ||  !pRec2 )
		return 0;

	long lValue1 = StrAToI(GetSecondIndex((char*)pRec1));
	long lValue2 = StrAToI(GetSecondIndex((char*)pRec2));

	int iDiff = lValue1 - lValue2;
	if ( iDiff < 0)
	{
		rc = -1;
	}
	else if ( iDiff > 0)
	{
		rc = 1;
	}

	return (short)rc;
}

#endif