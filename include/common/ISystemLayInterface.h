#ifndef ISYSTEMLAYINTERFACE_DEF_H
#define ISYSTEMLAYINTERFACE_DEF_H

#if !defined(_DEBUG) && !defined(DEBUG)
#ifndef NDEBUG
#	define NDEBUG
#endif
#endif

#include <assert.h>


#include "ISystemDataType.h"
#ifdef WINCE
#include <windows.h>
#elif	_WIN32	
#include <process.h>
#endif


/********************************************************************************************
  Description 	:     // ʵ��ϵͳ��װ
						1���߳�		CDLPThread��
						2����ͨ��	CDLPMutex��
						3���Զ���	CDLPAutoMutex��
						4��������	CDLPProcessMutex��
						5����д��	CDLPWRMutex��
						6���ź���	CDLPSem��
						7����������	CDLPCond��
						8��Ŀ¼ö�� CDLPDir��
						9��ö�ٴ���	DLPGetSysDisk����
						10��ʱ�亯�� DoGetTickCount()�� DoGetLocalTime����
********************************************************************************************/

/*******************************************************************************
����˵�����߳�

ʹ��˵�����̺߳����ڵ���ʱ���������·�ʽ��
void threadfun(CDLPThread *DLPThreadHandle, void *pUserParam)
{
	while(!DLPThreadHandle->TestExit()&&...)
	{
		...
	}
}
*******************************************************************************/
class CDLPThread;
class CDLPCond;
class CDLPMutex;

/********************************************************************************************
  Function		: �̺߳����Ķ���    
  DateTime		: 	
  Description	: �̺߳�������
  Input			: CDLPThread *DLPThreadHandle���߳̾��
					void *pUserParam��		�û�����
  Output		: NULL
  Return		: NULL
  Note			: NULL	
********************************************************************************************/
typedef void (*DLPThreadCallbackFunction)(CDLPThread *DLPThreadHandle, void *pUserParam );

/*#ifdef _WIN32
extern DWORD WINAPI CDLPThreadProcFunc(HANDLE param);
#elif _LINUX
extern void *CDLPThreadProcFunc(HANDLE param);
#endif*/ 

class CDLPThread
{
//public:
//#ifdef _WIN32
//	friend DWORD WINAPI CDLPThreadProcFunc(HANDLE param);
//#elif _LINUX
//	friend void *CDLPThreadProcFunc(HANDLE param);
//#endif 
	
public:
	CDLPThread();
	virtual ~CDLPThread(void);
	virtual BOOL Start(DLPThreadCallbackFunction fnOnEvent=NULL, void *pUserParam=NULL);	 //��ʼ�̣߳��ɹ�����TRUE,ʧ�ܷ���FALSE	
	virtual BOOL Stop();	//ֹͣ�̣߳��ɹ�����TRUE,ʧ�ܷ���FALSE	
	virtual BOOL TestExit();	//�ж��߳��Ƿ��˳������̺߳�����ѭ���е��ã�ִ����ͣ�ͻ��Ѳ���	
	BOOL Join();	//�ȴ��߳̽������ɹ�����TRUE,ʧ�ܷ���FALSE	
	BOOL Join(INT mseconds);	//�ȴ�һ��ʱ�䣬���߳̽���,�ɹ�����TRUE,ʧ�ܷ���FALSE	
	BOOL Suspend();		//�߳���ͣ,�ɹ�����TRUE,ʧ�ܷ���FALSE	
	BOOL Resume();		//�����߳�,�ɹ�����TRUE,ʧ�ܷ���FALSE	
	BOOL IsRunning ();	//�����߳�����״̬��TRUEΪ�������У�FALSEΪδ����
	void Cancel();		//ǿ���˳�
	DLPThread GetThreadHandle(); //��ȡ�߳̾�������ػ�ȡ�ľ��������������ڣ�����NULL
	void UnInitData();	//�ͷ��߳̾��

protected:
	void Kill(void);	//ǿ���߳��˳�	
	void Detach(void);	//linux�·����߳�	

public:
	BOOL m_bRunninDLPtate;
	BOOL m_bExit;
	DLPThreadCallbackFunction m_fnUser;
	void *m_pFnUserParam;
	CDLPMutex *m_DLPMutexUnit;
	BOOL m_bMutexLock;
	CDLPCond  *m_DLPCond;	

