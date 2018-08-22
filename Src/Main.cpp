/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "HACCP2002Rsc.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <PalmOS.h>
#include "Main.h"
#include "MenuHandler.h"
#include "PalmString.h"

Main * Main::_gMain = 0;

Main * Main::GetInstance()
{
	if ( _gMain == NULL )
	{
		_gMain = new Main;
	}
	return _gMain;
}

//---------------------------------------------------------------------------
Main::Main()   
{	
	m_fldCopyright = new UIField(MainCopyrightField, FORM_ID);

	m_lStartTime = 0;
}

//---------------------------------------------------------------------------
Main::~Main()
{
	if ( m_fldCopyright )
		delete m_fldCopyright;

	if ( _gMain )
		delete _gMain;

}

//---------------------------------------------------------------------------
FormId Main::NavigateTo( void )
{
	return LoginForm;
}

//---------------------------------------------------------------------------
void Main::SetStateOnFormOpen( void )
{   
}

//---------------------------------------------------------------------------
void Main::SetStateOnFormClose( void )
{   
}

//---------------------------------------------------------------------------
bool Main::OnFrmOpenEvent ( EventPtr eventP )
{
	m_lStartTime = TimGetSeconds();
	if ( FormPtr frmP = FrmGetActiveForm() )   
    {        
		FrmDrawForm(frmP);
	
		PalmString strLabel("");
		strLabel =  "            Patent Pending \n";
		strLabel += "       HACCP Solutions, Inc.\n";
		strLabel += "    www.haccpsolutions.com \n";

		m_fldCopyright->SetFieldTextFromString(strLabel.c_str());		
	}

	return true;
}

//---------------------------------------------------------------------------
bool Main::OnFrmCloseEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool Main::OnMenuEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool Main::OnFirstUserEvent ( EventPtr eventP )
{
	unsigned long lCurrentTime = TimGetSeconds();
	if ( lCurrentTime >= (m_lStartTime + 5) )
	{		
		Navigate();
	}
	return true;	
}

//---------------------------------------------------------------------------
Boolean MainFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP = NULL;

	Main * av = Main::GetInstance();

	switch (eventP->eType)
	{
		case menuEvent:
			av->OnMenuEvent(eventP);
			handled = true;
			break;
		case frmOpenEvent:
			av->OnFrmOpenEvent(eventP);
			handled = true;
			break;
		case frmCloseEvent:
			av->SetStateOnFormClose();
			handled = false;
			break;
		case nilEvent:
			av->OnFirstUserEvent( eventP );
			handled = true;
			break;
		case ctlSelectEvent:
			switch (eventP->data.ctlEnter.controlID)
			{
				//case MainOKButton :
				//	av->ReturnToForm();
				//	handled = true;
				//	break;
				default:
					break;
			}
			handled = true;
			break;
		default:
			break;
	}

	return handled;
}

//---------------------------------------------------------------------------