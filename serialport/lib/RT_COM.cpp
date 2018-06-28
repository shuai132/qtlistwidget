
#include "RT_COM.h"

RT_COM::RT_COM(DWORD MaxPackageSize, BOOL UsePackage)
{
	m_hComHandle = INVALID_HANDLE_VALUE;
	m_rxThread = INVALID_HANDLE_VALUE;
	m_RxPackageCallBack = NULL;
	m_RecvThreadStopFlag = FALSE;

    m_bUsePackage = UsePackage;

	m_FrameHead = 0xAA;
	m_FrameTail = 0x55;
	m_FrameCtrl = 0xA5;

	m_IsOpened = FALSE;
	m_MaxPackageSize =  MaxPackageSize * 2 +2;

	m_pRxBuf = new BYTE[m_MaxPackageSize];
	m_pTxBuf = new BYTE[m_MaxPackageSize];
	m_RxPackageDataCount = 0;

	m_dwSysErrCode = 0;
	ResetCounters();

	memset(&m_oSend, 0, sizeof(OVERLAPPED));
	memset(&m_oRecv, 0, sizeof(OVERLAPPED));

	m_oSend.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_oRecv.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

void RT_COM::ResetCounters()
{ 
	m_u64Total_Rx_Bytes = 0;
	m_u64Total_Tx_Bytes = 0;
	m_u64Total_Rx_Packages = 0;
	m_u64Total_Tx_Packages = 0;
    m_u64Total_Rx_LostPackages = 0;
}

RT_COM::~RT_COM()
{
	if (m_oRecv.hEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_oRecv.hEvent);
        m_oRecv.hEvent = INVALID_HANDLE_VALUE;
	}

	if (m_oSend.hEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_oSend.hEvent);
        m_oSend.hEvent = INVALID_HANDLE_VALUE;
	}

	Close();
    if(m_pRxBuf)
    {
		delete [] m_pRxBuf;
    	m_pRxBuf = NULL;
    }
    if(m_pTxBuf)
    {
        delete [] m_pTxBuf;
        m_pTxBuf = NULL;
    }
}

DWORD RT_COM::FindAll(DWORD *dwDevName)
{  
	HKEY	hKey;
	DWORD	dwCnt = 0;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey)==ERROR_SUCCESS)
	{
		try
		{
			for(int idx=0; idx<32767; idx++)
			{
				TCHAR sName[256], sValue[256];
				DWORD dwNameLen=256, dwValueSize=sizeof(sValue);
				if(RegEnumValue(hKey, idx, sName, &dwNameLen, NULL, NULL, (LPBYTE)sValue, &dwValueSize)!=ERROR_SUCCESS)
					break;
				
				dwCnt ++;
				
				if(dwDevName != NULL)
				{
					if(*sValue)
					{
						TCHAR *szTemp = new TCHAR[sizeof(DWORD)];
						_tcscpy_s(szTemp, sizeof(DWORD)*sizeof(TCHAR), sValue+3);
						dwDevName[idx] = (DWORD)_ttoi(szTemp);
						delete [] szTemp;
					}
						
				}
			}
		}
		catch(...)
		{
			;
		}
			
		RegCloseKey(hKey);
		return dwCnt;
	}
	else
	{
		return 0;
	}
}

void RT_COM::SetRxPackageCallBack(RxCallBackFunc func, void *pParam)
{
	m_pRxPackageCallBackParam = pParam;
	m_RxPackageCallBack = func;
}

void RT_COM::SetRxBufCallBack(RxCallBackFunc func, void *pParam)
{
	m_pRxPBufCallBackParam = pParam;
	m_RxBufCallBack = func;
}

