/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/



// HACCP2002.h
//
// header file for HACCP2002
//
// This wizard-generated code is based on code adapted from the
// stationery files distributed as part of the Palm OS SDK 4.0.
//
// Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
// All rights reserved.

#ifndef HACCP2002_H_
#define HACCP2002_H_

// ********************************************************************
// Internal Structures
// ********************************************************************

typedef struct HACCP2002PreferenceType
{
	UInt8 replaceme;
} HACCP2002PreferenceType;

// ********************************************************************
// Global variables
// ********************************************************************

extern HACCP2002PreferenceType g_prefs;

// ********************************************************************
// Internal Constants
// ********************************************************************

#define appFileCreator			'hcCP'
#define appName					"HACCP2002"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

// debug code



// ********************************************************************
// Helper template functions
// ********************************************************************

// use this template like:
// ControlType *button; GetObjectPtr(button, MainOKButton);

template <class T>
void GetObjectPtr(typename T * &ptr, UInt16 id)
{
	FormType * frmP;

	frmP = FrmGetActiveForm();
	ptr = static_cast<T *>(
		FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, id)));
}

// use this template like:
// ControlType *button = 
//     GetObjectPtr<ControlType>(MainOKButton);

template <class T>
typename T * GetObjectPtr(UInt16 id)
{
	FormType * frmP;

	frmP = FrmGetActiveForm();
	return static_cast<T *>(
		FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, id)));
}

#endif // HACCP2002_H_
