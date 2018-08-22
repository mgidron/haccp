#ifndef _UIBUTTON_H
#define _UIBUTTON_H

#include "stdTypes.H"
#include "UIComponent.h"
#include "PalmTypes.h"
#include "PalmString.h"

class UIButton : public UIComponent
{
public:
  UIButton(ButtonId button_id, FormId formid);
  ~UIButton();

//  void SetLabel( PalmString & str );
  void SetLabel( const char* cp );

  virtual void SetLabel(const PalmString & label);
  virtual void GetLabelText( PalmString & label );

  void GetLabel( PalmString & str );

  void CtlSetValue(bool is_on);
  bool CtlGetValue();

private:
  UIButton();

//  FormId    m_FormId;
//  ButtonId  m_ButtonId;
};

#endif  // _UIFIELD_H