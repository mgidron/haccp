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
#include "ItemsForm.h"
#include "MenuHandler.h"
#include "PalmString.h"
#include "Document.h"
#include "SelectionsForm.h"
#include "YesNoAlert.h"
#include "SerialPort.h"

Items * Items::_gItems = 0;

Items * Items::GetInstance()
{
	if ( _gItems == NULL )
	{
		_gItems = new Items;
	}
	return _gItems;
}

//---------------------------------------------------------------------------
Items::Items()   
{    
	m_lstFood	  = new UIList(ItemsFoodList, FORM_ID);
	m_btnLocation = new UIButton(ItemsLocationButton, FORM_ID);
	m_fldHidden   = new UIField(ItemsHiddenField, FORM_ID);

	for ( int i = 0; i < 10; i++ )
	{
		m_pBuf[i] = new char[101];	
	}

	sztmp = new char[256];
	m_pButtonText = new char[30];
	m_db = new MemoPadWriter;

	if ( SerialPort * port = SerialPort::GetInstance() )
	{
		port->Open();
	}
}

//---------------------------------------------------------------------------

Items::~Items()
{
	if ( m_lstFood )
		delete m_lstFood;

	if ( m_btnLocation )
		delete m_btnLocation;

	if ( m_fldHidden )
		delete m_fldHidden;

	for ( int i = 0; i < 10 ; i++ )
	{
		if ( m_pBuf[i] )
			delete[] m_pBuf[i];	
	}	

	if (sztmp)
	{
		delete[] sztmp;
	}

	if (m_pButtonText)
		delete[] m_pButtonText;

	if ( m_db )
	{
		delete m_db;
	}

	if ( _gItems )
	{
		delete _gItems;
	}
}

//---------------------------------------------------------------------------

FormId Items::NavigateTo( void )
{
	Document * doc = Document::GetInstance();
	doc->SetLstSelection(m_lstFood->GetSelectedIndex());

	if ( m_ParentFormData.m_ChildForm == SelectionsForm )
	{
		ResetHiddenFields( Selections::GetInstance()->m_lStartPos[doc->GetBtnPos()] );
	}

    return m_ParentFormData.m_ChildForm;
}

//---------------------------------------------------------------------------
void Items::SetStateOnFormOpen( void )
{   
}

//---------------------------------------------------------------------------
void Items::SetStateOnFormClose( void )
{   
}

//---------------------------------------------------------------------------
bool Items::OnFrmOpenEvent ( EventPtr eventP )
{
	if ( FormPtr frmP = FrmGetActiveForm() )   
    {
        FrmDrawForm(frmP);		
	}

	Document::GetInstance()->SetOutOfTemp(false);
	Document::GetInstance()->SetManualEntry(false);

	m_fldHidden->SetFieldTextFromString("");

	PalmString str = Document::GetInstance()->GetBtnSelection();
	m_btnLocation->SetLabel( str.substr(0, 16).c_str() );
	
	m_btnLocation->Disable();

	m_lstFood->UpdateList(); 
	m_lstFood->Draw();

	m_lstFood->ListSetSelection(Document::GetInstance()->GetLstSelection());

	return true;
}

//---------------------------------------------------------------------------
bool Items::OnFrmCloseEvent ( EventPtr eventP )
{	
	return true;
}

//---------------------------------------------------------------------------
bool Items::OnMenuEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool Items::OnFirstUserEvent ( EventPtr eventP)
{
	Document * doc = Document::GetInstance();
	if ( !doc )
		return false;

	SerialPort * port = SerialPort::GetInstance();
	if ( !port )
		return false;
	
	char buf[100];
	MemSet(buf, 0, 100);

	if ( doc->IsDataInPort() || (m_fldHidden->GetFieldText() != ""))
	{			
		doc->SetDataInPort(false);

		int size = port->PalmRead(buf, 100);

		if ( (size == 12) || (m_fldHidden->GetFieldText() != "") )
		{
			if ( m_fldHidden->GetFieldText() != "" )
			{
				StrNCopy(buf, m_fldHidden->GetFieldText().c_str(), 6);
				doc->SetTempature( StrAToI(buf) );				
			}
			else
			{
 			    doc->SetTempature( doc->ConvertTemp(buf) );
			}
			int temp = doc->GetTempature();
				
			if ( !m_db->Open('hcCP', 'tfil') ) 
				return 0;

			long pos = Selections::GetInstance()->m_lStartPos[doc->GetBtnPos()];
			pos += doc->GetLstSelection();

			m_db->ReadLine(pos, p, 512);
			int food_id = StrAToI(m_db->m_parse_array[1]);
			m_db->Close();

			SaveTempReading( food_id, pos );

			if ( (temp >= 42) && (temp <= 139) )
			{
				doc->SetOutOfTemp(true);				
				doc->SetManualEntry(true);
				m_ParentFormData.m_ChildForm = SeverityForm;				
				Navigate();
			}
			
			if ( doc->GetLstSelection() == (m_lstFood->GetListCount()-1) )
			{
				m_ParentFormData.m_ChildForm = SelectionsForm;				
				Navigate();
			}	
			
			m_fldHidden->SetFieldTextFromString("");

			// set next selection
			int iSel = doc->GetLstSelection();
			iSel++;
			doc->SetLstSelection(iSel);

			m_fldHidden->SetFieldTextFromString("");
			
			// update list control
			m_lstFood->Draw();
			m_lstFood->ListSetSelection(Document::GetInstance()->GetLstSelection());				
		}
	}

	return true;
}

