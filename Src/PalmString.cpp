/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include <FloatMgr.h>
#include <PalmOS.h>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include "PalmString.h"
#include <PalmCompatibility.h>


CONST_CHAR_PTR::CONST_CHAR_PTR(_opaque *void_hand)
{
  m_ptr = void_hand;
  m_locked = false;
}

CONST_CHAR_PTR::~CONST_CHAR_PTR()
{
  if (m_locked)
  {
    MemHandleUnlock(m_ptr);
  }
}

CONST_CHAR_PTR::operator const char*()
{
  if (!m_ptr)
  {
    return "";
  }

  m_locked = true;
  return static_cast<char*>(MemHandleLock(m_ptr));
}

PalmString::PalmString() : m_void_hand(0)
{
}

PalmString::~PalmString()
{
  if(m_void_hand)
  {
    MemHandleFree((_opaque*)m_void_hand);
  }
}

PalmString::PalmString(const PalmString &rhs) : m_void_hand(0)
{
  unsigned long len = rhs._size();
  if(len)
  {
    (_opaque*)m_void_hand = MemHandleNew(len);
    StrCopy((char*)MemHandleLock((_opaque*)m_void_hand), rhs.operator CONST_CHAR_PTR());
    MemHandleUnlock((_opaque*)m_void_hand);
  }
}

PalmString::PalmString(const char *c) : m_void_hand(0)
{
	if(c)
	{
	  unsigned long len = StrLen(c);
	  if(len)
	  {
	    (_opaque*)m_void_hand = MemHandleNew(len + 1);
	    StrCopy((char*)MemHandleLock((_opaque*)m_void_hand), c);
	    MemHandleUnlock((_opaque*)m_void_hand);
	  }
  }
}


PalmString::PalmString(float f) : m_void_hand(0)
{
  PalmString::operator=(f);
}

PalmString::PalmString(long l) : m_void_hand(0)
{
  PalmString::operator=(l);
}

bool PalmString::resize(unsigned long size)
{
  bool return_val = true;
  if(size)
  {
    if (m_void_hand == NULL)
    {
      (_opaque*)m_void_hand = MemHandleNew(size);
      MemSet((char*)MemHandleLock((_opaque*)m_void_hand), size, 0);
      MemHandleUnlock((_opaque*)m_void_hand);
    }
    else
    {
      if (MemHandleResize((_opaque*)m_void_hand, size) != 0)
      {
        return_val = false;
      }
    }
  }
  return return_val;
}

unsigned long PalmString::_size() const
{
  unsigned long ret(0);
  if(m_void_hand)
  {
    ret = MemHandleSize((_opaque*)m_void_hand);
  }

  return ret;
}

unsigned long PalmString::length() const
{
  unsigned long ret(0);

  if(m_void_hand)
  {
    ret = StrLen(c_str());
  }

  return ret;
}

bool PalmString::empty() const
{
  return 0 == length();
}

PalmString & PalmString::operator=(long num)
{
  char buf[20];

  StrIToA(buf, num);
  return operator=(buf);
}

PalmString & PalmString::operator=(unsigned long num)
{
  char buf[20];

  StrIToA(buf, num);
  return operator=(buf);
}

PalmString & PalmString::operator=(const char *c)
{
	if(c)
	{
	  if(m_void_hand)
	  {
	    MemHandleFree((_opaque*)m_void_hand);
	  }

	  unsigned long len = StrLen(c);
	  if(len)
	  {
	    (_opaque*)m_void_hand = MemHandleNew(len + 1);
	    StrCopy((char*)MemHandleLock((_opaque*)m_void_hand), c);
	    MemHandleUnlock((_opaque*)m_void_hand);
	  }
	  else
	  {
	    m_void_hand = 0;
	  }
  }
  else
  {
	  if(m_void_hand)
	  {
	    MemHandleFree((_opaque*)m_void_hand);
	    m_void_hand = 0;
	  }
  }

  return *this;
}

