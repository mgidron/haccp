/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "stdtypes.h"
//#include "SharedCode.h"
#include "YesNoAlert.h"
#include "PalmOS.h"

YesNoAlert::YesNoAlert(short alert_id) : m_alert_id(alert_id), m_result(DOALERT_NOT_CALLED)
{
}

YesNoAlert::AlertResponse YesNoAlert::DoAlert
(
  const char *message1, 
  const char *message2, 
  const char *message3
)
{
  short result = FrmCustomAlert(m_alert_id, message1, message2, message3);
  switch(result)
  {
    case 0:
      m_result = YES;
      break;
    case 1:
      m_result = NO;
      break;
    default:
      m_result = DOALERT_NOT_CALLED;
  }  

  return m_result;
}

void YesNoAlert::DoOkAlert
(
  const char *message1, 
  const char *message2, 
  const char *message3
)
{
  FrmCustomAlert(m_alert_id, message1, message2, message3);
}
