/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/


// HACCP2002Main.c
//
// main file for HACCP2002
//
// This wizard-generated code is based on code adapted from the
// stationery files distributed as part of the Palm OS SDK 4.0.
//
// Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
// All rights reserved.

#include <PalmOS.h>

// Handspring SDK headers
#include <HSExt.h>

#include "HACCP2002.h"
#include "HACCP2002Rsc.h"

#include "MenuHandler.h"
#include "Main.h"
#include "AboutForm.h"
#include "SelectionsForm.h"
#include "ItemsForm.h"
#include "SeverityForm.h"
#include "LoginForm.h"
#include "UIList.h"
#include "Document.h"
#include "YesNoAlert.h"
#include "SerialPort.h"


// ********************************************************************
// Entry Points
// ********************************************************************

// ********************************************************************
// Global variables
// ********************************************************************

// g_prefs
// cache for application preferences during program execution
HACCP2002PreferenceType g_prefs;


// ********************************************************************
// Internal Constants
// ********************************************************************

// Define the minimum OS version we support
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS10Version sysMakeROMVersion(1,0,0,sysROMStageRelease,0)

// FUNCTION: AppHandleEvent
//
// DESCRIPTION: 
//
// This routine loads form resources and set the event handler for
// the form loaded.
//
// PARAMETERS:
//
// event
//     a pointer to an EventType structure
//
// RETURNED:
//     true if the event was handled and should not be passed
//     to a higher level handler.

static Boolean AppHandleEvent(EventType * eventP)
{
    UInt16 formId;
    FormType * frmP;

    if (eventP->eType == frmLoadEvent)
    {
        // Load the form resource.
        formId = eventP->data.frmLoad.formID;
        frmP = FrmInitForm(formId);
        FrmSetActiveForm(frmP);

        // Set the event handler for the form.  The handler of the 
        // currently active form is called by FrmHandleEvent each 
        // time is receives an event.
        switch (formId)
        {
	        case MainForm:
		        FrmSetEventHandler(frmP, MainFormHandleEvent);
			    break;
			case AboutForm:
		        FrmSetEventHandler(frmP, AboutFormHandleEvent);
			    break;
			case SelectionsForm:
		        FrmSetEventHandler(frmP, SelectionsFormHandleEvent);
			    break;
			case ItemsForm:
		        FrmSetEventHandler(frmP, ItemsFormHandleEvent);
			    break;
			case SeverityForm:
		        FrmSetEventHandler(frmP, SeverityFormHandleEvent);
			    break;
			case LoginForm:
				FrmSetEventHandler(frmP, LoginFormHandleEvent);
				break;
			default:
				break;
        }
        return true;
    }

    return false;
}

// FUNCTION: AppEventLoop
//
// DESCRIPTION: This routine is the event loop for the application.

static void AppEventLoop(void)
{
    UInt16 error;
    EventType event;

	do 
	{
		EvtGetEvent(&event, 2);   // evtWaitForever );

		if (! SysHandleEvent(&event))
		{
			if (! MenuHandleEvent(0, &event, &error))
			{
				if (! AppHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}
			}
		}
	}
	while (event.eType != appStopEvent);
}

// FUNCTION: AppStart
//
// DESCRIPTION:  Get the current application's preferences.
//
// RETURNED:
//     errNone - if nothing went wrong

static Err AppStart(void)
{
    UInt16 prefsSize;

    // Read the saved preferences / saved-state information.
    prefsSize = sizeof(HACCP2002PreferenceType);
    if (PrefGetAppPreferences(
        appFileCreator, appPrefID, &g_prefs, &prefsSize, true) != 
        noPreferenceFound)
    {
        // FIXME: setup g_prefs with default values
    }
    
    return errNone;
}

// FUNCTION: AppStop
//
// DESCRIPTION: Save the current state of the application.

static void AppStop(void)
{
    // Write the saved preferences / saved-state information.  This 
    // data will be saved during a HotSync backup.
    PrefSetAppPreferences(
        appFileCreator, appPrefID, appPrefVersionNum, 
        &g_prefs, sizeof(HACCP2002PreferenceType), true);

	// Close all the open forms.
    FrmCloseAllForms();

	if ( SerialPort * port = SerialPort::GetInstance() )
	{
		port->Close();
	}
}

// all code from here to end of file should use no global variables
#pragma warn_a5_access on

// FUNCTION: RomVersionCompatible
//
// DESCRIPTION: 
//
// This routine checks that a ROM version is meet your minimum 
// requirement.
//
// PARAMETERS:
//
// requiredVersion
//     minimum rom version required
//     (see sysFtrNumROMVersion in SystemMgr.h for format)
//
// launchFlags
//     flags that indicate if the application UI is initialized
//     These flags are one of the parameters to your app's PilotMain
//
// RETURNED:
//     error code or zero if ROM version is compatible

static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
    UInt32 romVersion;

    // See if we're on in minimum required version of the ROM or later.
    FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
    if (romVersion < requiredVersion)
    {
        if ((launchFlags & 
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
        {
            FrmAlert (RomIncompatibleAlert);

            // Palm OS 1.0 will continuously relaunch this app unless 
            // we switch to another safe one.
            if (romVersion <= kPalmOS10Version)
            {
                AppLaunchWithCommand(
                    sysFileCDefaultApp, 
                    sysAppLaunchCmdNormalLaunch, NULL);
            }
        }

		return sysErrRomIncompatible;
    }

    return errNone;
}

// FUNCTION: HACCP2002PalmMain
//
// DESCRIPTION: This is the main entry point for the application.
//
// PARAMETERS:
//
// cmd
//     word value specifying the launch code. 
//
// cmdPB
//     pointer to a structure that is associated with the launch code
//
// launchFlags
//     word value providing extra information about the launch
//
// RETURNED:
//     Result of launch, errNone if all went OK

static UInt32 HACCP2002PalmMain(
    UInt16 cmd, 
    MemPtr /*cmdPBP*/, 
    UInt16 launchFlags)
{
    Err error;

    error = RomVersionCompatible (ourMinVersion, launchFlags);
    if (error) return (error);

    switch (cmd)
    {
	    case sysAppLaunchCmdNormalLaunch:

			error = AppStart();
			if (error) 
				return error;

		    FrmGotoForm(MainForm);
			AppEventLoop();

            AppStop();
		    break;

		default:
			break;
    }

    return errNone;
}

// FUNCTION: PilotMain
//
// DESCRIPTION: This is the main entry point for the application.
// 
// PARAMETERS:
//
// cmd
//     word value specifying the launch code. 
//
// cmdPB
//     pointer to a structure that is associated with the launch code
//
// launchFlags
//     word value providing extra information about the launch.
//
// RETURNED:
//     Result of launch, errNone if all went OK

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
    return HACCP2002PalmMain(cmd, cmdPBP, launchFlags);
}

// turn a5 warning off to prevent it being set off by C++
// static initializer code generation
#pragma warn_a5_access reset
