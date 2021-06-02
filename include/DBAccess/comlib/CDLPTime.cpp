#include "ISystemLayInterface.h"



/******************************************************************************
����˵����ϵͳʱ��
******************************************************************************/


/********************************************************************************************
  Function		: DoGetTickCount    
  DateTime		: 	
  Description	: ��ȡ�Ӳ���ϵͳ�����������������ĺ�����
  Input			: NULL
  Output		: NULL
  Return		: ���شӲ���ϵͳ�����������������ĺ�����
  Note			:				// ��ע
********************************************************************************************/
UINT64   DoGetTickCount()
{

	UINT64 iTickCount=0;
	
#ifdef _WIN32

	DWORD dwTickCount=0;
	dwTickCount=GetTickCount();
	iTickCount=(UINT64)dwTickCount;

#elif _LINUX

	struct timeval current;
	gettimeofday(&current, NULL);
	iTickCount =1000LL*current.tv_sec + current.tv_usec/1000;

#endif

	return iTickCount;

}


/********************************************************************************************
  Function		: GetLocalTime    
  DateTime		: 
  Description	: ��ȡ���صĵ�ǰϵͳ���ں�ʱ��
  Input			: pLoaltime: ָ��һ���û��Զ���������ں�ʱ����Ϣ������Ϊ StruSysTime �ı������ñ����������溯����ȡ��ʱ����Ϣ
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
void    DoGetLocalTime(StruSysTimePtr pLoaltime)
{

	if(NULL == pLoaltime) return ;

#ifdef _WIN32
	GetLocalTime((LPSYSTEMTIME)pLoaltime);
#elif _LINUX

	time_t struTime;
	struct tm *pStruLocalTime;
	struct timeval tv;

	time(&struTime);
	pStruLocalTime=localtime(&struTime); 
	pLoaltime->wYear=1900+pStruLocalTime->tm_year;
	pLoaltime->wMonth=1+pStruLocalTime->tm_mon;
	pLoaltime->wDay=pStruLocalTime->tm_mday;
	pLoaltime->wDayOfWeek=1+pStruLocalTime->tm_wday;
	pLoaltime->wHour=pStruLocalTime->tm_hour;
	pLoaltime->wMinute=pStruLocalTime->tm_min;
	pLoaltime->wSecond=pStruLocalTime->tm_sec;

	//���Ϻ���   
	gettimeofday(&tv, NULL);   
	tv.tv_usec /= 1000;  

	pLoaltime->wMilliseconds=(INT)tv.tv_usec;


#endif

}