	BOOL	m_bthreadfinish;
	CDLPMutex *m_DLPMutexthreadfinish;

protected:
	DLPThread m_DLPThread;
	CDLPCond	 *m_DLPCondPause;
	BOOL m_bExiting;
	BOOL m_bPause;
	BOOL m_bJoin;
	BOOL m_bDetached;
	CDLPMutex *m_DLPMutexPause;

};




/*******************************************************************************
����˵������ͨ��
*******************************************************************************/
class CDLPMutex
{
public:
	CDLPMutex();
	~CDLPMutex(void);
	BOOL	Lock();		//�������ɹ�����TRUE��ʧ�ܷ���FALSE  
	void	Unlock();	//�������ɹ�����TRUE,ʧ�ܷ���FALSE
	BOOL	TryLock();	//�������ļ������ɹ�����TRUE,ʧ�ܷ���FALSE

public:
	DLPMutex m_DLPMutex;
};

/*******************************************************************************
����˵�������̼����
*******************************************************************************/
class CDLPProcessMutex
{
public:
	CDLPProcessMutex( const char *czKey);
	~CDLPProcessMutex(void);
	BOOL	LockProcess();  //�������ɹ�����TRUE,ʧ�ܷ���FALSE  
	void	UnlockProcess(); //�������ɹ�����TRUE,ʧ�ܷ���FALSE
	BOOL	TryLockProcess();//�������ļ������ɹ�����TRUE,ʧ�ܷ���FALSE

private:
	DLPProcessMutex m_DLPProcessMutex;
};

/*******************************************************************************
����˵�����ź���
*******************************************************************************/
class CDLPSem
{
public:
	CDLPSem( const char *czKey = NULL,BOOL bProcess=TRUE ); //���������const char *czKey ���ź���������
															//BOOL bProcess���Ƿ��ǽ��̼���ź�����TRUE:�ǣ�FALSE:��
	~CDLPSem();
	BOOL IsValid( void ); //�ж��ź����Ƿ���Ч,�ɹ�����TRUE,ʧ�ܷ���FALSE
	BOOL Signal(); //�����ź������ɹ�����TRUE,ʧ�ܷ���FALSE
	BOOL Wait(); //�ȴ��ź������ɹ�����TRUE,ʧ�ܷ���FALSE
	BOOL Wait( UINT mSeconds ); //�ȴ��ź���һ��ʱ�䣬UINT mSeconds���ȴ���ʱ�䣬�ɹ�����TRUE,ʧ�ܷ���FALSE

private:
	DLPSem m_DLPSem;
	BOOL  m_bIsValid;
	
};

/*******************************************************************************
����˵������д��
*******************************************************************************/
class CDLPWRMutex
{
public:
	CDLPWRMutex();
	~CDLPWRMutex(void);
	BOOL	LockReader();     //�������������ɹ�����TRUE,ʧ�ܷ���FALSE 
	BOOL	TryLockReader();  //�������ļ������������ɹ�����TRUE,ʧ�ܷ���FALSE  
	void	UnlockReader(); //����������
	BOOL	LockWrite();    //����д�������ɹ�����TRUE,ʧ�ܷ���FALSE
	BOOL	TryLockWrite(); //�������ļ���д�������ɹ�����TRUE,ʧ�ܷ���FALSE
	void	UnlockWrite(); //����д����

private:
#ifdef WINCE
	BOOL DLPStopReadWaitSetWrite();
	HANDLE m_DLPReadEvent;
	HANDLE m_DLPWriteEvent;
	LONG m_readCount;
	LONG m_writeCount;
#elif _WIN32
	BOOL DLPStopReadWaitSetWrite();
	HANDLE m_DLPReadEvent;
	HANDLE m_DLPWriteEvent;
	volatile LONG m_readCount;
	volatile LONG m_writeCount;
#ifdef _DEBUG
#define _MUTEX_DEBUG  
#endif

#ifdef _MUTEX_DEBUG
	volatile LONG m_iRRefDebug;
	volatile LONG m_iWRefDebug;
#endif
#elif _LINUX
	DLPRwmutex	m_DLPRwmutex;
#endif
};

