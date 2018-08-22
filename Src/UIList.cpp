/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "UIList.h"
#include <PalmOS.h>
#include "MemoPadWriter.h"
#include "Document.h"
#include "SelectionsForm.h"


UIList::UIList(UInt16 id, FormId formid) :
	UIComponent(id, formid),
	m_MaxVisibleItems(0)
{

}

UIList::UIList(UInt16 id) : UIComponent(id)
{

}
    
UIList::~UIList()
{

}

void UIList::Draw()
{
	LstDrawList(GetListPtr());
}

void UIList::ScrollUp(int items)
{
	LstScrollList(GetListPtr(), winUp, items);
}

void UIList::ScrollDown(int items)
{
	LstScrollList(GetListPtr(), winDown, items);
}

Word UIList::GetSelectedIndex()
{
  Word ret(-1);

  ListPtr listP = static_cast<ListPtr>(GetObjectPtr());
  if(listP)
    ret = LstGetSelection(listP);

  return ret;
}

void UIList::ListSetSelection(short sel_index)
{
  ListPtr listP = static_cast<ListPtr>(GetObjectPtr());
  if(listP && sel_index < LstGetNumberOfItems(listP))
  {
    LstSetSelection(listP, sel_index);
  }
}

PalmString UIList::ListGetSelectedItemText()
{
  PalmString str("");

  ListPtr listP = static_cast<ListPtr>(GetObjectPtr());
  if(listP)
  {
    Word selItem = LstGetSelection(listP);
    if(noListSelection != selItem)
    {
      str = LstGetSelectionText(listP, selItem);
    }
  }

  return str;
}

void UIList::ListClearSelection()
{
  ListPtr listP = static_cast<ListPtr>(GetObjectPtr());
  if(listP)
  {
    LstSetSelection(listP, -1);
  }
}

ListPtr UIList::GetListPtr()
{
  return (ListPtr)GetObjectPtr();
}

void UIList::SetCountVisibleItems( UInt16 count )
{
  if( 0 >= m_MaxVisibleItems )        // max visible items not set
    LstSetHeight( GetListPtr(), count );
  else
    LstSetHeight( GetListPtr(), (m_MaxVisibleItems < count) ? m_MaxVisibleItems : count);
}

UInt16 UIList::GetListCount( void )
{
	return LstGetNumberOfItems( GetListPtr() );
}

void UIList::AddItemsToList( char** items, UInt16 count )
{
  LstSetListChoices (GetListPtr(), (char **) items, count);
  LstMakeItemVisible(GetListPtr(), count - 1);
  for (int i = 0; i < count;i++ )
  {
	  LstSetSelection(GetListPtr(), i);
  }
  LstSetSelection(GetListPtr(), count - 1);
  LstSetSelection(GetListPtr(), 0);    
}

UInt16 UIList::GetSelectedItem(PalmString & str)
{
  UInt16 ItemNum = LstGetSelection(GetListPtr());
  if( noListSelection == ItemNum )
  {
    str = "";
  }
  else
  {
    CharPtr cp = LstGetSelectionText( GetListPtr(), ItemNum );
    if( cp )
    {
      str = (char*)cp;
    }
  }

  return ItemNum;
}

void UIList::AddItemsToListFromStringResource( UInt16 resId )
{
  MemPtr theListPtr=0;
  MemHandle theListItems=0;
  UInt32 theListCount=0;
  MemHandle theRecHandle=0;
  char* theRecPtr=0;

  // Get a reference to the tSTL resource.
  // This resource is created when you make a
  // "String List" in constructor.
  theRecHandle = DmGetResource( 'tSTL', resId );
  theRecPtr = static_cast<char*>(MemHandleLock( theRecHandle ));

  // Parse off the prefix string
  int i = StrLen( theRecPtr);
  theRecPtr += StrLen( theRecPtr) + 1;

  // now grab the number of items and increment the pointer
  theListCount = (theRecPtr[0] << 8) + theRecPtr[1];
  theRecPtr += sizeof( Int16 );

  // Finally, create the list from the packed strings in the
  // string resource
  theListItems = SysFormPointerArrayToStrings( theRecPtr, theListCount );

  // Now lock down the list handle so we can pass it to the list
  theListPtr = MemHandleLock( theListItems );

  // Note that you cannot free either the resource pointer (theRecPtr) or
  // the list pointer (theListItems) until the list is disposed of.
}

