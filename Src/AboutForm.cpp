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
#include "AboutForm.h"
#include "MenuHandler.h"
#include "PalmString.h"

About * About::_gAbout = 0;

About * About::GetInstance()
{
	if ( _gAbout == NULL )
	{
		_gAbout = new About;
	}
	return _gAbout;
}

//---------------------------------------------------------------------------
About::About()   
{
	m_fldCopyright = new UIField(AboutCopyrightField, FORM_ID);
}

//---------------------------------------------------------------------------
About::~About()
{
	if ( _gAbout )
		delete _gAbout;

	if ( m_fldCopyright )
		delete m_fldCopyright;	
}

//---------------------------------------------------------------------------
FormId About::NavigateTo( void )
{
    return SelectionsForm;
}

//---------------------------------------------------------------------------
void About::SetStateOnFormOpen( void )
{   
}

//---------------------------------------------------------------------------
void About::SetStateOnFormClose( void )
{   
}

//---------------------------------------------------------------------------
bool About::OnFrmOpenEvent ( EventPtr eventP )
{
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
bool About::OnFrmCloseEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool About::OnMenuEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool About::OnFirstUserEvent ( EventPtr eventP )
{
	return true;
}

//---------------------------------------------------------------------------
Boolean AboutFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP = NULL;

	About * av = About::GetInstance();

	switch (eventP->eType)
	{
		case menuEvent:
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
		case firstUserEvent :
			av->OnFirstUserEvent( eventP );
			handled = true;
			break;
		case ctlSelectEvent:
			switch (eventP->data.ctlEnter.controlID)
			{
				case AboutOKButton :
					av->Navigate();
					break;
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