// this function assumes that no more than three digits will
// exist to the right of the decimal. This is true for lasers.
PalmString &PalmString::operator=(float d)
{
  char buf[30];


  long l = d * 1000;	// get rid of fraction

  StrPrintF(buf,"%04ld",l);  // need at least 4 digits

  int len = StrLen(buf); // assume len < 30 ** pretty safe assumption
  int pos;
  // shift last three digits to right to make room for decimal pt.
  for (pos = len; pos >= 0  &&  pos >= len - 3; pos--)
  {
    buf[pos + 1] = buf[pos];
  }

  buf[pos + 1] = '.';

  return operator=(PalmString(buf));
}

PalmString &PalmString::operator=(const PalmString &rhs)
{
  if(this != &rhs)
  {
    if(m_void_hand)
    {
      MemHandleFree((_opaque*)m_void_hand);
    }

    unsigned long len = rhs._size();
    if(len)
    {
      (_opaque*)m_void_hand = MemHandleNew(len);
      StrCopy((char*)MemHandleLock((_opaque*)m_void_hand), rhs.operator CONST_CHAR_PTR());
      MemHandleUnlock((_opaque*)m_void_hand);
    }
    else
    {
      m_void_hand = 0;
    }
  }

  return *this;
}

bool PalmString::operator<(const PalmString & str) const
{
  return (StrCompare(c_str(), str.c_str()) < 0);
}

bool PalmString::operator<=(const PalmString & str) const
{
  return (StrCompare(c_str(), str.c_str()) <= 0);
}

bool PalmString::operator>(const PalmString & str) const
{
  return (StrCompare(c_str(), str.c_str()) > 0);
}

bool PalmString::operator>=(const PalmString & str) const
{
  return (StrCompare(c_str(), str.c_str()) >= 0);
}

bool PalmString::operator==(const PalmString &rhs) const
{
  return 0 == StrCompare(c_str(), rhs.c_str());
//  return m_void_hand == rhs.m_void_hand;
}

bool PalmString::operator!=(const PalmString &rhs) const
{
  return 0 != StrCompare(c_str(), rhs.c_str());
//  return m_void_hand != rhs.m_void_hand;
}

bool PalmString::operator==(const char *rhs) const
{
  return 0 == StrCompare(c_str(), rhs);
}

bool PalmString::operator!=(const char *rhs) const
{
  return 0 != StrCompare(c_str(), rhs);
}

CONST_CHAR_PTR PalmString::c_str() const
{
  return CONST_CHAR_PTR((_opaque*)m_void_hand);
}

PalmString::operator CONST_CHAR_PTR() const
{
  return CONST_CHAR_PTR((_opaque*)m_void_hand);
}

char PalmString::operator[]( int pos ) const
{
  char ch = '\1';
  CONST_CHAR_PTR cptr(GetHandle()); // CONST_CHAR_PTR will keep the lock until it goes out of scope
  const char *p = (const char *)cptr;

  if( pos < length() )
    ch = p[pos];

  return ch;
}

void PalmString::operator+=(const char c)
{
  char buff[2];
  buff[0] = c;
  buff[1] = 0;
  append(buff);
}

void PalmString::operator+=(const char *c)
{
	if(c)
	{
	  unsigned long len = StrLen(c);
	  void **temp_hand(0);
	  if(len)
	  {
	    len += length();
	    (_opaque*)temp_hand = MemHandleNew(len + 1);
	    if(length())
	      StrPrintF((char*)MemHandleLock((_opaque*)temp_hand), "%s%s", (const char*)c_str(), c);
	    else
	      StrCopy((char*)MemHandleLock((_opaque*)temp_hand), c);


	    if(m_void_hand)
	    {
	      MemHandleFree((_opaque*)m_void_hand);
	    }
	    m_void_hand = temp_hand;
	    MemHandleUnlock((_opaque*)m_void_hand);
	  }
  }
}

void PalmString::operator+=(const PalmString &rhs)
{
  if(!rhs._size())
    return;

  unsigned long slen1  = _size();
  unsigned long slen2  = rhs._size();
  unsigned long len(0);

  if(slen1) // don't want 2 nulls
    len = slen1 + slen2 -1;
   else
    len = slen2;

  void ** temp_hand;
  (_opaque*)temp_hand = MemHandleNew(len);

  StrPrintF((char*)MemHandleLock((_opaque*)temp_hand), "%s%s",
             (const char*)c_str(), (const char*)rhs.c_str());

  if(m_void_hand)
  {
    MemHandleFree((_opaque*)m_void_hand);
  }

  m_void_hand = temp_hand;
  MemHandleUnlock((_opaque*)m_void_hand);
}