class CDLPAutoReaderMutex
{
private :
	CDLPWRMutex *m_pMutex;
public :
	CDLPAutoReaderMutex(CDLPWRMutex *pWRMutex )
		: m_pMutex(pWRMutex)
	{
		m_pMutex->LockReader();
	}
	~CDLPAutoReaderMutex(void)
	{
		m_pMutex->UnlockReader();
	}

};

class CDLPAutoWriterMutex
{
private :
	CDLPWRMutex *m_pMutex;
public :
	CDLPAutoWriterMutex(CDLPWRMutex *pWRMutex )
		: m_pMutex(pWRMutex)
	{
		m_pMutex->LockWrite();
	}
	~CDLPAutoWriterMutex(void)
	{
		m_pMutex->UnlockWrite();
	}

};

/*******************************************************************************
����˵�����Զ���
*******************************************************************************/
class CDLPAutoMutex
{

public :
	CDLPAutoMutex( CDLPMutex *locker ); //����
	~CDLPAutoMutex(void); //����

private:
	CDLPMutex *m_locker;

};

/*******************************************************************************
����˵������������
*******************************************************************************/
class CDLPCond
{
public:
	CDLPCond();
	~CDLPCond(void);
	INT	Wait(); //�ȴ����������� ����0Ϊ�ɹ���������ʾʧ��  
	INT	WaitTimeout(INT mseconds); //�ȴ���������һ��ʱ�䣬mseconds���ȴ�����������ʱ�䣬����0Ϊ�ɹ���������ʾʧ��
	INT	Signal(); //�����źţ�����0Ϊ�ɹ���������ʾʧ��
	INT	BroadcastSignal(); //���͹㲥�źţ�����0Ϊ�ɹ���������ʾʧ��

private:
	DLPCond m_DLPCond;
#ifdef _WIN32
	HANDLE m_mutex; 
#elif _LINUX
	CDLPMutex *m_CondMutex;
#endif


};


class CDLPCondEx
{
private :
#ifdef _WIN32
	HANDLE m_hEvent;
	INT m_iWaitConts; 
#else
	pthread_cond_t  m_hEvent;
#endif

public :
	//����ֵ
	static const INT R_SUCCESS = 0; //�ɹ�
	static const INT R_ESYSTEM = 1; //��ʱ
	static const INT R_ETIMEOUT = 2; //ϵͳ�������ô���



	CDLPCondEx(void);
	~CDLPCondEx(void);
	INT	Wait( CDLPMutex *pMutex ); //�ȴ���������, 	 ;
	INT	WaitTimeout(CDLPMutex *pMutex, INT mseconds); //�ȴ���������һ��ʱ�䣬
	//mseconds���ȴ�����������ʱ�䣬
	//��ʱ���� R_ETIMEOUT	
	INT	Signal(void); //�����źţ�
	INT	BroadcastSignal(void); //���͹㲥�źţ�����0Ϊ�ɹ���������ʾʧ��
};


/******************************************************************************
����˵����Ŀ¼ö��
******************************************************************************/
class CDLPDir
{
public:
	CDLPDir();
	~CDLPDir(void);
	BOOL	OpenDir(const char *czDirPath);   //��ָ��·����Ŀ¼���ɹ�����TRUE,ʧ�ܷ���FALSE 
	void	CloseDir(void);						//�ر�Ŀ¼
	void	ReadDir(std::vector<StruDLPFileInfo> &vectFileList); //��ȡĿ¼��Ϣ���������std::vector<StruDLPFileInfo> &vectFileList

private:
	HANDLE	m_DLPDir;

#ifdef WINCE
private:
	WCHAR	m_wczDirPath[MAX_PATH];
#endif

};

