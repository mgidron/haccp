/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "MemoPadWriter.h"
#include "HACCP2002.h"
#include "YesNoAlert.h"


///////////////////////////////////////////////////////////////////////////////

MemoPadWriter::MemoPadWriter()
{
	m_db_ref = 0;

	m_buf = new char[1024];
	
	for (int i = 0; i < 20; i++)
	{
		m_parse_array[i] = new char[100];
	}
}

///////////////////////////////////////////////////////////////////////////////

MemoPadWriter::~MemoPadWriter()
{
//  Close();

  for (int i = 0; i < 20; i++)
  {
	if ( m_parse_array[i] )
	{
      delete[] m_parse_array[i];
	}
  }
  
  if ( m_buf )
	delete[] m_buf;
}

///////////////////////////////////////////////////////////////////////////////

// open a database. If it doesn't exist, create it.
bool MemoPadWriter::OpenOrCreateDB(const char * pName, UInt32 lCreator, UInt32 lType)
{  
  if ( !lCreator && !lType)
    return false;

  if ( Open(lCreator, lType) )
	return true;

  Err err;
  err = DmCreateDatabase(0, pName, lCreator, lType, false);
  if (err != errNone) 
    return false;
      
  if ( Open(lCreator, lType) )
	return true;

  return false;
}

///////////////////////////////////////////////////////////////////////////////

bool MemoPadWriter::Open(UInt32 lCreator, UInt32 lType)
{
  if ( !lCreator && !lType)
  	  return false;
  
  m_db_ref = DmOpenDatabaseByTypeCreator(lType, lCreator, dmModeReadWrite);
  return (m_db_ref != 0);
}

///////////////////////////////////////////////////////////////////////////////

void MemoPadWriter::Close(void)
{
  if (m_db_ref != 0)
  {
	  DmCloseDatabase(m_db_ref);
	  m_db_ref = 0;
  }
}

///////////////////////////////////////////////////////////////////////////////

