/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#if !defined(_UIFORM_H_)
#define _UIFORM_H_

#include "stdtypes.h"
#include "FormManager.h"

#include <palmTypes.h>
#include <PalmOS.h>

class UIForm : protected FormManager
{
public:
  ~UIForm();

//  ApplicationManager & AppData( void );

protected:
  UIForm();
  UIForm(FormId id);

  virtual FormId NavigateTo( void ) = 0;

  void ShowForm(FormId parentId);
  void SetFieldTextFromString(const FieldId fldId, const char* charP);
  void SetFieldTextFromString(const FieldId fldId, PalmString &str);
  ObjectPtr GetObjectPtr( ObjectId objId );


private:
  FormId m_ParentForm;

};

#endif                                  // _UIFORM_H_
