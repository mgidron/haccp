/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _MENUHANDLER_H
#define _MENUHANDLER_H

#include "stdTypes.h"
#include "FormManager.h"

class MenuHandler// : protected FormManager
{
public:

	explicit MenuHandler(FormId id);
	FormId HandleEvent(int command);

private:

	FormId OnActivateDemoMode();
	FormId OnAboutHaccp2002();
	FormId OnExit();

protected:

	FormId m_handler_form_id;
};

#endif // _MENUHANDLER_H