void UIList::PopupList( void )
{
	ListPtr pList = GetListPtr();
	LstPopupList(pList);
}

////////////////////////////////////////////////////////////////////////////////

static void DrawString(short itemNum, RectangleType * bounds, char ** itemsText) 
{ 
	char s[101]; 
	char tmp[25];
        
	MemoPadWriter db; 

	if ( !db.Open('hcCP', 'tfil') ) 
		return; 

	Document * pDoc = Document::GetInstance();
	if (!pDoc)
		return;

	long pos = Selections::GetInstance()->m_lStartPos[pDoc->GetBtnPos()];
	pos += itemNum;

	db.ReadLine(pos, p, 512); 
	if ( pDoc->GetBtnId() == StrAToI(db.m_parse_array[3]) )
	{
		MemSet(tmp, 0, 25);
		StrNCopy(tmp, db.m_parse_array[2], 18);
		
		if ( StrCompare(db.m_parse_array[5], " ") == 0 )
		{
			WinDrawChars(tmp, StrLen(tmp), bounds->topLeft.x, bounds->topLeft.y);
		}
		else
		{
			StrPrintF(s, "%s\xb0", db.m_parse_array[5]);						
			WinDrawChars(tmp, StrLen(tmp), bounds->topLeft.x, bounds->topLeft.y);       
			WinDrawChars(s, StrLen(s), bounds->topLeft.x+78, bounds->topLeft.y);
			WinDrawChars("F", 1, bounds->topLeft.x+98, bounds->topLeft.y);      
			WinDrawChars(db.m_parse_array[6], StrLen(db.m_parse_array[6]), bounds->topLeft.x+120, bounds->topLeft.y);
		}
	}
	
	db.Close(); 	
} 

////////////////////////////////////////////////////////////////////////////////

static void DrawUsersString(short itemNum, RectangleType * bounds, char ** itemsText) 
{ 
	char s[101]; 

	MemoPadWriter db; 

	if ( !db.Open('hcCP', 'user') ) 
		return;

	db.ReadLine(itemNum, p, 512);
	StrPrintF(s, "%s %s", db.m_parse_array[1], db.m_parse_array[2]);

	db.Close();
	
	WinDrawChars(s, StrLen(s), bounds->topLeft.x, bounds->topLeft.y);       
}

//////////////////////////////////////////////////////////////////////////////// 

void UIList::UpdateList() 
{ 
	MemoPadWriter db; 

	Document * pDoc = Document::GetInstance();
	if (!pDoc)
		return;

	long pos = Selections::GetInstance()->m_lStartPos[pDoc->GetBtnPos()];
	long count = 0;
	
	if ( !db.Open('hcCP', 'tfil') ) 
		return; 

	long lMax = db.GetNumRecords();

	for (int i = pos; i < lMax;i++ )
	{
		db.ReadLine(i, p, 512); 
		if ( pDoc->GetBtnId() == StrAToI(db.m_parse_array[3]) )
		{
			count++;
		}
	}
	db.Close(); 

	LstSetDrawFunction(GetListPtr(), DrawString); 
	LstSetListChoices (GetListPtr(), NULL, count );                
} 

////////////////////////////////////////////////////////////////////////////////

void UIList::UpdateLoginList()
{
	MemoPadWriter db; 

	if ( !db.Open('hcCP', 'user') ) 
		return;

	long lMax = db.GetNumRecords(); 
	db.Close(); 

	LstSetDrawFunction(GetListPtr(), DrawUsersString); 
	LstSetListChoices (GetListPtr(), NULL, lMax -1);                
}

//////////////////////////////////////////////////////////////////////////////// 