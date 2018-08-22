/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "HACCP2002Rsc.h"
#include "UIForm.h"

UIForm::UIForm()
{
}

UIForm::~UIForm()
{
}

void UIForm::ShowForm(FormId parentId)
{
  m_ParentForm = parentId;
}

void UIForm::SetFieldTextFromString(const FieldId fldId, PalmString &str)
{
  CONST_CHAR_PTR cptr(str.GetHandle()); // CONST_CHAR_PTR will keep the lock until it goes out of scope
  const char *p = (const char *)cptr;
  SetFieldTextFromString(fldId, p);
}

//********************************************************************************************8
//void SetFieldTextFromString(const FieldId fldid, TString str)
//{
//    SetFieldTextFromString(fldid, str.c_str());
//}

//********************************************************************************************8
void UIForm::SetFieldTextFromString(const FieldId fldid, const char* charP)
{
  FieldPtr fldP = (FieldPtr)GetObjectPtr( fldid );

  if( fldP )
  {
    Word len = StrLen(charP);
    MemHandle handle = MemHandleNew(len+1);

    MemHandle oldText = FldGetTextHandle(fldP);

    if( handle )
    {
      MemPtr vP = MemHandleLock( handle );
      if( vP )
      {
        StrCopy( (char*)vP, charP );

        FldSetTextHandle (fldP, handle);

        MemHandleUnlock( handle );
//            MemHandleFree( handle );
        if( oldText )
            MemHandleFree( oldText );
        FldDrawField(fldP);
      }
    }
  }
  return;

}

ObjectPtr UIForm::GetObjectPtr( ObjectId objId )
{
  // get the active form
  FormPtr formP = FrmGetActiveForm();
  if( NULL != formP )
  {
    // get the object index
    ObjectIndex objNdx = FrmGetObjectIndex (formP, objId);

    // get the object ptr
    ObjectPtr objP = FrmGetObjectPtr (formP, objNdx);
    return objP;
  }
  return NULL;
}