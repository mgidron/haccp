/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "UILabel.h"

UILabel::UILabel(UInt16 id, FormId formid) :
  UIComponent(id, formid)
{
}

UILabel::~UILabel()
{
}

void UILabel::SetLabelText( PalmString & str )
{
  SetLabelText(str.c_str());
}

//void UILabel::SetLabelText( const PalmString & str )
//{
//  SetLabelText(str.c_str());
//}

void UILabel::SetLabelText( const char * str )
{
  ControlPtr ctrl = static_cast<ControlPtr>(GetObjectPtr());
  int len = StrLen(str);
  if (len >= 0)
  {
    // allocate a buffer for the control to keep
    char * buf = new char[len + 1];
    if (buf != NULL)
    {
	  if ( len > 15 )
	      StrNCopy(buf, str, 15);
	  else
		  StrCopy(buf, str);	
      CtlSetLabel(ctrl, buf);
	  delete[] buf;
    }
  }
}

