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
#include "SeverityForm.h"
#include "MenuHandler.h"
#include "PalmString.h"
#include "Document.h"
#include "MemoPadWriter.h"
#include "SelectionsForm.h"
#include "ItemsForm.h"

Severity * Severity::_gSeverity = 0;

Severity * Severity::GetInstance()
{
	if ( _gSeverity == NULL )
	{
		_gSeverity = new Severity;
	}
	return _gSeverity;
}

//---------------------------------------------------------------------------
Severity::Severity()   
{
	m_fldTemp = new UIField(SeverityTempField, FORM_ID);
	m_fldDegrees = new UIField(SeverityDegreesField, FORM_ID);	
	m_btnFood = new UIButton(SeverityFoodButton, FORM_ID);
	m_Popup   = new UIComponent(SeverityNotePopTrigger, FORM_ID);

	m_strArray[0] = "Reheated to 140 degrees";
	m_strArray[1] = "Reheated to 165 degrees";
	m_strArray[2] = "Reheated to 180 degrees";
	m_strArray[3] = "Placed in ice bath cooled to 40 degrees";
	m_strArray[4] = "Discarded Product";
	m_strArray[5] = "Replaced product with new batch";
	m_strArray[6] = "Bad reading, Altered to correct reading";

	m_strFood.clear();
}

//---------------------------------------------------------------------------
Severity::~Severity()
{
	if ( m_fldTemp )
		delete m_fldTemp;

	if ( m_fldDegrees )
		delete m_fldDegrees;

	if ( m_btnFood )
		delete m_btnFood;

	if ( m_Popup )
		delete m_Popup;

	if ( _gSeverity )
		delete _gSeverity;	
}

//---------------------------------------------------------------------------
FormId Severity::NavigateTo( void )
{
    return ItemsForm;
}

//---------------------------------------------------------------------------
void Severity::SetStateOnFormOpen( void )
{   
}

//---------------------------------------------------------------------------
void Severity::SetStateOnFormClose( void )
{   
}

//---------------------------------------------------------------------------
bool Severity::OnFrmOpenEvent ( EventPtr eventP )
{
	FormPtr frmP = FrmGetActiveForm();

	if ( frmP )   
    {
        FrmDrawForm(frmP);
	}

	m_fldDegrees->SetFieldTextFromString("\xb0 F");
	
	Document * doc = Document::GetInstance();
	if ( !doc )
		return true;

	doc->SetTimestamp(0);
	doc->SetNote("");
	doc->SetTempature(0);

	int selection = 0;
	int idx = 0;
	
	if ( !doc->IsManualEntry() )
	{
		selection = Selections::GetInstance()->m_lStartPos[doc->GetBtnPos()];
		selection += doc->GetLstSelection();//doc->GetLstSelection() - 1;
	}
	else
	{
		selection = Selections::GetInstance()->m_lStartPos[doc->GetBtnPos()];
		selection += doc->GetLstSelection();//doc->GetLstSelection();
	}

	selection--;

	MemoPadWriter db;

	if ( db.Open('hcCP', 'tfil') ) 
	{
		db.ReadLine(selection, p, 512);
		idx = StrAToI(db.m_parse_array[8]);
		db.Close();
	}	
	
	if ( db.Open('hcCP', 'stag') )
	{
		idx = db.GetNumRecords() - 1;				/* new line */
		db.ReadLine(idx, p, 512);
		doc->SetTempature(StrAToI(db.m_parse_array[6]));
		doc->SetNote(db.m_parse_array[7]);
		db.Close();
	}

	if ( db.Open('hcCP', 'tfil') ) 
	{
		int pos = Selections::GetInstance()->m_lStartPos[doc->GetBtnPos()];

		if ( !doc->IsManualEntry() )
			pos += doc->GetLstSelection();
		else
			pos += doc->GetLstSelection() - 1;
		
		db.ReadLine(pos, p, 512);

		ControlType * ctlP = NULL;
		PalmString str(db.m_parse_array[1]);

		if ( doc->GetNote() == "" )
		{
			ctlP = (ControlType *) FrmGetObjectPtr(frmP,
				FrmGetObjectIndex(frmP, SeverityNotePopTrigger));
			CtlSetLabel(ctlP, "Please select note");
		}
		else
		{
			ctlP = (ControlType *) FrmGetObjectPtr(frmP,
					FrmGetObjectIndex(frmP, SeverityNotePopTrigger));
			CtlSetLabel(ctlP, doc->GetNote().c_str());
		}
	
		m_strFood.clear();
		m_strFood = db.m_parse_array[2];

		m_btnFood->SetLabel(m_strFood.substr(0, 18).c_str());
		if ( doc->GetTempature() == 0 )
		{
			m_fldTemp->SetFieldTextFromString("");
		}
		else
		{
			PalmString strLbl((long)doc->GetTempature());
			m_fldTemp->SetFieldTextFromString( strLbl );				
		}
		db.Close();
	}

    return true;
}