int MemoPadWriter::GetNumRecords(void)
{
  if (m_db_ref != 0)
  {
    return DmNumRecords(m_db_ref);
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

unsigned long MemoPadWriter::GetRecordSize(unsigned short index)
{
  unsigned long result = 0;

  if (m_db_ref != 0  &&  index >= 0)
  {
	  // make sure index is within bounds
	  if (index < DmNumRecords(m_db_ref))
	  {
      // read the memo record
      _opaque * memo_rec = DmQueryRecord(m_db_ref, index);
	    if (memo_rec != NULL)
	    {
        result = MemHandleSize(memo_rec);
      }
    }
  }

  return result;
}

///////////////////////////////////////////////////////////////////////////////

bool MemoPadWriter::WriteLine(unsigned short index, const char * str)
{
  // write the string and the NULL terminator
  return WriteBinary(index, str, StrLen(str) + 1);
}

///////////////////////////////////////////////////////////////////////////////

bool MemoPadWriter::AppendLine(const char * str)
{
  // append the string and the NULL terminator
  return WriteBinary(dmMaxRecordIndex, str, StrLen(str) + 1);
}

///////////////////////////////////////////////////////////////////////////////

int MemoPadWriter::ReadLine(unsigned short index, char * str, unsigned int str_size)
{
  int result = -1;

  if (str != NULL  &&  str_size > 0)
  {
    // init the destination string
    str[0] = 0;

    if (ReadBinary(index, str, str_size) > 0)
    {
	    result = StrLen(str);
		ParseBuffer(str);
    }
  }

  return result;
}

///////////////////////////////////////////////////////////////////////////////

bool MemoPadWriter::WriteBinary(unsigned short index, const char * buf, int buf_len)
{
  bool is_success = false;


  if (m_db_ref != 0  &&  index >= 0  &&  buf != NULL  &&  buf_len > 0)
  {
	  // Add a record at the end of the database.
	  _opaque * memo_rec = DmNewRecord(m_db_ref, &index, buf_len);
	  if (memo_rec != NULL)
	  {
          // lock the memo record memory
	    void * ptr = MemHandleLock(memo_rec);
        if (ptr != NULL)
        {
		  // write the buffer to the record
		  DmWrite(ptr, 0, buf, buf_len); 
		}
        MemHandleUnlock(memo_rec);

        is_success = true;        
	  
	    // release the memo record
	    DmReleaseRecord(m_db_ref, index, true);
	  }
  }

  return is_success;
}

///////////////////////////////////////////////////////////////////////////////

bool MemoPadWriter::AppendBinary(const char * buf, int buf_len)
{
  return WriteBinary(dmMaxRecordIndex, buf, buf_len);
}

///////////////////////////////////////////////////////////////////////////////

int MemoPadWriter::ReadBinary(unsigned short index, char * buf, int buf_size)
{
  int result = -1;

  if (m_db_ref != 0  &&  index >= 0  &&  buf != NULL  &&  buf_size > 0)
  {
	  // make sure index is within bounds
	  if (index < DmNumRecords(m_db_ref))
	  {
        // read the memo record
        _opaque * memo_rec = DmQueryRecord(m_db_ref, index);
	    if (memo_rec != NULL)
	    {
	      // check the size of the record to see if it will fit in the buffer
          int num_bytes = static_cast<int>(MemHandleSize(memo_rec));
          if (num_bytes > 0  &&  num_bytes <= buf_size)
          {
            // lock the memo record memory
	        void * ptr = MemHandleLock(memo_rec);
            if (ptr != NULL)
            {
		       MemMove(buf, ptr, num_bytes);
            }

		    // unlock the memo record memory
	        MemHandleUnlock(memo_rec);

	        result = num_bytes;
          }
       }
    }
  }

  return result;
}

///////////////////////////////////////////////////////////////////////////////

bool MemoPadWriter::DeleteLine(unsigned short index)
{
  bool is_success = false;

  if (m_db_ref != 0)
  {
    if (index < DmNumRecords(m_db_ref))
    {
      if (DmRemoveRecord(m_db_ref, index) == 0)
      {
        is_success = true;
      }
    }
  }
  return is_success;
}

///////////////////////////////////////////////////////////////////////////////

void MemoPadWriter::DeleteAllRecords(void)
{
  if (m_db_ref != 0)
  {
    int i = DmNumRecords(m_db_ref);
    while (i > 0)
    {
      DmRemoveRecord(m_db_ref, i - 1);
      i--;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void MemoPadWriter::SortDatabase(void)
{
	if ( m_db_ref != 0 )
	{
		DmQuickSort(m_db_ref, CompareIndexFunc, 0);
	}
} 

////////////////////////////////////////////////////////////////////////////////

long MemoPadWriter::StringSearch(const char * buf)
{
  YesNoAlert oka(InformationAlertAlert);
  //oka.DoAlert(sztmp);

  UInt16 index = -1;
  UInt32 pos;
  UInt16 len;

  if (m_db_ref != 0)
  {
	bool match = false;
    while ( !match )
    {	  
	  index++;

	  _opaque * handle = DmQueryNextInCategory(m_db_ref, &index, dmAllCategories);
	  if ( !handle )
	  {
		index = 0;
	    break;
	  }

      int num_bytes = static_cast<int>(MemHandleSize(handle));
      void * ptr = MemHandleLock(handle);
      if (ptr != NULL)
	  {
	    MemMove(m_buf, ptr, num_bytes);	
	  }

	  // unlock the memo record memory
	  MemHandleUnlock(handle);
	  ParseBuffer(m_buf);

	  if ( TxtFindString(m_parse_array[1], buf, &pos, &len) )
	  {
		if (pos != 0)
		{
		  match = true;				
		  //oka.DoAlert("Find = true"); 
		}
	  }
	  else
	  {
		//oka.DoAlert("Find = false"); 
	  }
	}
  }
  return index;
}

////////////////////////////////////////////////////////////////////////////////

bool MemoPadWriter::FindInSortedDatabase(void * findRec, UInt32 * recordNumber)
{
	if ( !m_db_ref )
		return false;

	bool foundIt = false;
	
	long n = DmFindSortPosition(m_db_ref, findRec, NULL, CompareIndexFunc, 0);
	*recordNumber = n;

	if ( *recordNumber > 0)
	{
		MemHandle h;
		void *potentialMatchRec;

		h = DmQueryRecord(m_db_ref, *recordNumber - 1);
		potentialMatchRec = MemHandleLock(h);
		if ( (*CompareIndexFunc)(findRec, potentialMatchRec, 0, NULL, NULL, NULL) == 0)
		{
			foundIt = true;
			(*recordNumber)--;
		}
		MemHandleUnlock(h);
	}
	return foundIt;
}

////////////////////////////////////////////////////////////////////////////////

bool MemoPadWriter::FindInSortedDatabaseField2(void * findRec, UInt32 * recordNumber)
{
	if ( !m_db_ref )
		return false;

	bool foundIt = false;
	
	short n = DmFindSortPosition(m_db_ref, findRec, NULL, CompareIndexFuncField2, 0);
	*recordNumber = n;

	if ( *recordNumber > 0)
	{
		MemHandle h;
		void *potentialMatchRec;

		h = DmQueryRecord(m_db_ref, *recordNumber - 1);
		potentialMatchRec = MemHandleLock(h);
		if ( (*CompareIndexFuncField2)(findRec, potentialMatchRec, 0, NULL, NULL, NULL) == 0)
		{
			foundIt = true;
			(*recordNumber)--;
		}
		MemHandleUnlock(h);
	}
	return foundIt;
}

////////////////////////////////////////////////////////////////////////////////

bool MemoPadWriter::FindInSortedDatabaseQuery2(void * findRec, UInt32 * recordNumber)
{
	if ( !m_db_ref )
		return false;

	bool foundIt = false;
	
	short n = DmFindSortPosition(m_db_ref, findRec, NULL, CompareDoubleIndexFunc, 0);
	*recordNumber = n;

	if ( *recordNumber > 0)
	{
		MemHandle h;
		void *potentialMatchRec;

		h = DmQueryRecord(m_db_ref, *recordNumber - 1);
		potentialMatchRec = MemHandleLock(h);
		if ( (*CompareIndexFuncField2)(findRec, potentialMatchRec, 0, NULL, NULL, NULL) == 0)
		{
			foundIt = true;
			(*recordNumber)--;
		}
		MemHandleUnlock(h);
	}
	return foundIt;
}

////////////////////////////////////////////////////////////////////////////////

void MemoPadWriter::ParseBuffer(const char * buf)
{
	if ( !buf )
		return;
		
	for ( int idx = 0; idx < 20; idx++ )
	{
		StrCopy(m_parse_array[idx], "");
	}

	int i = 0;
	int j = 0;
	int k = 0;
	
	const long lLen = StrLen(buf);
	
	char pBuffer[512];
	StrCopy(pBuffer, buf);

	char sztmp[1024];
	MemSet(sztmp, 0, 1024);

	while ( k < lLen )
	{		
		if ( pBuffer[k] == '|' )
		{
			sztmp[i] = 0;
			StrCopy(m_parse_array[j], sztmp);
			MemSet(sztmp, 0, 1024);
			i = 0;
			j++;
		}
		else
		{
			sztmp[i] = pBuffer[k];
			i++;
		}	
		k++;
	}
}