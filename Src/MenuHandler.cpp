/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "stdtypes.h"
#include "HACCP2002Rsc.h"
#include "MenuHandler.h"
#include "YesNoAlert.h"
#include "Document.h"   

////////////////////////////////////////////////////////////////////////////

MenuHandler::MenuHandler(FormId id) : m_handler_form_id(id)
{
}

////////////////////////////////////////////////////////////////////////////

FormId MenuHandler::HandleEvent(int command)
{
	FormId ret(false);

	switch(command)
	{ 		
		case OptionsActivateDemoMode:
			ret = OnActivateDemoMode();
			break;
		case OptionsAboutHACCP2004:
			ret = OnAboutHaccp2002();
			break;
		case OptionsExit:
			ret = OnExit();		    
			break;	  
		default:
			break;
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////

FormId MenuHandler::OnActivateDemoMode()
{
	if ( Document * pDoc = Document::GetInstance() )
	{
		if ( pDoc->IsDemoMode() == false)
		{
			pDoc->SetDemoMode(true);
			Trace("Demo Mode Activated.");
		}
		else
		{
			pDoc->SetDemoMode(false);
			Trace("Demo Mode Deactivated.");
		}
	}
	return FormId(0);
}

//////////////////////////////////////////////////////////////////////////////

FormId MenuHandler::OnAboutHaccp2002()
{
	return AboutForm;
}

//////////////////////////////////////////////////////////////////////////////

FormId MenuHandler::OnExit()
{
	EventType evt;
	evt.eType = appStopEvent;
	evt.penDown = true;
	evt.screenX = 0;
	evt.screenY = 0;
	EvtAddEventToQueue(&evt);
	
	return FormId(0);
}