/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _UICOMPONENT_H
#define _UICOMPONENT_H

#include "stdtypes.h"
#include "PalmString.h"
#include <PalmTypes.h>
#include <PalmOS.h>

class UIComponent
{
public:
  UIComponent(ComponentId id, FormId formid);
  UIComponent(UInt16 id);
  virtual ~UIComponent();

  void *GetObjectPtr();
  Word GetObjectIndex();
  inline UInt16 GetId();

  void Show(bool show = true);
  void ShowControl();
  void HideControl();

  FormId GetFormId() const;
  FormPtr GetFormPtr() const;

  void Enable() ;
  void Disable() ;
  bool IsEnabled( void );

/*
  virtual void SetLabel(PalmString & )  // should be implemented in derived class
  { }
  virtual void GetLabelText( PalmString )
  {}
*/

protected:
  MemPtr GetObjectPointer() const;

private:
  UIComponent();

  const ComponentId  m_component_id; // the resource id
  const FormId  m_FormId;
  UInt16        m_ObjectNdx;

};

inline void UIComponent::ShowControl()
{
  Show();
}

inline void UIComponent::HideControl()
{
  Show(false);
}

inline UInt16 UIComponent::GetId()
{
  return m_component_id;
}

inline FormId UIComponent::GetFormId() const
{
    return m_FormId;
}

inline FormPtr UIComponent::GetFormPtr() const
{
	return FrmGetFormPtr( m_FormId );
}

inline MemPtr UIComponent::GetObjectPointer() const
{
    return FrmGetObjectPtr(GetFormPtr(), m_ObjectNdx);
}

#endif  // _COMPONENT_H
