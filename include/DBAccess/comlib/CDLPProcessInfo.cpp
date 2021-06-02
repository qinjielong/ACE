#include "ISystemLayInterface.h"
/**************************************************************************************************

  Description 	: ��ȡ������Ϣ
**************************************************************************************************/

CDLPProcessInfo::CDLPProcessInfo()
{
	m_processor_count = 0;
	
}
CDLPProcessInfo::~CDLPProcessInfo(void)
{

}


/**************************************************************************************************
  Function	:     
  DateTime	: 	
  Description	:// �������ܡ����ܵȵ�����
  Input		:// �������˵��������ÿ����������
  Output	:// �����������˵����
  Return	:// ��������ֵ��˵��
  Note		:// ��ע
**************************************************************************************************/

INT32	CDLPProcessInfo::DLPGetTotalCPUUsage(INT32 &iCPU)
{
	INT32 iRet = -1;
#ifdef _WIN32

	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;

	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	GetSystemTimes( &idleTime, &kernelTime, &userTime );

	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime ;

	//����1s���ٻ�ȡ
	MSLEEP(1000);
	
	GetSystemTimes( &idleTime, &kernelTime, &userTime );

	INT64 idle = CompareFileTime( preidleTime,idleTime);
	INT64 kernel = CompareFileTime( prekernelTime, kernelTime);
	INT64 user = CompareFileTime(preuserTime, userTime);

	//CPU������
	 iCPU = (kernel +user - idle) *100/(kernel+user);

	 //CPU������
	INT64 cpuidle = ( idle) *100/(kernel+user);

	iRet = 0;
#elif _LINUX
	DWORD total1;
	DWORD total2;

	DWORD user1;
	DWORD nice1;
	DWORD system1;
	DWORD idle1;
	DWORD iowait1;
	DWORD irq1;
	DWORD softirq1;

	DWORD user2;
	DWORD nice2;
	DWORD system2;
	DWORD idle2;
	DWORD iowait2;
	DWORD irq2;
	DWORD softirq2;

	char cpu[21];
	char text[201];

	FILE *fp;

	fp = fopen("/proc/stat", "r");
	
	fgets(text, 200, fp);
	if(strstr(text, "cpu"))
	{
		sscanf(text, "%s%lu%lu%lu%lu", cpu, &user1, &nice1, &system1, &idle1, &iowait1, &irq1, &softirq1);
	}
	
	fclose(fp);

	total1 = (user1+nice1+system1+idle1+iowait1+irq1+softirq1);

	MSLEEP(1000);        

	fp = fopen("/proc/stat", "r");
	
	fgets(text, 200, fp);
	if(strstr(text, "cpu"))
	{
		sscanf(text, "%s%lu%lu%lu%lu", cpu, &user2, &nice2, &system2, &idle2, &iowait2, &irq2, &softirq2);
	}
	
	fclose(fp);

	total2 = (user2+nice2+system2+idle2+iowait2+irq2+softirq2);

	if ((total2 - total1) == 0)
	{
		iCPU = 0;
	}
	else
	{
		iCPU = (INT32)((user2-user1+nice2 - nice1 +system2 - system1)*10000)/(total2-total1);
	}

	iRet = 0;
	
#endif	
	return	iRet;
}
/**************************************************************************************************
  Function	:     
  DateTime	: 	
  Description	:// �������ܡ����ܵȵ�����
  Input		:// �������˵��������ÿ����������
  Output	:// �����������˵����
  Return	:// ��������ֵ��˵��
  Note		:// ��ע
**************************************************************************************************/

INT32	CDLPProcessInfo::DLPGetTotalMemoryUsage(DWORD &dwMem)
{
	INT32 iRet = -1;
#ifdef _WIN32

	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof (statex);

	GlobalMemoryStatusEx (&statex);

	dwMem = statex.dwMemoryLoad;

	iRet = 0;
#elif _LINUX
	FILE *fd;          
	int n;             
	char buff[256];   
	char name[20];    
	unsigned long total; 
	char name2[20];
	unsigned long free; 

	fd = fopen ("/proc/meminfo", "r"); 

	fgets (buff, sizeof(buff), fd); 
	fgets (buff, sizeof(buff), fd); 
	fgets (buff, sizeof(buff), fd); 
	fgets (buff, sizeof(buff), fd); 
	sscanf (buff, "%s %u %s", name, &total, name2); 

	fgets (buff, sizeof(buff), fd); //��fd�ļ��ж�ȡ����Ϊbuff���ַ����ٴ浽��ʼ��ַΪbuff����ռ��� 
	sscanf (buff, "%s %u", name2, &free, name2); 

	fclose(fd);     //�ر��ļ�fd

	dwMem = (total - free)*100/total;
#endif
	
	return	iRet;
}


#ifdef _WIN32
INT64 CDLPProcessInfo::CompareFileTime ( FILETIME time1, FILETIME time2 )
{
	INT64 a = ((INT64)time1.dwHighDateTime) << 32 | time1.dwLowDateTime ;
	INT64 b = ((INT64)time2.dwHighDateTime) << 32 | time2.dwLowDateTime ;

	return   (b - a);
}
#endif

/**************************************************************************************************
  Function	:     
  DateTime	: 
  Description	:// �������ܡ����ܵȵ�����
  Input		:// �������˵��������ÿ����������
  Output	:// �����������˵����
  Return	:// ��������ֵ��˵��
  Note		:// ��ע
**************************************************************************************************/

INT32	CDLPProcessInfo::DLPGetProcessorNumber()
{
	INT32 iNum = 0;
#ifdef _WIN32
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	iNum = (INT32)info.dwNumberOfProcessors;
#endif
	return	iNum;
}

