/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _UILABEL_H
#define _UILABEL_H

#include "UIComponent.h"
#include "PalmString.h"

class UILabel : public UIComponent
{
public:
  UILabel(FieldId field_id, FormId formid);
  ~UILabel();

  void SetLabelText( PalmString & str );
  //void SetLabelText( const PalmString & str );
  void SetLabelText( const char * str );

private:
  UILabel();
};

#endif  // _UILABEL_H
