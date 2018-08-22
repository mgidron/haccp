/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "UICheckBox.h"

#include <PalmOS.h>
#include "stdtypes.h"

UICheckBox::UICheckBox(ButtonId button_id, FormId formid) :
    UIComponent(button_id, formid),
    m_ButtonId( button_id ),
    m_FormId( formid )
{
}

UICheckBox::~UICheckBox()
{
}

bool UICheckBox::IsChecked()
{
  return (1 == CtlGetValue ( (ControlPtr) GetObjectPtr() ));
}

void UICheckBox::SetCheck( bool b )
{
  CtlSetValue( (ControlPtr) GetObjectPtr(), false == b ? 0 : 1 );  // 0 turns check off, else turn check on
}

