/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _UIFIELD_H
#define _UIFIELD_H

#include "UIComponent.h"
#include "PalmString.h"

class UIField : public UIComponent
{
public:
  UIField(FieldId field_id, FormId formid);
  UIField(FieldId field_id);
  ~UIField();

  void ClearFieldString(const FormId frmId);
  void SetFieldTextFromString(const PalmString & str);
  void SetFieldTextFromString(const char * charP);
  void SetFieldText( long l );
  void SetFieldText(const PalmString & str);
  void SetFieldText(PalmString & str);
  void SetFieldText(const char *);
  void SetFieldText(char *);
  void SetFieldText(int num, const char *format = 0);
  void AppendTextToFieldText(const char * str);
  PalmString GetFieldText();
  Word GetFieldTextLength();
  Word GetFieldMaxTextLength();

  void SetMaxChars( unsigned max_chars );

  void SetLabel(PalmString & label);
  void SetLabel(const PalmString & label);
  void GetLabelText( PalmString  & str);

private:
  UIField();
};

#endif  // _UIFIELD_H