BOOL RT_COM::Open(DWORD dwPortNo, DWORD dwBaud)
{
	TCHAR sCom[20] = _T("\\\\.\\COM");
	TCHAR szPortNo[3];

	_itot_s(dwPortNo, szPortNo, sizeof(szPortNo), 10);
	_tcscat_s(sCom, 20, szPortNo);

	if(m_IsOpened)
	{
		if((dwPortNo == m_PortNo) && (dwBaud == m_DCB.BaudRate))
		{
			return TRUE;
		}
		else
		{
			Close();
		}
	}
	m_PortNo = dwPortNo;
	m_hComHandle = ::CreateFile(sCom,
		GENERIC_READ|GENERIC_WRITE,0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	if(m_hComHandle == INVALID_HANDLE_VALUE)
	{
		m_dwSysErrCode = GetLastError();
		CloseHandle(m_hComHandle);
		return FALSE;
	}

	PurgeComm(m_hComHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	if(SetupComm(m_hComHandle, m_MaxPackageSize, m_MaxPackageSize) == FALSE)
	{
		m_dwSysErrCode = GetLastError();
		CloseHandle(m_hComHandle);
		return FALSE;
	}

	m_DCB.DCBlength = sizeof(DCB);
	if(GetCommState(m_hComHandle, &m_DCB) == FALSE)
	{
		m_dwSysErrCode = GetLastError();
		CloseHandle(m_hComHandle);
		return FALSE;
	}
	else
	{
		m_DCB.BaudRate     = dwBaud;
		m_DCB.ByteSize     = 8;
		m_DCB.Parity       = NOPARITY;
		m_DCB.StopBits     = ONESTOPBIT;

		if(SetCommState(m_hComHandle, &m_DCB) == FALSE)
		{
			m_dwSysErrCode = GetLastError();
			CloseHandle(m_hComHandle);
			return FALSE;
		}
	}
	
	Sleep(60);
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = 1;
	TimeOuts.ReadTotalTimeoutMultiplier = 1;
	TimeOuts.ReadTotalTimeoutConstant = 1;

	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 1000;
	TimeOuts.WriteTotalTimeoutConstant = 50000;

	SetCommTimeouts(m_hComHandle,&TimeOuts);

	m_RecvThreadStopFlag = FALSE;
	m_rxThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)RecvThread,this,NULL,NULL);
	m_IsOpened = TRUE;
	return TRUE;
}

void RT_COM::Close()
{
	if(m_rxThread != INVALID_HANDLE_VALUE)
	{
		m_RecvThreadStopFlag = TRUE;
		WaitForSingleObject(m_rxThread,INFINITE);
		CloseHandle(m_rxThread);
		m_rxThread = INVALID_HANDLE_VALUE;
	}
	if(m_hComHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComHandle);
		Sleep(60);
		m_hComHandle = INVALID_HANDLE_VALUE;
	}
	m_IsOpened = FALSE;
}

//接包回调
void RT_COM::OnRecvPackage(const BYTE * byBuf, DWORD dwLen)
{
	if(m_RxPackageCallBack)
		m_RxPackageCallBack(m_pRxPackageCallBackParam, byBuf, dwLen);

	m_u64Total_Rx_Packages ++;
}

//接收缓冲回调
void RT_COM::OnRecvBuf(const BYTE * byBuf, DWORD dwLen)
{
	if(m_RxBufCallBack)
		m_RxBufCallBack(m_pRxPBufCallBackParam, byBuf, dwLen);

	m_u64Total_Rx_Bytes += dwLen;
}

//接收线程
void RT_COM::RecvThreadLoop()
{
	BYTE buf[1024];
	DWORD nr;
	DWORD dwRead;
	BOOL bResult;

	for(;;)
	{
		if (m_RecvThreadStopFlag)
			return;

		nr = 0;
		ResetEvent(m_oRecv.hEvent);
		bResult = ReadFile(m_hComHandle, buf, 1024, &nr, &m_oRecv);
		if(bResult == FALSE)
		{
			m_dwSysErrCode = GetLastError();
			if (m_dwSysErrCode == ERROR_IO_PENDING)
			{
				WaitForSingleObject(m_oRecv.hEvent, INFINITE);
				GetOverlappedResult(m_hComHandle, &m_oRecv, &dwRead, FALSE);

				if (dwRead != 0)
				{
					OnRecvBuf(buf, dwRead);
                    if(m_bUsePackage)
                    {
                        for(DWORD i = 0 ; i < dwRead ; i++)
                        {
                            if(AnalyzePackage(buf[i]))
                                OnRecvPackage(m_pRxBuf, m_RxPackageDataCount);
                        }
                    }
				}
			}
		}
		else
		{
			if(nr)
			{
				OnRecvBuf(buf, nr);
                if(m_bUsePackage)
                {
                    for(DWORD i = 0 ; i < nr ; i++)
                    {
                        if(AnalyzePackage(buf[i]))
                            OnRecvPackage(m_pRxBuf, m_RxPackageDataCount);
                    }
                }
			}
		}
	}
}

void RT_COM::feedData(const BYTE * byBuf, DWORD dwLen) {
    const BYTE * buf = byBuf;
    DWORD nr = dwLen;
    if(nr)
    {
        OnRecvBuf(buf, nr);
        if(m_bUsePackage)
        {
            for(DWORD i = 0 ; i < nr ; i++)
            {
                if(AnalyzePackage(buf[i]))
                    OnRecvPackage(m_pRxBuf, m_RxPackageDataCount);
            }
        }
    }
}

