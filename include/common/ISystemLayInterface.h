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
  Description 	:     // 实现系统封装
						1）线程		CDLPThread类
						2）普通锁	CDLPMutex类
						3）自动锁	CDLPAutoMutex类
						4）进程锁	CDLPProcessMutex类
						5）读写锁	CDLPWRMutex类
						6）信号量	CDLPSem类
						7）条件变量	CDLPCond类
						8）目录枚举 CDLPDir类
						9）枚举磁盘	DLPGetSysDisk（）
						10）时间函数 DoGetTickCount()， DoGetLocalTime（）
********************************************************************************************/

/*******************************************************************************
功能说明：线程

使用说明：线程函数在调用时，采用如下方式：
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
  Function		: 线程函数的定义    
  DateTime		: 	
  Description	: 线程函数定义
  Input			: CDLPThread *DLPThreadHandle：线程句柄
					void *pUserParam：		用户参数
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
	virtual BOOL Start(DLPThreadCallbackFunction fnOnEvent=NULL, void *pUserParam=NULL);	 //开始线程，成功返回TRUE,失败返回FALSE	
	virtual BOOL Stop();	//停止线程，成功返回TRUE,失败返回FALSE	
	virtual BOOL TestExit();	//判断线程是否退出，在线程函数的循环中调用，执行暂停和唤醒操作	
	BOOL Join();	//等待线程结束，成功返回TRUE,失败返回FALSE	
	BOOL Join(INT mseconds);	//等待一段时间，让线程结束,成功返回TRUE,失败返回FALSE	
	BOOL Suspend();		//线程暂停,成功返回TRUE,失败返回FALSE	
	BOOL Resume();		//唤醒线程,成功返回TRUE,失败返回FALSE	
	BOOL IsRunning ();	//返回线程运行状态，TRUE为正在运行，FALSE为未运行
	void Cancel();		//强制退出
	DLPThread GetThreadHandle(); //获取线程句柄，返回获取的句柄，若句柄不存在，返回NULL
	void UnInitData();	//释放线程句柄

protected:
	void Kill(void);	//强制线程退出	
	void Detach(void);	//linux下分离线程	

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
功能说明：普通锁
*******************************************************************************/
class CDLPMutex
{
public:
	CDLPMutex();
	~CDLPMutex(void);
	BOOL	Lock();		//加锁，成功返回TRUE，失败返回FALSE  
	void	Unlock();	//解锁，成功返回TRUE,失败返回FALSE
	BOOL	TryLock();	//非阻塞的加锁，成功返回TRUE,失败返回FALSE

public:
	DLPMutex m_DLPMutex;
};

/*******************************************************************************
功能说明：进程间的锁
*******************************************************************************/
class CDLPProcessMutex
{
public:
	CDLPProcessMutex( const char *czKey);
	~CDLPProcessMutex(void);
	BOOL	LockProcess();  //加锁，成功返回TRUE,失败返回FALSE  
	void	UnlockProcess(); //解锁，成功返回TRUE,失败返回FALSE
	BOOL	TryLockProcess();//非阻塞的加锁，成功返回TRUE,失败返回FALSE

private:
	DLPProcessMutex m_DLPProcessMutex;
};

/*******************************************************************************
功能说明：信号量
*******************************************************************************/
class CDLPSem
{
public:
	CDLPSem( const char *czKey = NULL,BOOL bProcess=TRUE ); //输入参数：const char *czKey ：信号量的名称
															//BOOL bProcess：是否是进程间的信号量，TRUE:是，FALSE:否
	~CDLPSem();
	BOOL IsValid( void ); //判断信号量是否有效,成功返回TRUE,失败返回FALSE
	BOOL Signal(); //发送信号量，成功返回TRUE,失败返回FALSE
	BOOL Wait(); //等待信号量，成功返回TRUE,失败返回FALSE
	BOOL Wait( UINT mSeconds ); //等待信号量一定时间，UINT mSeconds：等待的时间，成功返回TRUE,失败返回FALSE

private:
	DLPSem m_DLPSem;
	BOOL  m_bIsValid;
	
};

/*******************************************************************************
功能说明：读写锁
*******************************************************************************/
class CDLPWRMutex
{
public:
	CDLPWRMutex();
	~CDLPWRMutex(void);
	BOOL	LockReader();     //加锁读操作，成功返回TRUE,失败返回FALSE 
	BOOL	TryLockReader();  //非阻塞的加锁读操作，成功返回TRUE,失败返回FALSE  
	void	UnlockReader(); //解锁读操作
	BOOL	LockWrite();    //加锁写操作，成功返回TRUE,失败返回FALSE
	BOOL	TryLockWrite(); //非阻塞的加锁写操作，成功返回TRUE,失败返回FALSE
	void	UnlockWrite(); //解锁写操作

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
功能说明：自动锁
*******************************************************************************/
class CDLPAutoMutex
{

public :
	CDLPAutoMutex( CDLPMutex *locker ); //加锁
	~CDLPAutoMutex(void); //解锁

private:
	CDLPMutex *m_locker;

};

/*******************************************************************************
功能说明：条件变量
*******************************************************************************/
class CDLPCond
{
public:
	CDLPCond();
	~CDLPCond(void);
	INT	Wait(); //等待条件变量， 返回0为成功，其他表示失败  
	INT	WaitTimeout(INT mseconds); //等待条件变量一定时间，mseconds：等待条件变量的时间，返回0为成功，其他表示失败
	INT	Signal(); //发送信号，返回0为成功，其他表示失败
	INT	BroadcastSignal(); //发送广播信号，返回0为成功，其他表示失败

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
	//返回值
	static const INT R_SUCCESS = 0; //成功
	static const INT R_ESYSTEM = 1; //超时
	static const INT R_ETIMEOUT = 2; //系统函数调用错误



