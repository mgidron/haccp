/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _UIPUSHBUTTON_H
#define _UIPUSHBUTTON_H

#include "stdTypes.H"
#include "UIComponent.h"
#include "PalmTypes.h"
#include "PalmString.h"
#include "UIButton.h"

class UIPushButton : public UIButton
{
public:
  UIPushButton(ButtonId button_id, FormId formid);
  ~UIPushButton();

  void SetSelected(bool TurnOn = true);

private:
  UIPushButton();

//  FormId    m_FormId;
//  ButtonId  m_ButtonId;
};

#endif  // _UIPUSHBUTTON_H