//解包函数
BOOL RT_COM::AnalyzePackage(BYTE data)
{
	static BYTE USART_LastByte = 0;
	static bool USART_BeginFlag = FALSE;
	static bool USART_CtrlFlag = FALSE;
	static DWORD USART_RevOffset = 0;
	static BYTE CheckSum = 0;

	if(((data == m_FrameHead) && (USART_LastByte == m_FrameHead)) || (USART_RevOffset > m_MaxPackageSize))
	{
    	if(USART_RevOffset < 17 && USART_RevOffset > 0)
        {
            m_u64Total_Rx_LostPackages ++;
        }
		//RESET
		USART_RevOffset = 0;
		USART_BeginFlag = TRUE;
		USART_LastByte = data ;
		return FALSE;
	}
	if( (data==m_FrameTail)&&(USART_LastByte==m_FrameTail)&&USART_BeginFlag )
	{
		USART_RevOffset--;//得到除去头尾和控制符的数据的个数
		m_RxPackageDataCount = USART_RevOffset - 1;
		CheckSum -= m_FrameTail;
		CheckSum -= m_pRxBuf[m_RxPackageDataCount];
		USART_LastByte = data;
		USART_BeginFlag = FALSE; 
		if(CheckSum == m_pRxBuf[m_RxPackageDataCount])
		{                          		   		
			CheckSum = 0;
			return TRUE;
		}
        m_u64Total_Rx_LostPackages ++;

		CheckSum = 0;                    
		return FALSE;
	}
	USART_LastByte = data ;
	if(USART_BeginFlag)
	{
		if(USART_CtrlFlag)
		{
			m_pRxBuf[USART_RevOffset++] = data;
			CheckSum += data;
			USART_CtrlFlag = FALSE;
			USART_LastByte = m_FrameCtrl;
		}
		else if(data == m_FrameCtrl)
		{
			USART_CtrlFlag = TRUE;
		}
		else
		{
			m_pRxBuf[USART_RevOffset++] = data;
			CheckSum += data;
		}
	}

	return FALSE;
}

//打包函数
DWORD RT_COM::WritePackage(BYTE * buf, DWORD len, DWORD dwTimeout)
{

	DWORD i;
	BYTE *pBuf;
	BYTE CheckSum = 0;

	if (buf == NULL)
	{
		return FALSE;
	}

 	if(m_bUsePackage == FALSE)
    	return 0;

	pBuf = m_pTxBuf;
	*pBuf++ = m_FrameHead;
	*pBuf++ = m_FrameHead;

	for (i=0 ; i < len ; i++)
	{
		if ((buf[i] == m_FrameCtrl)|| (buf[i] == m_FrameHead) || (buf[i] == m_FrameTail))
		{
			*pBuf++ = m_FrameCtrl;
		}
		*pBuf++ = buf[i];
		CheckSum += buf[i];
	}

	//校验和
	if ((CheckSum == m_FrameCtrl) || (CheckSum == m_FrameHead) || (CheckSum == m_FrameTail))
	{
		*pBuf++ = m_FrameCtrl;
	}
	*pBuf++ = CheckSum;

	//Send Tail USART_FRAMETAIL USART_FRAMETAIL
	*pBuf++ = m_FrameTail;
	*pBuf++ = m_FrameTail;

	len =  pBuf - m_pTxBuf;

	DWORD nw = WriteBuf(m_pTxBuf, len, dwTimeout);
	
	if(nw) m_u64Total_Tx_Packages ++;	
	
	return nw;
}

//写缓存函数
DWORD RT_COM::WriteBuf(const BYTE *byBuf, DWORD dwLen, DWORD dwTimeout /* = INFINITE */)
{
	DWORD nw = 0;
	
	if(byBuf == NULL)
		return 0;

	if(!IsOpened())
		return 0;


	ResetEvent(m_oSend.hEvent);
	BOOL bResult = WriteFile(m_hComHandle, byBuf, dwLen, &nw, &m_oSend);

	if(!bResult)
	{
		m_dwSysErrCode = GetLastError();
		if (m_dwSysErrCode == ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_oSend.hEvent, dwTimeout);
			GetOverlappedResult(m_hComHandle, &m_oSend, &nw, FALSE);
		}
		else
			return 0;
	}

	m_u64Total_Tx_Bytes += nw;
	return nw;
}


