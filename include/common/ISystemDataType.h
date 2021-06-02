#ifndef ISYSTEMDATATYPE_DEF_H
#define ISYSTEMDATATYPE_DEF_H

#include "Define.h"
#include "Type.h"

#ifdef _WIN32
#elif _LINUX
#include <semaphore.h>
#endif

/******************************************************************************/
/********************************���ݽṹ����*********************** ***/
/******************************************************************************/

#define DLP_FILE_FIFO		0x1000  //�ܵ��ļ�
#define DLP_FILE_OTHER		0x2000	//��������	
#define DLP_FILE_DIRECTORY   0x4000	//Ŀ¼
#define DLP_FILE_COMMON		0x8000  //��ͨ�ļ�

/*�ļ���Ϣ�ṹ*/
typedef struct _StruDLPFileInfo
{
	std::string strFileName;		//�ļ���
	INT	  iFileType;				//�ļ����� :Ŀ¼����ͨ�ļ����ܵ��ļ�����������
}StruDLPFileInfo, *StruDLPFileInfoPtr;


/* ϵͳʱ��ṹ�� */
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
/********************************������Ϣ��ȡ���ݽṹ����*********************** ***/
/******************************************************************************/
//����ת���ֽ�Ϊ1K
#define DIV 1024
#define	DLP_MAX_NET_NAME_LEN	256
#define	MAX_NET_COUNTS		16

typedef struct StruDLPNETSTATTable
{	
	char szName[DLP_MAX_NET_NAME_LEN]; //������������
	INT64 iRecv;					//�ܹ������ֽ���
	INT64 iTrans;					//�ܹ������ֽ���
	double dRecvSpeed;				//��������ٶ�
	double dTransSpeed;				//���紫���ٶ�

}StruDLPNETSTATTable,*StruDLPNETSTATTablePtr;

typedef struct StruDLPNETSTAT
{	
	INT32				iNetNum;			//��������
	StruDLPNETSTATTable	stNetStatTable[MAX_NET_COUNTS];	//����������Ϣ
}StruDLPNETSTAT,*StruDLPNETSTATPtr;
/******************************************************************************/
/********************************�߳����Ԫ�ض���*********************** ***/
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

#define DLPThread			HANDLE				//�߳�����
#define DLPProcessMutex		HANDLE				//������
#define DLPMutex				CRITICAL_SECTION	//�߳���
#define DLPRwmutex			CRITICAL_SECTION	//��д��
#define DLPCond				HANDLE				//��������
#define DLPSem				HANDLE				//�ź���

#elif _LINUX

#define DLPThread			pthread_t
#define DLPProcessMutex		INT
#define DLPMutex				pthread_mutex_t
#define DLPRwmutex			pthread_rwlock_t
#define DLPCond				pthread_cond_t
#define DLPSem				sem_t*


#endif


#endif // ISYSTEMDATATYPE_DEF_H
