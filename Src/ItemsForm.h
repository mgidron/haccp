/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _ItemsFORM_H
#define _ItemsFORM_H

#include <PalmOS.h>
#include "HACCP2002Rsc.h"
#include "stdTypes.h"
#include "UIForm.h"
#include "UIField.h"
#include "UIList.h"
#include "UIButton.h"
#include "MemoPadWriter.h"

#include "HACCP2002.h"

class Items : public UIForm
{

friend Boolean ItemsFormHandleEvent(EventPtr eventP);

public:

	 static Items * GetInstance();	
	 ~Items();

	 enum { FORM_ID = ItemsForm };
	 virtual FormId FrmId( void ) const {
		return FORM_ID;
	 }	

protected:
	Items();

  	virtual UInt16 NavigateTo( void );
  	virtual void SetStateOnFormOpen( void );
  	virtual void SetStateOnFormClose( void );

  	virtual bool OnFrmOpenEvent ( EventPtr eventP );
  	virtual bool OnFrmCloseEvent ( EventPtr eventP );
  	virtual bool OnMenuEvent ( EventPtr eventP );
  	virtual bool OnFirstUserEvent ( EventPtr eventP);

	void ResetHiddenFields( int idx );

public:
	void SaveTempReading( int food_id, long idx, bool bResave = false);

private:

	static Items * _gItems;
	UIList * m_lstFood;
	UIButton * m_btnLocation;  	

	UIField * m_fldHidden;

	char * m_pBuf[10];
	char * sztmp;

	MemoPadWriter * m_db;

	char * m_pButtonText;
};

Boolean ItemsFormHandleEvent(EventPtr eventP);

#endif		// _Items_H
