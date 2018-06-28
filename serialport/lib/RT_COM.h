#ifndef __RT_COM_H
#define __RT_COM_H

#include <Windows.h>
#include "tchar.h"

//#ifndef _MSC_VER  //如果不是Visual Studio的编译环境，则如下的函数不能得到支持

//#define _itot_s(value, buffer, sizeInCharacters, radix) _itot(value, buffer, radix)
//#define _tcscat_s(strDestination, numberOfElements, strSource) _tcscat(strDestination, strSource)
//#define _tcscpy_s(strDestination, numberOfElements, strSource) _tcscpy(strDestination, strSource)

//#endif


typedef void (*RxCallBackFunc)(void *pParam, const BYTE * byBuf, DWORD dwLen);

class RT_COM
{
public:
	RT_COM(DWORD dwMaxPackageSize = 4096, BOOL UsePackage = TRUE);
	~RT_COM();
	static DWORD FindAll(DWORD *dwDevName);
	BOOL Open(DWORD dwPortNo, DWORD dwBaud);
	void Close();

	void SetRxPackageCallBack(RxCallBackFunc func, void *pParam);
	void SetRxBufCallBack(RxCallBackFunc func, void *pParam);
	DWORD GetSysErrCode(){ return m_dwSysErrCode;}
	DWORD WritePackage(BYTE * byBuf, DWORD dwLen, DWORD dwTimeout = INFINITE);
	DWORD WriteBuf(const BYTE *byBuf, DWORD dwLen, DWORD dwTimeout = INFINITE);

	BOOL IsOpened(){ return m_IsOpened; }
	DWORD GetBaudRate(){ return m_DCB.BaudRate;}
	DWORD GetPortNo(){ return m_PortNo;}

	UINT64 GetTotalRxByteCount(){ return m_u64Total_Rx_Bytes; }
	UINT64 GetTotalTxByteCount(){ return m_u64Total_Tx_Bytes; }
	UINT64 GetTotalRxPackageCount(){ return m_u64Total_Rx_Packages; }
	UINT64 GetTotalTxPackageCount(){ return m_u64Total_Tx_Packages; }
    UINT64 GetTotalRxLostPackageCount(){ return m_u64Total_Rx_LostPackages; }
	void ResetCounters();

    void EnablePackage(BOOL flag = TRUE) {m_bUsePackage = flag;}
    BOOL IsUsePackage() {return m_bUsePackage;}

protected:
	virtual void OnRecvPackage(const BYTE * byBuf, DWORD dwLen);
	virtual void OnRecvBuf(const BYTE * byBuf, DWORD dwLen);

private:
	UINT64 m_u64Total_Rx_Bytes;
	UINT64 m_u64Total_Rx_Packages;
	UINT64 m_u64Total_Tx_Packages;
    UINT64 m_u64Total_Tx_Bytes;
	UINT64 m_u64Total_Rx_LostPackages;

	void *m_pRxPackageCallBackParam;
	void *m_pRxPBufCallBackParam;
	RxCallBackFunc m_RxPackageCallBack;
	RxCallBackFunc m_RxBufCallBack;

	OVERLAPPED m_oSend;
	OVERLAPPED m_oRecv;

	HANDLE m_hComHandle;
	HANDLE m_rxThread;

	BYTE m_FrameHead, m_FrameTail, m_FrameCtrl;
	
	BOOL m_RecvThreadStopFlag;
	BOOL m_IsOpened;
	DCB m_DCB;
	DWORD m_MaxPackageSize;
	BYTE * m_pRxBuf;
	BYTE * m_pTxBuf;
	DWORD m_RxPackageDataCount;	
	DWORD m_PortNo;
	DWORD m_dwSysErrCode;
    BOOL m_bUsePackage;

	void RecvThreadLoop();
	static DWORD RecvThread(LPVOID lpParameter)
	{
		RT_COM *pGComm = (RT_COM *)lpParameter;
		pGComm->RecvThreadLoop();
		return 0;
	}

    BOOL AnalyzePackage(BYTE data);

public:
    void feedData(const BYTE * byBuf, DWORD dwLen);
} ;


#endif
