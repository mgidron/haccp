/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _LoginFORM_H
#define _LoginFORM_H

#include <PalmOS.h>
#include "HACCP2002Rsc.h"
#include "stdTypes.h"
#include "UIForm.h"
#include "UIField.h"
#include "UIButton.h"
#include "UIList.h"

class Login : public UIForm
{

friend Boolean LoginFormHandleEvent(EventPtr eventP);

public:

	 static Login * GetInstance();	
	 ~Login();

	 enum { FORM_ID = LoginForm };
	 virtual FormId FrmId( void ) const {
		return FORM_ID;
	 }	

protected:
	Login();

  	virtual UInt16 NavigateTo( void );
  	virtual void SetStateOnFormOpen( void );
  	virtual void SetStateOnFormClose( void );

  	virtual bool OnFrmOpenEvent ( EventPtr eventP );
  	virtual bool OnFrmCloseEvent ( EventPtr eventP );
  	virtual bool OnMenuEvent ( EventPtr eventP );
  	virtual bool OnFirstUserEvent ( EventPtr eventP );

private:

	void FillUsersList( void );

	static Login * _gLogin;
  	UIList		 * m_lstLogin;
	UIButton * m_btnContinue;
	char		 * m_pBuf[8];
};

Boolean LoginFormHandleEvent(EventPtr eventP);

#endif		// _Login_H
