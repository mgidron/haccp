/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#ifndef _UILIST_H
#define _UILIST_H

#include "UIComponent.h"

class UIList : public UIComponent
{

public:

  UIList(ListId field_id, FormId formid);
  UIList(ListId field_id);
  ~UIList();

  void Draw();
  void ScrollUp(int items = 1);
  void ScrollDown(int items = 1);

  PalmString ListGetSelectedItemText();
  void ListClearSelection();

  Word GetSelectedIndex();
  UInt16 GetSelectedItem(PalmString & str);

  void SetCountVisibleItems( UInt16 count );
  UInt16 GetListCount( void );

  void AddItemsToList( char** items, UInt16 count );
  void AddItemsToListFromStringResource( UInt16 resId );

  void ListSetSelection(short sel_index);

  void PopupList( void );
  void UpdateList( void );
  void UpdateLoginList( void );
  
  ListPtr GetListPtr();

private:
  UIList(); 

  UInt16 m_MaxVisibleItems;


};

static char p[512];

#endif  // _UIFIELD_H
