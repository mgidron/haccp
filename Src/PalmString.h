/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _PALMSTRING_
#define _PALMSTRING_

#include <size_t.h>
#include <MemoryMgr.h>


class CONST_CHAR_PTR
{
friend class PalmString;

public:
  operator const char*();
  CONST_CHAR_PTR(_opaque *void_hand);
  ~CONST_CHAR_PTR();

private:
  CONST_CHAR_PTR(const CONST_CHAR_PTR &);

//  void *operator new(std::size_t);

  _opaque *m_ptr;
  bool m_locked;
};

class PalmString
{
public:
  PalmString();
  PalmString(const PalmString &rhs);
  PalmString(const char *c);
  PalmString(float f);
  PalmString(long l);

  ~PalmString();

  PalmString & operator=(const PalmString &rhs);
  PalmString & operator=(const char *c);
  PalmString & operator=(float d);
  PalmString & operator=(short num);
  PalmString & operator=(unsigned short num);
  PalmString & operator=(int num);
  PalmString & operator=(unsigned int num);
  PalmString & operator=(long num);
  PalmString & operator=(unsigned long num);

  bool resize(unsigned long n);
  
  bool operator<(const PalmString & str) const;
  bool operator<=(const PalmString & str) const;
  bool operator>(const PalmString & str) const;
  bool operator>=(const PalmString & str) const;

  unsigned long length() const;
  bool IsEmpty( void ) const;
  bool operator==(const PalmString &rhs) const;
  bool operator!=(const PalmString &rhs) const;
  bool operator==(const char *rhs) const;
  bool operator!=(const char *rhs) const;
  char operator[]( int pos ) const;
  operator CONST_CHAR_PTR() const;
  CONST_CHAR_PTR c_str() const;
  void operator+=(const PalmString &rhs);
  void operator+=(const char *c);
  void operator+=(const char c);
  void append(const PalmString &rhs);
  void append(const char *c);
  bool IsSubString(PalmString& token) const;
  bool IsSubString( const char* token ) const;
  PalmString mid( PalmString& startToken, PalmString& endToken ) const;
  PalmString SubString(PalmString& token) const;
  PalmString substr(int index, int len) const;
  unsigned long _size() const;
  unsigned long size() const { return _size(); }
  inline _opaque * GetHandle() const;
  bool empty() const;
  void CopyToBuffer( char * buf ) const;
  int Find(const char c) const;
  void SetAt(int pos, const char c);
  void truncate(unsigned long length);
  void clear( void );

private:
  void** m_void_hand;
};

inline PalmString & PalmString::operator=(short num)
{
  return operator=(static_cast<long>(num));
}

inline PalmString & PalmString::operator=(unsigned short num)
{
  return operator=(static_cast<unsigned long>(num));
}

inline PalmString & PalmString::operator=(int num)
{
  return operator=(static_cast<long>(num));
}

inline PalmString & PalmString::operator=(unsigned int num)
{
  return operator=(static_cast<unsigned long>(num));
}

inline void PalmString::clear( void )
{
 operator =("");
}

inline bool PalmString::IsEmpty( void ) const
{
  return 0 >= length();
}

inline _opaque * PalmString::GetHandle() const
{
  return (_opaque*)m_void_hand;
}

#endif // _PALMSTRING_
