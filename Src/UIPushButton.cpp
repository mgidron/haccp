/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "Utility.h"
#include "UIPushButton.h"

#include <PalmOS.h>
#include "stdtypes.h"
#include <string.h>

UIPushButton::UIPushButton(ButtonId button_id, FormId formid) :
    UIButton(button_id, formid)
//    m_ButtonId( button_id ),
//    m_FormId( formid )
{
}

UIPushButton::~UIPushButton()
{
}

void UIPushButton::SetSelected(bool TurnOn)
{
  CtlSetValue (TurnOn);
}