//---------------------------------------------------------------------------

void Items::SaveTempReading( int food_id, long idx, bool bResave )
{
	Document * pDoc = Document::GetInstance();
	if ( !pDoc )
		return;
		
	if ( !m_db->OpenOrCreateDB("Stage-hcCP", 'hcCP', 'stag') )
		return;

	/*
	Make a line of data to save
	*/

	int stage = pDoc->GetStageId();
	int user = pDoc->GetUserId();
	int location = pDoc->GetBtnId();
	char temp[30];
	StrPrintF(temp, "%d", pDoc->GetTempature());
	char time[30];
	char hour[30];
	StrNCopy(time, pDoc->GetCurrentTimeStamp().c_str(), 30);

	StrNCopy(hour, pDoc->GetHour().c_str(), 30);

	//unsigned long time = pDoc->GetTimestamp();
	StrCopy(sztmp, pDoc->GetNote().c_str());

	/*
	End of Make a line of data to save
	*/

	StrPrintF(p, "%d|%d|%d|%d|%s|%s|%s|%s|",
			stage, user, location,
			food_id, hour, time, temp, sztmp);

	if ( pDoc->IsDemoMode() == false )
	{
		if ( bResave )
			m_db->DeleteLine( (unsigned short)pDoc->GetStageId() );

		m_db->WriteBinary((unsigned short)pDoc->GetStageId(), p, 512);
	}
	m_db->Close();	

	
	/********   Update hidden field*/

	if ( !m_db->Open('hcCP', 'tfil') ) 
		return;

	m_db->ReadLine(idx, p, 512);
	StrPrintF(m_db->m_parse_array[5], "%d", pDoc->GetTempature());

	if ( !pDoc->IsOutOfTemp() )
		StrCopy(m_db->m_parse_array[6], "none");
	else
		StrCopy(m_db->m_parse_array[6], " yes");

	StrPrintF(m_db->m_parse_array[7], "%d", stage);

	// make the string
	StrPrintF(p, "%s|%s|%s|%s|%s|%s|%s|%s|", 
				m_db->m_parse_array[0],
				m_db->m_parse_array[1],
				m_db->m_parse_array[2],
				m_db->m_parse_array[3],
				m_db->m_parse_array[4],
				m_db->m_parse_array[5],
				m_db->m_parse_array[6],
				m_db->m_parse_array[7]);

	m_db->DeleteLine(idx);
	m_db->WriteBinary(idx, p, 512);
	m_db->Close();	

	// iterage stage id
	stage++;
	pDoc->SetStageId(stage);

	pDoc->SetTimestamp(0);
	pDoc->SetNote("");
	pDoc->SetTempature(0);
}

//---------------------------------------------------------------------------
void Items::ResetHiddenFields( int idx )
{
	if ( idx < 0 )
		return;

	if ( !m_db->Open('hcCP', 'tfil') ) 
		return;

	long lMax = m_db->GetNumRecords();

	for (int i = idx; i < lMax; i++)
	{
		m_db->ReadLine(i, p, 512);

		StrCopy(m_db->m_parse_array[5], " ");
		StrCopy(m_db->m_parse_array[6], " ");
		StrCopy(m_db->m_parse_array[7], " ");

		// make the string
		StrPrintF(p, "%s|%s|%s|%s|%s|%s|%s|%s|", 
					m_db->m_parse_array[0],
					m_db->m_parse_array[1],
					m_db->m_parse_array[2],
					m_db->m_parse_array[3],
					m_db->m_parse_array[4],
					m_db->m_parse_array[5],
					m_db->m_parse_array[6],
					m_db->m_parse_array[7]);

		m_db->DeleteLine(i);
		m_db->WriteBinary(i, p, 512);
	}

	m_db->Close();
}

//---------------------------------------------------------------------------
Boolean ItemsFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP = NULL;

	Items * av = Items::GetInstance();

	Document * doc = Document::GetInstance();
	PalmString strMsg("");

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
		case nilEvent :
			if ( doc->IsBatterPowerLow(strMsg) )
			{
				YesNoAlert Alert(InformationAlertAlert);
				Alert.DoAlert(strMsg.c_str());	
			}

			av->OnFirstUserEvent( eventP );
			handled = true;
			break;
		case ctlSelectEvent:
			switch (eventP->data.ctlEnter.controlID)
			{
				case ItemsBackGraphicButton :
					av->m_ParentFormData.m_ChildForm = SelectionsForm;
					av->Navigate();
					break;
				default:
					break;
			}
			handled = true;
			break;
		case lstSelectEvent:
			doc->SetLstSelection(av->m_lstFood->GetSelectedIndex());
			//av->m_ParentFormData.m_ChildForm = SeverityForm;
			//av->Navigate();
			frmP = FrmGetActiveForm(); 
			UInt16 id = av->m_fldHidden->GetObjectIndex();
			FrmSetFocus(frmP, id);			
			SysKeyboardDialog(kbdNumbersAndPunc);
			handled = false;
			break;
		case fldEnterEvent :
			SysKeyboardDialog(kbdNumbersAndPunc);
			handled = false;
			break;
		default:
			break;
	}	

	return handled;
}

//---------------------------------------------------------------------------