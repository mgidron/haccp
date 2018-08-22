/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _ABOUTFORM_H
#define _ABOUTFORM_H

#include <PalmOS.h>
#include "HACCP2002Rsc.h"
#include "stdTypes.h"
#include "UIForm.h"
#include "UIField.h"

class About : public UIForm
{

friend Boolean AboutFormHandleEvent(EventPtr eventP);

public:

	 static About * GetInstance();	
	 ~About();

	 enum { FORM_ID = AboutForm };
	 virtual FormId FrmId( void ) const {
		return FORM_ID;
	 }	

protected:
	About();

  	virtual UInt16 NavigateTo( void );
  	virtual void SetStateOnFormOpen( void );
  	virtual void SetStateOnFormClose( void );

  	virtual bool OnFrmOpenEvent ( EventPtr eventP );
  	virtual bool OnFrmCloseEvent ( EventPtr eventP );
  	virtual bool OnMenuEvent ( EventPtr eventP );
  	virtual bool OnFirstUserEvent ( EventPtr eventP );

private:

	static About * _gAbout;
  	UIField * m_fldCopyright;
	  		
};

Boolean AboutFormHandleEvent(EventPtr eventP);

#endif		// _About_H
