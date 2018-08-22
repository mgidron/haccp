/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _YESNOALERT_H
#define _YESNOALERT_H

class YesNoAlert //: public Alert --> would inherit from Form
{
public:
  YesNoAlert(short alert_id);
  enum AlertResponse {YES, NO, DOALERT_NOT_CALLED};

  AlertResponse DoAlert(const char *m1 = "", const char *m2 = "", const char *m3 = "");

  void DoOkAlert(const char *m1 = "", const char *m2 = "", const char *m3 = "");
  inline AlertResponse GetAlertResult();  


private:
  YesNoAlert();
  
  short m_alert_id;
  AlertResponse m_result;
};

inline YesNoAlert::AlertResponse YesNoAlert::GetAlertResult()
{
  return m_result;
}

#include "HACCP2002rsc.h"
static void Trace( const char * buf )
{
	YesNoAlert oka(InformationAlertAlert);
	oka.DoAlert(buf);
}


#endif  // _YESNOALERT_H