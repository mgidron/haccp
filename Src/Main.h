/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _MAINFORM_H
#define _MAINFORM_H

#include <PalmOS.h>
#include "HACCP2002Rsc.h"
#include "stdTypes.h"
#include "UIForm.h"
#include "UIField.h"

class Main : public UIForm
{

friend Boolean MainFormHandleEvent(EventPtr eventP);

public:

	 static Main * GetInstance();	
	 ~Main();

	 enum { FORM_ID = MainForm };
	 virtual FormId FrmId( void ) const {
		return FORM_ID;
	 }	

protected:
	Main();

  	virtual UInt16 NavigateTo( void );
  	virtual void SetStateOnFormOpen( void );
  	virtual void SetStateOnFormClose( void );

  	virtual bool OnFrmOpenEvent ( EventPtr eventP );
  	virtual bool OnFrmCloseEvent ( EventPtr eventP );
  	virtual bool OnMenuEvent ( EventPtr eventP );
  	virtual bool OnFirstUserEvent ( EventPtr eventP );

private:

	static Main * _gMain;
	UIField * m_fldCopyright;

	unsigned long m_lStartTime;
  		
};

Boolean MainFormHandleEvent(EventPtr eventP);

#endif		// _MAINFORM_H
