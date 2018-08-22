/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _FORMMANAGER_H
#define _FORMMANAGER_H

#include "stdTypes.h"
#include "PalmString.h"
//#include <stack>
//#include <vector>

const unsigned NO_VALUE = 0;

class FormManager
{

public:
  FormManager();
  virtual ~FormManager();

  enum FormResult {
    eNoFormResult = NO_VALUE,
    eEnter,
    eCancel
  };
  enum {
    eDefault = NO_VALUE,
    eReturnToParent,
    eDoNotNavigate,
    eWIP                                                             // Work In Progres
  };

protected:
  struct ParentFormData    // not shared across forms
  {
    FormId m_ChildForm;                      // The form that set m_Parameter
    ButtonId  m_btnField;                 // the button the user selected to change a field value

    ParentFormData() {
      reset();
    }

    void reset() {
      m_ChildForm = NO_VALUE;
      m_btnField = NO_VALUE;
    }
  };

  struct FormResults // shared data across forms
  {
    FormId m_Parent;                // The form that set m_Data (child form)
    FormResult m_UserAction;

    private:

    enum {MAX_PARAMETERS = 5};

    PalmString m_Data[MAX_PARAMETERS];

    public:

    FormResults();
    void SetDataAt( unsigned loc, const PalmString & data );
    PalmString operator[]( unsigned loc );
    bool operator[]( const char * str );
    bool operator[]( const PalmString & str );
    PalmString GetDataAt( unsigned loc );
    void ClearData( void );
    void reset();
  };

public:
  static void ResetMenuEvent( void );

protected:

  void SetFormResult( FormResult result );

  static void SetMenuEvent( void );
  static bool IsMenuEvent( void );

  virtual void Navigate( void );

  virtual FormId FrmId( void ) const = 0;
  virtual FormId NavigateTo( void ) = 0;

  // required events
  virtual bool OnFrmOpenEvent( EventPtr eventP ) = 0;
  virtual bool OnFrmCloseEvent( EventPtr eventP ) = 0;
  virtual bool OnMenuEvent( EventPtr eventP ) = 0;
  virtual bool OnFirstUserEvent( EventPtr eventP ) = 0;

//protected:
private:

protected:
  void GotoForm( FormId frmId, bool isMenuEvent = false );
  void ReturnToForm( void );

private:
  static bool m_bIsMenuEvent;
  //static std::stack<FormId> m_stkForms;

protected:
  static FormResults m_FormResults;
  ParentFormData m_ParentFormData;
  ControlId m_ControlId; // the users action
};

#endif // _GROUP_H