/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "UIField.h"
#include <string.h>

UIField::UIField(UInt16 id, FormId formid) : UIComponent(id, formid)
{
}

UIField::UIField(UInt16 id) : UIComponent(id)
{
}

UIField::~UIField()
{
}

void UIField::ClearFieldString(const FormId frmId)
{
  frmId; // to prevent warning

  FieldPtr fldP = static_cast<FieldPtr>(GetObjectPtr());

  if(fldP)
  {
    FldSetTextHandle (fldP, NULL);
  }
}

void UIField::SetLabel(PalmString & label)
{
  SetFieldTextFromString(label.c_str());
}

void UIField::SetLabel(const PalmString & label)
{
  SetFieldTextFromString(label.c_str());
}

void UIField::SetFieldTextFromString(const PalmString & str)
{
  SetFieldTextFromString(str.c_str());
}

void UIField::SetFieldTextFromString(const char* charP)
{
  FieldPtr fldP = static_cast<FieldPtr>(GetObjectPtr());
  Word len = StrLen(charP);
  MemHandle handle = MemHandleNew(len+1);

  if( fldP && handle )
  {
    MemHandle oldText = FldGetTextHandle(fldP);

    if( handle )
    {
      MemPtr vP = MemHandleLock( handle );
      if(vP)
      {
        StrCopy(static_cast<char*>(vP), charP);
        FldSetTextHandle(fldP, handle);

        MemHandleUnlock(handle);
        if(oldText)
          MemHandleFree(oldText);
        FldDrawField(fldP);
      }
    }
  }
}

void UIField::SetFieldText(const PalmString & str)
{
  SetFieldTextFromString( str.c_str() );
}

void UIField::SetFieldText(PalmString & str)
{
  SetFieldTextFromString( str.c_str() );
}

void UIField::SetFieldText( long l )
{
  char temp[10];

  StrIToA (temp, l);

  SetFieldTextFromString( temp );
}

void UIField::SetFieldText(const char *str)
{
  SetFieldTextFromString(str);
}

void UIField::SetFieldText(char *str)
{
  SetFieldTextFromString(str);
}


void UIField::SetFieldText(int num, const char *format)
{
  char c[50];
  if(format)
  {
    StrPrintF(c, format, num);
  }
  else
  {
    StrPrintF(c, "%d", num);
  }

  SetFieldTextFromString(c);
}

void UIField::AppendTextToFieldText(const char * str)
{
  PalmString strText = GetFieldText();
  strText += str;

  if(strText.length() < GetFieldMaxTextLength())
    SetFieldTextFromString(strText.c_str());
}


void UIField::GetLabelText( PalmString & label )
{
  label = GetFieldText();
}

PalmString UIField::GetFieldText()
{
  PalmString str;

  FieldPtr fldP = static_cast<FieldPtr>(GetObjectPtr());

  if( fldP )
  {
      CharPtr charP = FldGetTextPtr(fldP);
    if (charP != NULL)
    {
      str = charP;
    }
  }
    return str;
}


Word UIField::GetFieldTextLength()
{
  Word ret(0);

  FieldPtr fldP = static_cast<FieldPtr>(GetObjectPtr());

  if(fldP)
    ret = FldGetTextLength(fldP);
  else
    ret = -1;  // Todo: add error code

  return ret;
}

Word UIField::GetFieldMaxTextLength()
{
  Word ret(0);
  FieldPtr fldP = static_cast<FieldPtr>(GetObjectPtr());

  if(fldP)
    ret = FldGetMaxChars(fldP);
  else
    ret = -1;  // Todo: add error code

  return ret;
}

void UIField::SetMaxChars( unsigned max_chars )
{
  FieldPtr fp = (FieldPtr)GetObjectPtr();
  if(fp)
    FldSetMaxChars(fp, max_chars);
}