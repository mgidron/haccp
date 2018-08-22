/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _UICHECKBOX_H
#define _UICHECKBOX_H

#include "stdTypes.H"
#include "UIComponent.h"
#include "PalmTypes.h"

class UICheckBox : public UIComponent
{
public:
  UICheckBox(CheckBoxId button_id, FormId formid);
  ~UICheckBox();

  bool IsChecked();
  void SetCheck( bool b );

private:
  UICheckBox();

  FormId    m_FormId;
  ButtonId  m_ButtonId;
};

#endif  // _UIFIELD_H