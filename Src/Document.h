/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include <PalmOS.h>
#include "stdTypes.h"
#include "PalmString.h"
#include <PalmCompatibility.h>


class Document
{

public:

	static Document * GetInstance();
	~Document();

public:

	long			GetStageId () const { return m_lStageId; }
	void			SetStageId ( const long lStageId ) { m_lStageId = lStageId; }

	long			GetUserId () const { return m_lUserId;}
	void			SetUserId ( const long lUserId) { m_lUserId = lUserId;}

	long			GetBtnId () const { return m_lBtnId;}
	void			SetBtnId ( const long lBtnId) { m_lBtnId = lBtnId;}

	int				GetBtnPos () const { return m_iBtnPos;}
	void			SetBtnPos ( const int iBtnPos) { m_iBtnPos = iBtnPos;}

	PalmString		GetBtnSelection () const { return m_strBtnSelection;}
	void			SetBtnSelection ( const PalmString strBtnSelection) { m_strBtnSelection = strBtnSelection;}

	int				GetLstSelection () const { return m_iLstSelection;}
	void			SetLstSelection ( const int iLstSelection) { m_iLstSelection = iLstSelection;}

	int				GetTempature () const { return m_iTempature;}
	void			SetTempature ( const int iTempature) { m_iTempature = iTempature;}

	unsigned long 	GetTimestamp () const { return m_lTimestamp;}
	void			SetTimestamp ( const unsigned long lTimestamp) { m_lTimestamp = lTimestamp;}

	PalmString		GetNote () const { return m_strNote;}
	void			SetNote ( const PalmString strNote) { m_strNote = strNote;}

	bool			IsOutOfTemp () const { return m_bOutOfTemp;}
	void			SetOutOfTemp ( const bool bOutOfTemp) { m_bOutOfTemp = bOutOfTemp;}

	bool			IsDataInPort () const { return m_bDataInPort;}
	void			SetDataInPort ( const bool bDataInPort) { m_bDataInPort = bDataInPort;}
	
	bool			IsManualEntry () const { return m_bManualEntry;}
	void			SetManualEntry ( const bool bManualEntry) { m_bManualEntry = bManualEntry;}

	bool			IsDemoMode () const { return m_bDemoMode;}
	void			SetDemoMode ( const bool bDemoMode) { m_bDemoMode = bDemoMode;}

	PalmString		GetCurrentTimeStamp( void );
	PalmString		GetHour( void );
	int				ConvertTemp( char * pszBuffer );

	void			LoadStateMachine( void );
	void			SaveStateMachine( void );

	bool			IsBatterPowerLow(PalmString & strMsg);

	 
protected:
	 
	Document();
  
private:

	static Document * _gDocument;

	long			m_lStageId;
	long			m_lUserId;
	long			m_lBtnId;
	int				m_iBtnPos;
	int				m_iLstSelection;
	int 			m_iTempature;
	unsigned long 	m_lTimestamp;
	PalmString		m_strBtnSelection;
	PalmString 		m_strNote;

	bool			m_bOutOfTemp;	
	bool			m_bDataInPort;
	bool			m_bManualEntry;
	
	bool			m_bDemoMode;

	char *			m_pszBuffer;
};