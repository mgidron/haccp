#include "Utility.h"
#include "UIButton.h"

#include <PalmOS.h>
#include "stdtypes.h"
#include <string.h>

UIButton::UIButton(ButtonId button_id, FormId formid) :
    UIComponent(button_id, formid)
//    m_ButtonId( button_id ),
//    m_FormId( formid )
{
}

UIButton::~UIButton()
{
}

void UIButton::SetLabel( const PalmString & str )
{
  if( str.IsEmpty() )
    return;

  SetLabel( str.c_str() );
}

void UIButton::SetLabel(const char * str)
{
  ControlPtr ctrl = static_cast<ControlPtr>(GetObjectPtr());
  int len = StrLen(str);
  if (len >= 0)
  {
    // allocate a buffer for the control to keep
    char * buf = new char[len + 1];
    if (buf != NULL)
    {
      StrCopy(buf, str);
      CtlSetLabel(ctrl, buf);
    }
  }
}

void UIButton::GetLabelText( PalmString & label )
{
  GetLabel( label );
}

void UIButton::GetLabel( PalmString & str )
{
  str = CtlGetLabel(reinterpret_cast<ControlType*>(GetObjectPtr()));
}

void UIButton::CtlSetValue(bool is_on)
{
  ControlPtr ctrl = static_cast<ControlPtr>(GetObjectPtr());

  ::CtlSetValue(ctrl, is_on);
}

bool UIButton::CtlGetValue()
{
  ControlPtr ctrl = static_cast<ControlPtr>(GetObjectPtr());

  return ::CtlGetValue(ctrl);
}