/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _SeverityFORM_H
#define _SeverityFORM_H

#include <PalmOS.h>
#include "HACCP2002Rsc.h"
#include "stdTypes.h"
#include "UIForm.h"
#include "UIField.h"
#include "UIComponent.h"
#include "UIList.h"
#include "UIButton.h"
#include "PalmString.h"

class Severity : public UIForm
{

friend Boolean SeverityFormHandleEvent(EventPtr eventP);

public:

	 static Severity * GetInstance();	
	 ~Severity();

	 enum { FORM_ID = SeverityForm };
	 virtual FormId FrmId( void ) const {
		return FORM_ID;
	 }	

protected:
	Severity();

  	virtual UInt16 NavigateTo( void );
  	virtual void SetStateOnFormOpen( void );
  	virtual void SetStateOnFormClose( void );

  	virtual bool OnFrmOpenEvent ( EventPtr eventP );
  	virtual bool OnFrmCloseEvent ( EventPtr eventP );
  	virtual bool OnMenuEvent ( EventPtr eventP );
  	virtual bool OnFirstUserEvent ( EventPtr eventP );

private:

	static Severity * _gSeverity;
  	UIField * m_fldTemp;
	UIField * m_fldDegrees;
	UIButton * m_btnFood;
	UIComponent * m_Popup;

	PalmString m_strArray[7];
	PalmString m_strFood;  		
};

Boolean SeverityFormHandleEvent(EventPtr eventP);

#endif		// _Severity_H
