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
#include "LoginForm.h"
#include "MenuHandler.h"
#include "PalmString.h"
#include "YesNoAlert.h"
#include "Document.h"
#include "MemoPadWriter.h"

Login * Login::_gLogin = 0;

Login * Login::GetInstance()
{
	if ( _gLogin == NULL )
	{
		_gLogin = new Login;
	}
	return _gLogin;
}

//---------------------------------------------------------------------------
Login::Login()   
{
	m_lstLogin     = new UIList(LoginUserList, FORM_ID);
	m_btnContinue  = new UIButton(LoginContinueButton, FORM_ID);

	for ( int i = 0; i < 8; i++ )
	{
		m_pBuf[i] = new char[101];
		StrCopy(m_pBuf[i], "");
	}		
}

//---------------------------------------------------------------------------
Login::~Login()
{
	if ( m_lstLogin )
		delete m_lstLogin;	
	
	if ( m_btnContinue )
		delete m_btnContinue;	

	for ( int i = 0; i < 8; i++ )
	{
		if ( m_pBuf[i] )
			delete[] m_pBuf[i];
	}

	if ( _gLogin )
		delete _gLogin;

}

//---------------------------------------------------------------------------
FormId Login::NavigateTo( void )
{
    return SelectionsForm;
}

//---------------------------------------------------------------------------
void Login::SetStateOnFormOpen( void )
{   
}

//---------------------------------------------------------------------------
void Login::SetStateOnFormClose( void )
{   
}

//---------------------------------------------------------------------------
bool Login::OnFrmOpenEvent ( EventPtr eventP )
{
	if ( FormPtr frmP = FrmGetActiveForm() )   
    {
        FrmDrawForm(frmP);
		FillUsersList();
	}

    return true;
}

//---------------------------------------------------------------------------
bool Login::OnFrmCloseEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool Login::OnMenuEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool Login::OnFirstUserEvent ( EventPtr eventP )
{
	return true;
}

//---------------------------------------------------------------------------
void Login::FillUsersList( void )
{
	MemoPadWriter db; 

	if ( !db.Open('hcCP', 'user') ) 
		return;

	long lMax = db.GetNumRecords();

	for (int i = 0; i < 8; i++)
	{
		if ( i < lMax )
		{
			db.ReadLine(i, p, 512);
			StrPrintF(m_pBuf[i], "%s %s", db.m_parse_array[1], db.m_parse_array[2]);
		}		
	}

	db.Close();
			  		
	m_lstLogin->AddItemsToList(m_pBuf, 8); 
	m_lstLogin->UpdateLoginList(); 	
}

//---------------------------------------------------------------------------
Boolean LoginFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP = NULL;


	Login * av = Login::GetInstance();

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
				case LoginContinueButton :
					MemoPadWriter db;		
					if ( db.Open('hcCP', 'user') ) 
					{
						int idx = av->m_lstLogin->GetSelectedIndex();
						if ( idx < 0 )
							idx = 0;
							
						db.ReadLine(idx, p, 512);
						Document::GetInstance()->SetUserId( StrAToI(db.m_parse_array[0]) );	
						db.Close();
					}

					if ( Document::GetInstance()->GetUserId() == 0 )
					{
						YesNoAlert oka(InformationAlertAlert);
						oka.DoAlert("Please select a user.");
					}
					else
						av->Navigate();
					handled = true;
					break;
				default:
					break;
			}
			handled = false;
			break;
		case lstSelectEvent:
			/*
			MemoPadWriter db;		
			if ( db.Open('hcCP', 'user') ) 
			{
				int idx = av->m_lstLogin->GetSelectedIndex();
				if ( idx < 0 )
					idx = 0;
					
				db.ReadLine(idx, p, 512);
				Document::GetInstance()->SetUserId( StrAToI(db.m_parse_array[0]) );	
				db.Close();
			}
			*/
			handled = false;
			break;
		default:
			break;
	}

	return handled;
}

//---------------------------------------------------------------------------