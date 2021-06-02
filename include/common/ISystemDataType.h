#ifndef ISYSTEMDATATYPE_DEF_H
#define ISYSTEMDATATYPE_DEF_H

#include "Define.h"
#include "Type.h"

#ifdef _WIN32
#elif _LINUX
#include <semaphore.h>
#endif

/******************************************************************************/
/********************************数据结构定义*********************** ***/
/******************************************************************************/

#define DLP_FILE_FIFO		0x1000  //管道文件
#define DLP_FILE_OTHER		0x2000	//其他类型	
#define DLP_FILE_DIRECTORY   0x4000	//目录
#define DLP_FILE_COMMON		0x8000  //普通文件

/*文件信息结构*/
typedef struct _StruDLPFileInfo
{
	std::string strFileName;		//文件名
	INT	  iFileType;				//文件类型 :目录，普通文件，管道文件，其他类型
}StruDLPFileInfo, *StruDLPFileInfoPtr;


/* 系统时间结构体 */
typedef struct _StruSysTime
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
}StruSysTime, *StruSysTimePtr;

/******************************************************************************/
/********************************网络信息获取数据结构定义*********************** ***/
/******************************************************************************/
//用来转换字节为1K
#define DIV 1024
#define	DLP_MAX_NET_NAME_LEN	256
#define	MAX_NET_COUNTS		16

typedef struct StruDLPNETSTATTable
{	
	char szName[DLP_MAX_NET_NAME_LEN]; //网卡描述名称
	INT64 iRecv;					//总共接收字节数
	INT64 iTrans;					//总共传送字节数
	double dRecvSpeed;				//网络接收速度
	double dTransSpeed;				//网络传送速度

}StruDLPNETSTATTable,*StruDLPNETSTATTablePtr;

typedef struct StruDLPNETSTAT
{	
	INT32				iNetNum;			//网卡数量
	StruDLPNETSTATTable	stNetStatTable[MAX_NET_COUNTS];	//网卡流量信息
}StruDLPNETSTAT,*StruDLPNETSTATPtr;
/******************************************************************************/
/********************************线程相关元素定义*********************** ***/
/******************************************************************************/

#ifdef _WIN32

#ifndef  DLP_INIT_SEM_COUNT						
#define  DLP_INIT_SEM_COUNT  (LONG)1
#endif
#ifndef	 DLP_MAX_SEM_COUNT
#define  DLP_MAX_SEM_COUNT   (LONG)1
#endif

#define CAN_NOT_READ_SIGN   0x80000000
#define CAN_NOT_WRITE_SIGN  0x40000000
#define READ_COUNT_MASK     0x3fffffff

#define DLPThread			HANDLE				//线程类型
#define DLPProcessMutex		HANDLE				//进程锁
#define DLPMutex				CRITICAL_SECTION	//线程锁
#define DLPRwmutex			CRITICAL_SECTION	//读写锁
#define DLPCond				HANDLE				//条件变量
#define DLPSem				HANDLE				//信号量

#elif _LINUX

#define DLPThread			pthread_t
#define DLPProcessMutex		INT
#define DLPMutex				pthread_mutex_t
#define DLPRwmutex			pthread_rwlock_t
#define DLPCond				pthread_cond_t
#define DLPSem				sem_t*


#endif


#endif // ISYSTEMDATATYPE_DEF_H
