/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _SelectionsFORM_H
#define _SelectionsFORM_H

#include <PalmOS.h>
#include "HACCP2002Rsc.h"
#include "stdTypes.h"
#include "UIForm.h"
#include "UIField.h"
#include "UIPushButton.h"

class Selections : public UIForm
{

friend Boolean SelectionsFormHandleEvent(EventPtr eventP);

public:

	 static Selections * GetInstance();	
	 ~Selections();

	 enum { FORM_ID = SelectionsForm };
	 virtual FormId FrmId( void ) const {
		return FORM_ID;
	 }	

protected:
	Selections();

  	virtual UInt16 NavigateTo( void );
  	virtual void SetStateOnFormOpen( void );
  	virtual void SetStateOnFormClose( void );

  	virtual bool OnFrmOpenEvent ( EventPtr eventP );
  	virtual bool OnFrmCloseEvent ( EventPtr eventP );
  	virtual bool OnMenuEvent ( EventPtr eventP );
  	virtual bool OnFirstUserEvent ( EventPtr eventP );

private:

	static Selections * _gSelections;

	UIPushButton * m_btnButton[6];

public:

	long m_lBtnId[6];
	long m_lStartPos[6];
	char * m_pBuffer[6];
};

Boolean SelectionsFormHandleEvent(EventPtr eventP);

#endif		// _Selections_H