//---------------------------------------------------------------------------
bool Severity::OnFrmCloseEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool Severity::OnMenuEvent ( EventPtr eventP )
{
    return true;
}

//---------------------------------------------------------------------------
bool Severity::OnFirstUserEvent ( EventPtr eventP )
{
	return true;
}

//---------------------------------------------------------------------------
Boolean SeverityFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP = NULL;
	frmP = FrmGetActiveForm();	

	Severity * av = Severity::GetInstance();
	
	PalmString str;
	long lTemp = 0;

	MemoPadWriter db;

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
			av->OnFirstUserEvent( eventP );
			handled = true;
			break;
		case fldEnterEvent :
			if (eventP->data.fldEnter.fieldID == SeverityTempField)
			{
				SysKeyboardDialog(kbdNumbersAndPunc);
			}
			handled = false;
			break;
		case ctlSelectEvent:
			switch (eventP->data.ctlEnter.controlID)
			{
				case SeverityOkButton :
				
					Document * doc = Document::GetInstance();
					str = av->m_fldTemp->GetFieldText();
					lTemp = StrAToI(str.c_str());
					doc->SetTempature(lTemp);

					unsigned long l = TimGetSeconds();
					doc->SetTimestamp(l);

					if ( db.Open('hcCP', 'tfil') ) 
					{	
						long pos = Selections::GetInstance()->m_lStartPos[doc->GetBtnPos()];
						
						if ( !doc->IsManualEntry() )
							pos += doc->GetLstSelection();
						else
							pos += doc->GetLstSelection();

						db.ReadLine(pos, p, 512);
						int food_id = StrAToI(db.m_parse_array[1]);
						db.Close();

						Items::GetInstance()->SaveTempReading( food_id, --pos, true );
					}

					av->Navigate();
					handled = true;
					break;
					
				default:
					break;
			}
			handled = false;
			break;
		case popSelectEvent:
			Document * doc = Document::GetInstance();
			if ( doc )
			{
				if ( eventP->data.popSelect.listID == SeverityNotesList )
				{
					switch (eventP->data.popSelect.selection)
					{
						case 0:
							doc->SetNote(av->m_strArray[0]);							
							break;
						case 1:
							doc->SetNote(av->m_strArray[1]);
							break;
						case 2:
							doc->SetNote(av->m_strArray[2]);
							break;
						case 3:
							doc->SetNote(av->m_strArray[3]);
							break;
						case 4:
							doc->SetNote(av->m_strArray[4]);
							break;
						case 5:
							doc->SetNote(av->m_strArray[5]);
							break;
						case 6:
							doc->SetNote(av->m_strArray[6]);
							break;						
						default:
							break;					
					}					
				}
				// set focus
				UInt16 id = av->m_fldTemp->GetObjectIndex();
				FrmSetFocus(frmP, id);		
			}
			handled = false;
			break;
		default:
			break;
	}

	return handled;
}

//---------------------------------------------------------------------------