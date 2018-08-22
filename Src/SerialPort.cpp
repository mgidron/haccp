/*
	HACCP2004 Code written by: Michael B. Gidron

	Copyright (c) 2002 by Michael B. Gidron
	All rights reserved.
*/

#include "SerialPort.h"
#include "Document.h"
#include "PalmString.h"

static void SerialCallback(UInt32 refCon)
{  
	if ( Document * pDoc = Document::GetInstance() )
		pDoc->SetDataInPort(true);

    EvtWakeup();	
}

SerialPort * SerialPort::_instance = 0;

SerialPort * SerialPort::GetInstance()
{
	if ( _instance == NULL )
	{
		_instance = new SerialPort;
	}
	return _instance;
}

///////////////////////////////////////////////////////////////////////////////

SerialPort::SerialPort()   
{
	m_iPortId = 0;
	m_error = false;
	SetDataInPort(false);
}

///////////////////////////////////////////////////////////////////////////////

SerialPort::~SerialPort()
{
	if ( _instance )
	{
		delete _instance;
	}
}

///////////////////////////////////////////////////////////////////////////////

UInt32 SerialPort::Receive(void *rcvBufP, UInt32 count, Int32 timeout, Err *errP) 
{ 
	if ( !m_iPortId ) 
		return 0;
		
	return SrmReceive(m_iPortId, rcvBufP, count, timeout, errP); 	
} 

///////////////////////////////////////////////////////////////////////////////

Err SerialPort::SetReceiveBuffer(void *bufP, UInt16 bufSize) 
{ 
	if ( !m_iPortId ) 
		return 0;
	
	return SrmSetReceiveBuffer(m_iPortId, bufP, bufSize);      	
} 

///////////////////////////////////////////////////////////////////////////////

Err SerialPort::ReceiveWait(UInt32 bytes, Int32 timeout) 
{ 
    if ( !m_iPortId ) 
    	return 0;
	
	return SrmReceiveWait(m_iPortId, bytes, timeout);     
} 

///////////////////////////////////////////////////////////////////////////////

Err SerialPort::ReceiveFlush(Int32 timeout) 
{ 
    if ( !m_iPortId ) 
    	return 0;
    	
	return SrmReceiveFlush(m_iPortId, timeout);      
} 

///////////////////////////////////////////////////////////////////////////////

Err SerialPort::ReceiveCheck(UInt32 *numBytesP) 
{ 
	if ( !m_iPortId ) 
    	return 0;
    	
	return SrmReceiveCheck(m_iPortId, numBytesP); 	
} 

///////////////////////////////////////////////////////////////////////////////

Err SerialPort::Close(void) 
{ 
	if ( !m_iPortId ) 
    	return 0;
  
	SetReceiveBuffer(NULL, 0); 
    return SrmClose(m_iPortId);      	
} 

///////////////////////////////////////////////////////////////////////////////

Err SerialPort::Open(void) 
{ 
	Err err;
      
    err = SrmOpen(serPortCradlePort /* port */, 4800, /* baud */ &m_iPortId); 
    if (err) 
    { 
		// display error message here. 
		switch (err)
		{
			case serErrAlreadyOpen:
				ErrDisplay("Serial Port Already Open");
		        break;
			case serErrBadPort:
		        ErrDisplay("Bad Port");
				break;
		    case memErrNotEnoughSpace:
				ErrDisplay("Not enough space to open serial port");
				break;
	        case cncErrGetProfileFailed:
				ErrDisplay("Unable to get the device profile");
				break;
			case cncErrProfileNotFound:
				ErrDisplay("Unable to find the device profile");
				break;
			case cncErrConDBNotFound:
				ErrDisplay("Unable to fine the device profile database");
				break;
			default:
				break;
	    }
	} 
	else
	{
		SrmSetWakeupHandler(m_iPortId, SerialCallback, m_iPortId);
		SrmPrimeWakeupHandler(m_iPortId, 12);	
	}
  
	return err;
} 

///////////////////////////////////////////////////////////////////////////////

// returns n'th (0-based) comma-delimited field within buffer 
// true if field found, false otherwise 
Boolean SerialPort::GetField(const Char *buffer, UInt16 n, Char *result) 
{ 
        int     i; 
        
        // skip n commas 
        for (i = 0; i < n; i++) { 
                while (*buffer && *buffer != ',') 
                        buffer++; 
                if (*buffer == '\0') 
                        return false; 
                buffer++; 
        } 
        while (*buffer && *buffer != ',') 
                *result++ = *buffer++; 
        *result = '\0'; 
        return *buffer == ',' || *buffer == '\0'; 
}

///////////////////////////////////////////////////////////////////////////////

void SerialPort::HandleSerReceiveErr(Err err) 
{ 
	UInt32 lineStatus; 
	UInt16 lineErrs; 
  
	if (err == serErrLineErr) 
    { 
		SrmGetStatus(m_iPortId, &lineStatus, &lineErrs); 
        
        // test for framing or parity error. 
        if (lineErrs & serLineErrorFraming | serLineErrorParity)  
		{ 
				//framing or parity error occurred. Do something. 
		} 
		ReceiveFlush(1);
		SrmClearErr(m_iPortId); 
	} 
}

