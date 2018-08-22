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
#include "SelectionsForm.h"
#include "MenuHandler.h"
#include "PalmString.h"
#include "MemoPadWriter.h"
#include "Document.h"
#include "UIList.h"
#include "YesNoAlert.h"

Selections * Selections::_gSelections = 0;

Selections * Selections::GetInstance()
{
	if ( _gSelections == NULL )
	{
		_gSelections = new Selections;
	}
	return _gSelections;
}

//---------------------------------------------------------------------------
Selections::Selections()   
{
	m_btnButton[0] = new UIPushButton(SelectionsReceivingPushButton, FORM_ID);
	m_btnButton[1] = new UIPushButton(SelectionsEquipmentPushButton, FORM_ID);
	m_btnButton[2] = new UIPushButton(SelectionsButton1PushButton, FORM_ID);
	m_btnButton[3] = new UIPushButton(SelectionsButton2PushButton, FORM_ID);
	m_btnButton[4] = new UIPushButton(SelectionsButton3PushButton, FORM_ID);
	m_btnButton[5] = new UIPushButton(SelectionsButton4PushButton, FORM_ID); 

	for (int i = 0;i < 6; i++)
	{
		m_lBtnId[i] = 0;
		m_pBuffer[i] = new char[40];
	}
}

//---------------------------------------------------------------------------
Selections::~Selections()
{
	for (int idx = 0;idx < 6; idx++ )
	{
		if ( m_btnButton[idx] )
			delete m_btnButton[idx];

		if ( m_pBuffer[idx] )
			delete[] m_pBuffer[idx];
	}

	if ( _gSelections )
		delete _gSelections;
}

//---------------------------------------------------------------------------
FormId Selections::NavigateTo( void )
{
	return m_ParentFormData.m_ChildForm;   
}

//---------------------------------------------------------------------------
void Selections::SetStateOnFormOpen( void )
{   
}

//---------------------------------------------------------------------------
void Selections::SetStateOnFormClose( void )
{   
}

//---------------------------------------------------------------------------
bool Selections::OnFrmOpenEvent ( EventPtr eventP )
{
	if ( FormPtr frmP = FrmGetActiveForm() )   
    {
        FrmDrawForm(frmP);
	}

	Document::GetInstance()->SetLstSelection(0);

	MemoPadWriter db;
	int BtnIdx = 0;

	if ( db.Open('hcCP', 'tfil') ) 
	{
		for (long i = 0;i < db.GetNumRecords(); i++)
		{
			
			db.ReadLine(i, p, 512);

			if ( i == 0 )        // read first location id
			{
				m_lBtnId[BtnIdx] = StrAToI(db.m_parse_array[3]);
				m_lStartPos[BtnIdx] = i;
				StrNCopy(m_pBuffer[BtnIdx], db.m_parse_array[4], 40);
				m_btnButton[BtnIdx]->SetLabel(m_pBuffer[BtnIdx]);				
			}
			else if ( (m_lBtnId[BtnIdx] != StrAToI(db.m_parse_array[3])) && (BtnIdx < 5) )
			{
				BtnIdx++;
				m_lBtnId[BtnIdx] = StrAToI(db.m_parse_array[3]);
				m_lStartPos[BtnIdx] = i;
				StrCopy(m_pBuffer[BtnIdx], db.m_parse_array[4]);
				m_btnButton[BtnIdx]->SetLabel(m_pBuffer[BtnIdx]);				
			}
			
		}

		if ( BtnIdx < 6 )
		{
			for( int idx = ++BtnIdx; idx < 6; idx++ )
			{
				m_btnButton[idx]->SetLabel("None");
				m_btnButton[idx]->Disable();
			}
		}

		db.Close();
	}	

    return true;
}

//---------------------------------------------------------------------------
bool Selections::OnFrmCloseEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool Selections::OnMenuEvent ( EventPtr eventP )
{
    SetMenuEvent();
    m_ParentFormData.m_ChildForm = MenuHandler( FORM_ID ).HandleEvent(eventP->data.menu.itemID);
    if ( m_ParentFormData.m_ChildForm != 0 )
		Navigate();
	return true;
}

//---------------------------------------------------------------------------
bool Selections::OnFirstUserEvent ( EventPtr eventP )
{
	return true;
}

//---------------------------------------------------------------------------
Boolean SelectionsFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP = NULL;

	Selections * av = Selections::GetInstance();
	PalmString str;

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
		case firstUserEvent :
			av->OnFirstUserEvent( eventP );
			handled = true;
			break;
		case ctlSelectEvent:
			switch (eventP->data.ctlEnter.controlID)
			{
				case SelectionsReceivingPushButton:
					av->m_ParentFormData.m_ChildForm = ItemsForm;   
					av->m_btnButton[0]->GetLabel(str);
					if ( Document * doc = Document::GetInstance() )
					{	
						doc->SetBtnId(av->m_lBtnId[0]);
						doc->SetBtnPos(0);
						doc->SetBtnSelection(str);
					}
					av->Navigate();
					break;
				case SelectionsEquipmentPushButton:
					av->m_ParentFormData.m_ChildForm = ItemsForm;
					av->m_btnButton[1]->GetLabel(str);
					if ( Document * doc = Document::GetInstance() )
					{
						doc->SetBtnId(av->m_lBtnId[1]);
						doc->SetBtnPos(1);
						doc->SetBtnSelection(str);
					}
					av->Navigate();
					break;
				case SelectionsButton1PushButton:
					av->m_ParentFormData.m_ChildForm = ItemsForm;
					av->m_btnButton[2]->GetLabel(str);
					if ( Document * doc = Document::GetInstance() )
					{
						doc->SetBtnId(av->m_lBtnId[2]);
						doc->SetBtnPos(2);
						doc->SetBtnSelection(str);
					}
					av->Navigate();
					break;
				case SelectionsButton2PushButton:
					av->m_ParentFormData.m_ChildForm = ItemsForm;
					av->m_btnButton[3]->GetLabel(str);
					if ( Document * doc = Document::GetInstance() )
					{
						doc->SetBtnId(av->m_lBtnId[3]);
						doc->SetBtnPos(3);
						doc->SetBtnSelection(str);
					}
					av->Navigate();
					break;
				case SelectionsButton3PushButton:
					av->m_ParentFormData.m_ChildForm = ItemsForm;
					av->m_btnButton[4]->GetLabel(str);
					if ( Document * doc = Document::GetInstance() )
					{
						doc->SetBtnId(av->m_lBtnId[4]);
						doc->SetBtnPos(4);
						doc->SetBtnSelection(str);
					}
					av->Navigate();
					break;				
				case SelectionsButton4PushButton:
					av->m_ParentFormData.m_ChildForm = ItemsForm;
					av->m_btnButton[5]->GetLabel(str);
					if ( Document * doc = Document::GetInstance() )
					{
						doc->SetBtnId(av->m_lBtnId[5]);
						doc->SetBtnPos(5);
						doc->SetBtnSelection(str);					
					}
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