void PalmString::append(const char *c)
{
  append(PalmString(c));
}

void PalmString::append(const PalmString &rhs)
{
  operator+=(rhs);
}

bool PalmString::IsSubString(PalmString& token) const
{
    if( StrStr( c_str(), token.c_str() ))
        return true;
    else
        return false;
}

bool PalmString::IsSubString( const char* token ) const
{
    if( StrStr( c_str(), token ))
        return true;
    else
        return false;

}

PalmString PalmString::SubString(PalmString& token) const
{
    PalmString str;

    if( IsSubString( token) )
    {
        str = StrStr( c_str(), token.c_str() );
    }
    return str;
}

PalmString PalmString::substr(int index, int len) const
{
  PalmString sub_str;

  if (index >= 0  &&  len > 0)
  {
    CONST_CHAR_PTR cptr(GetHandle()); // CONST_CHAR_PTR will keep the lock until it goes out of scope
    const char * str = (const char *) cptr;
    if (str != NULL)
    {
      int max_len = StrLen(str);
      if (index + len > max_len)
      {
        // adjust len to the maximum available
        len = max_len - index;
      }
      if (len > 0)
      {
        char * temp = new char[len + 1];
        if (temp != NULL)
        {
          StrNCopy(temp, str, len);
          temp[len] = 0;
          sub_str = temp;

          delete [] temp;
        }
      }
    }
  }

  return sub_str;
}

PalmString PalmString::mid( PalmString& startToken, PalmString& endToken ) const
{
    PalmString str;

    if( IsSubString( startToken ))
    {
        const char* cpStart = SubString(startToken).c_str();  // get the location of the start of the substring
        const char* cpEnd = StrStr( c_str(), endToken.c_str() );

        // is the end token present
        if( NULL == cpEnd )
        {
            // end token not present to copy entire string
            str = cpStart;
        }
        else                            // token is present to copy from token
        {
            while( cpStart != cpEnd && NULL != *cpStart && str.length() < StrLen(cpEnd))
            {
                str += *cpStart;
                cpStart++;
            }
        }
    }

    return str;
}

void PalmString::CopyToBuffer( char * buf ) const
{
  CONST_CHAR_PTR cptr(GetHandle()); // CONST_CHAR_PTR will keep the lock until it goes out of scope
  const char *p = (const char *)cptr;
  int len = length() + 1;

  if( p && 0 < length() )
  {
    for( int ndx = 0; ndx < len; ndx ++ )
    {
      buf[ndx] = p[ndx];
    }
  }
}

int PalmString::Find(const char c) const
{
  int i(0);

  CONST_CHAR_PTR cptr(GetHandle()); // CONST_CHAR_PTR will keep the lock until it goes out of scope
  const char *p = (const char *)cptr;
  while(*p)
  {
    if(c == *p)
      break;
    i++;
    p++;
  }

  return *p ? i : -1;
}

void PalmString::SetAt(int pos, const char c)
{
  int i;

  CONST_CHAR_PTR cptr(GetHandle()); // CONST_CHAR_PTR will keep the lock until it goes out of scope
  char *p = const_cast<char*>((const char *)cptr);  // cast away constness

  for(i = 0; i < pos; i++)
  {
    if(!*p)
      return;
    p++;
  }

  *p = c;
}

void PalmString::truncate(const unsigned long length)
{
  const unsigned long this_length = this->length();

  if(!this_length || this_length < length)
    return;

  if(this_length == length)
  {
    MemHandleFree((_opaque*)m_void_hand);
    m_void_hand = 0;
    return;
  }

  void **temp_hand = m_void_hand;
  (_opaque*)m_void_hand = MemHandleNew(this_length - length + 1);
  MemSet((char*)MemHandleLock((_opaque*)m_void_hand), this_length - length + 1, 0);
  StrNCopy((char*)MemHandleLock((_opaque*)m_void_hand), (char*)MemHandleLock((_opaque*)temp_hand), this_length - length);
  MemHandleUnlock((_opaque*)m_void_hand);
  MemHandleUnlock((_opaque*)temp_hand);
  MemHandleFree((_opaque*)temp_hand);
}