	CDLPCondEx(void);
	~CDLPCondEx(void);
	INT	Wait( CDLPMutex *pMutex ); //等待条件变量, 	 ;
	INT	WaitTimeout(CDLPMutex *pMutex, INT mseconds); //等待条件变量一定时间，
	//mseconds：等待条件变量的时间，
	//超时返回 R_ETIMEOUT	
	INT	Signal(void); //发送信号，
	INT	BroadcastSignal(void); //发送广播信号，返回0为成功，其他表示失败
};


/******************************************************************************
功能说明：目录枚举
******************************************************************************/
class CDLPDir
{
public:
	CDLPDir();
	~CDLPDir(void);
	BOOL	OpenDir(const char *czDirPath);   //打开指定路径的目录，成功返回TRUE,失败返回FALSE 
	void	CloseDir(void);						//关闭目录
	void	ReadDir(std::vector<StruDLPFileInfo> &vectFileList); //读取目录信息，输入参数std::vector<StruDLPFileInfo> &vectFileList

private:
	HANDLE	m_DLPDir;

#ifdef WINCE
private:
	WCHAR	m_wczDirPath[MAX_PATH];
#endif

};

/******************************************************************************
功能说明：进程信息
******************************************************************************/
class CDLPProcessInfo
{
public:
	CDLPProcessInfo();
	~CDLPProcessInfo(void);
	INT32	DLPGetTotalCPUUsage(INT32 &iCPU);   //获取总的CPU使用率，返回CPU使用率，返回-1，获取失败
	INT32	DLPGetTotalMemoryUsage(DWORD &dwMem);	//获取总的内存使用率

private:
#ifdef _WIN32
	INT64 CompareFileTime (FILETIME time1, FILETIME time2);
#endif
	//获得CPU的核数
	INT32	DLPGetProcessorNumber(); 

private:
	//cpu数量
	INT32 m_processor_count;
};
/******************************************************************************
功能说明：网络流量获取
******************************************************************************/
class CDLPNetInfo
{
public:
	CDLPNetInfo();
	~CDLPNetInfo(void);
	INT32	DLPGetNetCount();	//获取网卡个数，返回网卡个数，失败返回0或者负数
	INT32	DLPGetNetUsage(StruDLPNETSTAT * pstNetStat,INT32 iBuffLen);		//获取网络流量参数
private:
	StruDLPNETSTATTable* GetLastNetStat(const char* szName);
private:
	UINT64				m_uiLastTime;		//上一次获取流量的时间
	INT32				m_iNetCounts;		//网卡个数
	StruDLPNETSTATTable	m_stLastNetStat[MAX_NET_COUNTS];//上一次各个网卡的流量
	char*				m_pDataBuf;
	INT32				m_iBufLen;
};
/*******************************************************************************
功能说明：枚举磁盘
*******************************************************************************/

EXPORT_API  INT	DLPGetSysDisk(std::vector<std::string> &vectDiskList); //获取磁盘信息，存放在磁盘表中，返回磁盘个数

/*******************************************************************************
功能说明：系统时间
*******************************************************************************/
EXPORT_API  UINT64   DoGetTickCount(); //获取从操作系统启动到现在所经过的毫秒数，返回从操作系统启动到现在所经过的毫秒数

EXPORT_API  void    DoGetLocalTime(StruSysTimePtr pLoaltime); //获取当地的当前系统日期和时间，存放在包含日期和时间信息的结构中

// 对iVal 进行递增操作， 返回增加后的值
EXPORT_API long AtomicInterInc( DLPAtomicInter &iVal );
// 对iVal 进行递减操作， 返回递减后的值
EXPORT_API long AtomicInterDec( DLPAtomicInter &iVal );

//比较iVal 释放 以iOldVal 相等， 如果相等，把 iVal 设定为 iNewVal, 并返回TRUE， 否则返回FALSE
EXPORT_API BOOL AtomicInterCompareExchange(DLPAtomicInter &iVal, const long iOldVal, const long iNewVal);

//设置 iVal 为 iNewVal, 返回 设定前 iVal 的值
EXPORT_API long AtomicInterSet(DLPAtomicInter &iVal, const long iNewVal );

//对 iVal 执行 AND 运算 iFlag, 返回 设定前 iVal 的值
EXPORT_API long AtomicInterAnd(DLPAtomicInter &iVal, const long iFlag );

//对 iVal 执行 Or 运算 iFlag, 返回 设定前 iVal 的值
EXPORT_API long AtomicInterOr(DLPAtomicInter &iVal, const long iFlag );

//返回当前引用程序的所在的目录
CDLPString DLPGetApplicationPath(void);

//格式化目录
void DLPPathParser( CDLPString &strPath);

//检测czPath 是否存在， 如果不存在 将创建改目录
BOOL DLPTestAndCreateDir( const char *czPath);


#ifndef _WIN32
#define  localtime_s localtime_r
#endif

#endif // ISYSTEMLAYINTERFACE_DEF_H
