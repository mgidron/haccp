/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _STDTYPES_H
#define _STDTYPES_H

#ifndef __PALMOS_H__
#include <PalmOS.h>
#endif

// forms
typedef UInt16 FormId;
typedef FormPtr FormP;

// form objects
typedef UInt16 FieldId;
typedef UInt16 ListId;

typedef FieldPtr FieldP;
typedef ListPtr ListP;

// form controls
typedef UInt16 ControlId;
typedef UInt16 ComponentId;
typedef UInt16 ButtonId;
typedef UInt16 CheckBoxId;
typedef UInt16 BitmapId;

//
typedef MemPtr ObjectPtr;
typedef UInt16 ObjectId;

typedef UInt16 ObjectIndex;

// handles
typedef MemHandle TextHandle;

// types
typedef UInt16 UINT;
typedef UInt16 Word;
typedef char* CharPtr;



#endif // _STDTYPES_H
