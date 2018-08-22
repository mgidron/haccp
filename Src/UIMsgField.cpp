/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "UIMsgField.h"

UIMsgField::UIMsgField(UInt16 id, FormId formid) : UIField(id, formid)
{
}


UIMsgField::~UIMsgField()
{
}

void UIMsgField::ShowMessage(PalmString & msg)
{
  m_strMessage = msg;

  ShowControl();
  SetFieldText( m_strMessage );
}

void UIMsgField::HideMessage( void )
{
  m_strMessage.empty();
}
