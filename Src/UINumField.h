/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _UINUMFIELD_H
#define _UINUMFIELD_H

#include "UIField.h"

class UINumField : public UIField
{
public:
  UINumField(FieldId field_id, FormId formid);
  ~UINumField();

  long GetFieldTextAsLong();
  void SetFieldTextAsLong(long txt);

private:
  UINumField();
};

#endif  // _UIFIELD_H