///////////////////////////////////////////////////////////////////////////////

int SerialPort::PalmRead(char * buf, int buf_size)
{
  int result = -1;

  if (m_iPortId)
  {
    unsigned char * rx_buf = NULL;
    UInt32 rx_buf_len = 0;
    
    // open a window to the receive buffer
    SrmReceiveWindowOpen(m_iPortId, &rx_buf, &rx_buf_len);
	
    if (rx_buf != NULL  &&  rx_buf_len > 0)
    {
      if (rx_buf_len == 0)
      {
        result = -1;
      }
      else
      {
        result = -2;
      }
      if (rx_buf_len > buf_size)
      {
        rx_buf_len = buf_size;
      }

      // copy data from the serial receive buffer to buf
      //MemMove(buf, rx_buf, rx_buf_len);
	  int n = 0;
	  for (int i = 0; i < rx_buf_len; i++)
	  {
		if ( TxtCharIsDigit( rx_buf[i] ) )
		{
			buf[n] = rx_buf[i];
			n++;
		}
	  }
	  buf[n] = '\0';
	  
      result = (int) rx_buf_len;
    }
    else
    {
      // error opening the window so don't remove any data
      rx_buf_len = 0;
    }

    // close the window and remove the data that was copied
    SrmReceiveWindowClose(m_iPortId, rx_buf_len);
  }

  SrmPrimeWakeupHandler(m_iPortId, 12);
  ReceiveFlush(1);

  return result;
} 

///////////////////////////////////////////////////////////////////////////////

UInt32 SerialPort::WaitForData(UInt32 bufferSize)
{
	UInt32 numBytesPending = 0;
	Err	err;
	// const Int32 kOneHalfSecond = gTicksPerSecond / 2;
	
	// We'll fill our read buffer, or 1/2 second between
	// bytes, whichever comes first.
	err = ReceiveWait(bufferSize, SysTicksPerSecond() * 5);

	if (err == serErrLineErr) 
	{
		FrmCustomAlert(1300, "ReceiveWaite serErrLineErr", 0, 0);
		ReceiveFlush(1);	// will clear the error
		return 0;			
	} 

	if (err != serErrTimeOut)
		ErrFatalDisplayIf(err != 0, "DoReceiveWait");

	err = ReceiveCheck(&numBytesPending);

	if (err == serErrLineErr) {
		FrmCustomAlert(1300,"ReceiveCheck serErrLineErr", 0, 0); 
		ReceiveFlush(1);	// will clear the error
		return 0; 			
	}

	ErrFatalDisplayIf(err != 0, "DoReceiveCheck Fail");
	return numBytesPending;
}

///////////////////////////////////////////////////////////////////////////////

bool SerialPort::ReadReady(void) const
{
  UInt32 byte_count;
  SrmReceiveCheck(m_iPortId, &byte_count);
  if( byte_count != 0 ) 
  {
    return true;
  }
  else
  {
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////////

void SerialPort::Read(char & byte)
{
  m_error = false;
  Err error;
  UInt32 count = SrmReceive(m_iPortId, &byte, 1, 1000, &error);
  if( error )
  {
    m_error = true;
  }
}

///////////////////////////////////////////////////////////////////////////////

bool SerialPort::Read( char * data, UInt32 bufferSize, UInt32 numBytesPending )
{
	Err err;
	UInt32 numBytes;

	// Read however many bytes are waiting
	if ( numBytesPending > bufferSize )
		numBytesPending = bufferSize;
	
	numBytes = SrmReceive( m_iPortId, data, bufferSize, 0, &err);

	if ( err == serErrLineErr )
		SrmReceiveFlush(m_iPortId, 1); // will clear the error
	else
		data[numBytes] = '\0'; // null terminate the data

	SrmReceiveFlush(m_iPortId, 1); // will clear the error
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

bool SerialPort::Error(void) const
{
  return m_error;
}

///////////////////////////////////////////////////////////////////////////////

void SerialPort::ConfigPort()
{
	Err err; 
	unsigned short paramSize; 
	Int32 baudRate = 4800; 
	UInt32 flags = srmSettingsFlagBitsPerChar8 |
	srmSettingsFlagStopBits1 | srmSettingsFlagRTSAutoM;
//	srmSettingsFlagCTSAutoM; 
	Int32 ctsTimeout = SysTicksPerSecond() / 2; 
	  
	paramSize = sizeof(baudRate); 
	err = SrmControl(m_iPortId, srmCtlSetBaudRate, &baudRate,  
	   &paramSize); 
	  
	paramSize = sizeof(flags); 
	err = SrmControl(m_iPortId, srmCtlSetFlags, &flags, &paramSize); 
	  
	paramSize = sizeof(ctsTimeout); 
	err = SrmControl(m_iPortId, srmCtlSetCtsTimeout, &ctsTimeout,  
	   &paramSize); 
}

///////////////////////////////////////////////////////////////////////////////