/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "UINumField.h"
#include "PalmString.h"

UINumField::UINumField(UInt16 id, FormId formid) : UIField(id, formid)
{
}

UINumField::~UINumField()
{
}

long UINumField::GetFieldTextAsLong()
{
  PalmString str = GetFieldText();

  if( str.empty() )
    return (long) 0;

  return StrAToI (str.c_str());
}

void UINumField::SetFieldTextAsLong(long txt)
{
  char buf[15];
  buf[0] = NULL;

  StrIToA (buf, txt);

  SetFieldTextFromString(buf);
}

