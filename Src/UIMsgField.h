/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _UIMSGFIELD_H
#define _UIMSGFIELD_H

#include "UIField.h"

class UIMsgField : public UIField
{
public :
  UIMsgField(FieldId field_id, FormId formid);
  ~UIMsgField();

  void ShowMessage(PalmString & msg);
  void HideMessage( void );

private :
  PalmString m_strMessage;
};

#endif                                  // _UIMSGFIELD_H
