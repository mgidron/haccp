/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "UIComponent.h"

UIComponent::UIComponent(ComponentId id, FormId formid) :
    m_component_id(id),
    m_FormId(formid)
{
}

UIComponent::UIComponent(UInt16 id) : 
  m_component_id(id),
  m_FormId(-1)
{
}

UIComponent::UIComponent() :
	m_component_id(-1),
  m_FormId(-1)
{
}

UIComponent::~UIComponent()
{
}

void *UIComponent::GetObjectPtr()
{
	return FrmGetObjectPtr(FrmGetActiveForm(), FrmGetObjectIndex(FrmGetActiveForm(), m_component_id));
}

Word UIComponent::GetObjectIndex()
{
  return FrmGetObjectIndex(FrmGetActiveForm(), m_component_id);
}

void UIComponent::Show(bool show)
{
  if(show)
    FrmShowObject(FrmGetActiveForm(), GetObjectIndex());
  else
    FrmHideObject(FrmGetActiveForm(), GetObjectIndex());
}

void UIComponent::Enable()
{
  CtlSetEnabled ((ControlPtr) GetObjectPtr(), true);
}

void UIComponent::Disable()
{
  CtlSetEnabled ((ControlPtr) GetObjectPtr(), false);
}

bool UIComponent::IsEnabled( void )
{
  bool bRv = CtlEnabled ((ControlPtr) GetObjectPtr());

  return bRv;
}