/******************************************************************************
����˵����������Ϣ
******************************************************************************/
class CDLPProcessInfo
{
public:
	CDLPProcessInfo();
	~CDLPProcessInfo(void);
	INT32	DLPGetTotalCPUUsage(INT32 &iCPU);   //��ȡ�ܵ�CPUʹ���ʣ�����CPUʹ���ʣ�����-1����ȡʧ��
	INT32	DLPGetTotalMemoryUsage(DWORD &dwMem);	//��ȡ�ܵ��ڴ�ʹ����

private:
#ifdef _WIN32
	INT64 CompareFileTime (FILETIME time1, FILETIME time2);
#endif
	//���CPU�ĺ���
	INT32	DLPGetProcessorNumber(); 

private:
	//cpu����
	INT32 m_processor_count;
};
/******************************************************************************
����˵��������������ȡ
******************************************************************************/
class CDLPNetInfo
{
public:
	CDLPNetInfo();
	~CDLPNetInfo(void);
	INT32	DLPGetNetCount();	//��ȡ������������������������ʧ�ܷ���0���߸���
	INT32	DLPGetNetUsage(StruDLPNETSTAT * pstNetStat,INT32 iBuffLen);		//��ȡ������������
private:
	StruDLPNETSTATTable* GetLastNetStat(const char* szName);
private:
	UINT64				m_uiLastTime;		//��һ�λ�ȡ������ʱ��
	INT32				m_iNetCounts;		//��������
	StruDLPNETSTATTable	m_stLastNetStat[MAX_NET_COUNTS];//��һ�θ�������������
	char*				m_pDataBuf;
	INT32				m_iBufLen;
};
/*******************************************************************************
����˵����ö�ٴ���
*******************************************************************************/

EXPORT_API  INT	DLPGetSysDisk(std::vector<std::string> &vectDiskList); //��ȡ������Ϣ������ڴ��̱��У����ش��̸���

/*******************************************************************************
����˵����ϵͳʱ��
*******************************************************************************/
EXPORT_API  UINT64   DoGetTickCount(); //��ȡ�Ӳ���ϵͳ�����������������ĺ����������شӲ���ϵͳ�����������������ĺ�����

EXPORT_API  void    DoGetLocalTime(StruSysTimePtr pLoaltime); //��ȡ���صĵ�ǰϵͳ���ں�ʱ�䣬����ڰ������ں�ʱ����Ϣ�Ľṹ��

// ��iVal ���е��������� �������Ӻ��ֵ
EXPORT_API long AtomicInterInc( DLPAtomicInter &iVal );
// ��iVal ���еݼ������� ���صݼ����ֵ
EXPORT_API long AtomicInterDec( DLPAtomicInter &iVal );

//�Ƚ�iVal �ͷ� ��iOldVal ��ȣ� �����ȣ��� iVal �趨Ϊ iNewVal, ������TRUE�� ���򷵻�FALSE
EXPORT_API BOOL AtomicInterCompareExchange(DLPAtomicInter &iVal, const long iOldVal, const long iNewVal);

//���� iVal Ϊ iNewVal, ���� �趨ǰ iVal ��ֵ
EXPORT_API long AtomicInterSet(DLPAtomicInter &iVal, const long iNewVal );

//�� iVal ִ�� AND ���� iFlag, ���� �趨ǰ iVal ��ֵ
EXPORT_API long AtomicInterAnd(DLPAtomicInter &iVal, const long iFlag );

//�� iVal ִ�� Or ���� iFlag, ���� �趨ǰ iVal ��ֵ
EXPORT_API long AtomicInterOr(DLPAtomicInter &iVal, const long iFlag );

//���ص�ǰ���ó�������ڵ�Ŀ¼
CDLPString DLPGetApplicationPath(void);

//��ʽ��Ŀ¼
void DLPPathParser( CDLPString &strPath);

//���czPath �Ƿ���ڣ� ��������� ��������Ŀ¼
BOOL DLPTestAndCreateDir( const char *czPath);


#ifndef _WIN32
#define  localtime_s localtime_r
#endif

#endif // ISYSTEMLAYINTERFACE_DEF_H
