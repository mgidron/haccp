/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include <PalmOS.h>
#include <SerialMgr.h> 
#include "Haccp2002Rsc.h"
#include "YesNoAlert.h"

#define BYTES	         2048 
#define BAUD_RATE        4800 
#define MAX_WAKEUP_CHARS 12

class SerialPort
{

public:

	static SerialPort * GetInstance();
	~SerialPort();
	
	UInt32 Receive(void *rcvBufP, UInt32 count, Int32 timeout, Err *errP);
	Err SetReceiveBuffer(void *bufP, UInt16 bufSize);
	Err ReceiveWait(UInt32 bytes, Int32 timeout);
	Err ReceiveFlush(Int32 timeout); 
	Err ReceiveCheck(UInt32 *numBytesP);

	Err Close(void); 
	Err Open(void);

	Boolean GetField(const Char *buffer, UInt16 n, Char *result); 	
	void HandleSerReceiveErr(Err err);
	// Returns the number of bytes read (including 0), otherwise it returns < 0
	int PalmRead(char * buf, int buf_size);
	UInt32 WaitForData(UInt32 bufferSize);

	bool Read( char * data, UInt32 bufferSize, UInt32 numBytesPending );

	bool ReadReady(void) const;
	void Read(char & byte);
	bool Error(void) const;

	void ConfigPort();

	void SetDataInPort( bool bVal ){ m_bDataInPort = bVal; }
	bool IsDataInPort( void ){ return m_bDataInPort; }

protected:

	SerialPort();
	
private:

	static SerialPort * _instance;
	UInt16  m_iPortId;
	bool m_bDataInPort;
	bool m_error;
};