/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "stdTypes.h"
#include "FormManager.h"
#include "HACCP2002Rsc.h"


bool                     FormManager::m_bIsMenuEvent = false;
FormManager::FormResults FormManager::m_FormResults;
//std::stack<FormId>       FormManager::m_stkForms;

FormManager::FormManager()
{
}

FormManager::~FormManager()
{
}

void FormManager::Navigate( void )
{
  FormId formId = NavigateTo();

  switch( formId )
  {
    case eReturnToParent :
      ReturnToForm();
      m_ParentFormData.m_ChildForm = eDefault;
      break;
    case eDoNotNavigate :
      return;
    case eWIP :
      break;
    case eDefault :
    default:
      m_ParentFormData.m_ChildForm = formId;  // remember where we're going
      m_FormResults.m_Parent = FrmId();

      GotoForm(formId);
  }
}

void FormManager::GotoForm( FormId frmId, bool isMenuEvent  )
{
  //m_stkForms.push( frmId );

  FrmGotoForm( frmId );
}

void FormManager::ReturnToForm( void )
{
  //m_stkForms.pop();   // remove the current form id

  FormId frmId = 0;//= m_stkForms.top();

  FrmGotoForm( frmId );
}


void FormManager::SetMenuEvent( void )
{
  m_bIsMenuEvent = true;
}

void FormManager::ResetMenuEvent( void )
{
  m_bIsMenuEvent = false;
}

bool FormManager::IsMenuEvent( void )
{
  return m_bIsMenuEvent;
}

FormManager::FormResults::FormResults()
{
  reset();
}

void FormManager::FormResults::SetDataAt( unsigned loc, const PalmString & data )
{
  m_Data[loc] = data;
}

PalmString FormManager::FormResults::operator[]( unsigned loc )
{
  return m_Data[loc];
}

bool FormManager::FormResults::operator[]( const char * str )
{
  return operator[] ( PalmString(str) );
}

bool FormManager::FormResults::operator[]( const PalmString & str )
{
  for( int i = 0; i < MAX_PARAMETERS - 1; i++ ){
    if( m_Data[i].IsEmpty() ) {
      m_Data[i] = str;
      break;
    }
  }
  return true;
}

PalmString FormManager::FormResults::GetDataAt( unsigned loc )
{
  return m_Data[loc];
}

void FormManager::FormResults::ClearData( void )
{
  for( int i = 0; i < MAX_PARAMETERS - 1; i++ )
  {
    m_Data[i] = "";
  }
}

void FormManager::FormResults::reset()
{
  m_Parent = NO_VALUE;
  m_UserAction = eNoFormResult;
  ClearData();
}